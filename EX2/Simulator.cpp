#include "Simulator.hpp"
#include "WallsSensorObject.hpp"
#include "DirtSensorObject.hpp"
#include "BatteryMeterObject.hpp"
#include "Algorithm.hpp"
using std::vector, std::pair, std::ifstream, std::stringstream;
using std::cerr, std::endl;

// Function to trim leading and trailing whitespace from a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Function to check if a string represents a valid integer
bool isInteger(const string& str) {
    if (str.empty()) return false;
    stringstream ss(str);
    int num;
    ss >> num;
    return !ss.fail() && ss.eof();
}

void Simulator::setAlgorithm(unique_ptr<AbstractAlgorithm> algorithm){
    alg = std::move(algorithm);
    alg->setBatteryMeter(BatteryMeterObject(shared_from_this()));
    alg->setDirtSensor(DirtSensorObject(shared_from_this()));
    alg->setWallsSensor(WallsSensorObject(shared_from_this()));
    alg->setMaxSteps(maxSteps);
    
}
int Simulator::readHouseFile(const string& filename){
    size_t maxSteps;
    size_t maxBattery;
    size_t rows;
    size_t cols;
    vector<vector<int>> grid; // Changed to store int values for wall=-1, ' '=0, D=-2, and dirt levels as int
    pair<int, int> dockingStation; // Pair to store row and column of the docking station

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file" << endl;
        return -1;
    }

    string line;
    size_t lineNumber = 0;
    size_t currentRows = 0;
    bool foundDockingStation = false;

    while (getline(file, line)) {
        lineNumber++;

        if (lineNumber == 2) {
            // Parse MaxSteps
            size_t pos = line.find('=');
            if (pos == string::npos) {
                cerr << "Error: MaxSteps line format is invalid" << endl;
                file.close();
                return -1;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                cerr << "Error: MaxSteps value is not a valid integer" << endl;
                file.close();
                return -1;
            }
            maxSteps = stoi(value);
        } else if (lineNumber == 3) {
            // Parse MaxBattery
            size_t pos = line.find('=');
            if (pos == string::npos) {
                cerr << "Error: MaxBattery line format is invalid" << endl;
                file.close();
                return -1;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                cerr << "Error: MaxBattery value is not a valid integer" << endl;
                file.close();
                return -1;
            }
            maxBattery = stoi(value);
        } else if (lineNumber == 4) {
            // Parse Rows
            size_t pos = line.find('=');
            if (pos == string::npos) {
                cerr << "Error: Rows line format is invalid" << endl;
                file.close();
                return -1;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                cerr << "Error: Rows value is not a valid integer" << endl;
                file.close();
                return -1;
            }
            rows = stoi(value);
        } else if (lineNumber == 5) {
            // Parse Cols
            size_t pos = line.find('=');
            if (pos == string::npos) {
                cerr << "Error: Cols line format is invalid" << endl;
                file.close();
                return -1;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                cerr << "Error: Cols value is not a valid integer" << endl;
                file.close();
                return -1;
            }
            cols = stoi(value);
            grid.resize(rows, vector<int>(cols, 0));
        } else if (lineNumber >= 6 && currentRows < rows) {
            // Parse house grid rows
            if (line.size() < cols) {
                line.append(cols - line.size(), ' '); // Pad with spaces if line is shorter
            }
            for (size_t i = 0; i < cols; ++i) {
                char c = line[i];
                if (c == ' ') {
                    grid[currentRows][i] = 0; // Empty space
                } else if (c == 'W') {
                    grid[currentRows][i] = -1; // Wall
                } else if (c == 'D') {
                    if (foundDockingStation) {
                        cerr << "Error: More than one docking station found" << endl;
                        file.close();
                        return -1;
                    }
                    grid[currentRows][i] = -2; // Docking station
                    dockingStation = {currentRows, i}; // Save docking station coordinates
                    foundDockingStation = true;
                } else if (c >= '0' && c <= '9') {
                    grid[currentRows][i] = c - '0'; // Dirt levels (convert char to int)
                } else {
                    grid[currentRows][i] = 0; // every other char is considered as an empty space
                }
            }
            currentRows++;
        }
    }

    file.close();

    if (!foundDockingStation) {
        cerr << "Error: No docking station found" << endl;
        return -1;
    }

    //initialize data members from file data
    this->maxSteps = maxSteps;
    this->h = make_unique<House>(grid);
    this->vc = make_unique<VaccumCleaner>(maxBattery,dockingStation);
    return maxBattery;
}
void Simulator::run(){
    size_t steps = 0;
    Step action;
    vector<string> directionsTranslate{"North", "East", "South","West"};
    StepLog.reserve(maxSteps);
    string logMessage;
//     vector<int> wallSensor{0, 0, 0, 0}; // wallSensor[i] == 1 if there is a wall in direction dir, where NORTH=0, EAST=1, SOUTH=2, WEST=3
    

    while (steps < maxSteps) {
        if (vc->getCurrentLoc() == h->getDockingStationLoc() && h->getTotalDirtLeft() == 0)//success
            break;
        // Ask algorithm for next move decision
        action = alg->nextStep();
        if(action==Step::Finish)
            stepDescriptor.append("F");
            break;
        // Perform the move we got from the algorithm
//         if (action == -1) {
//             logMessage = "failure. battery is empty and not on docking station";
//             StepLog.push_back(logMessage);
//             break;
//         }
        int x = vc->getCurrentLoc().first;
        int y = vc->getCurrentLoc().second;
        if (action == Step::Stay) {
            steps++;
            int err;
            if (vc->getCurrentLoc() == h->getDockingStationLoc()) {//staying on docking is charging
                logMessage = "step: " + to_string(steps) + ". battery: " + to_string(vc->getBatterySteps()) + ". current location: [" + to_string(x) + "," + to_string(y) + "]. action: Stay (charge)";
                StepLog.push_back(logMessage);
                err = vc->charge();
            } else {//staying elsewhere is cleaning
                logMessage = "step: " + to_string(steps) + ". battery: " + to_string(vc->getBatterySteps()) + ". current location: [" + to_string(x) + "," + to_string(y) + "]. action: Stay (clean).";
                StepLog.push_back(logMessage);
                err=vc->clean();
            }
            if (err) {
                logMessage = "failure. algorithm tried to make vacuum cleaner clean with no battery.";
                StepLog.push_back(logMessage);
                break;
            }
            stepDescriptor.append("s");
            continue;
        } else { // need to advance 1 step in the direction returned with action
            steps++;
            Direction dir = static_cast<Direction>(action);
            logMessage = "step: " + to_string(steps) + ". battery: " + to_string(vc->getBatterySteps()) + ". current location: [" + to_string(x) + "," + to_string(y) + "]. action: move in direction " + directionsTranslate[static_cast<int>(action)];
            StepLog.push_back(logMessage);
            if (h->isWallInDirection(dir, vc->getCurrentLoc())) {
                logMessage = "failure. algorithm tried to move vacuum cleaner into a wall.";
                StepLog.push_back(logMessage);
                break;
            }
            int err = vc->move(dir);
            if (err) {
                logMessage = "failure. algorithm tried to move vacuum cleaner with no battery.";
                StepLog.push_back(logMessage);
                break;
            } 
            stepDescriptor.append(to_string(directionsTranslate[static_cast<int>(action)].at(0)));
            continue;
        }
    }
}

void Simulator::makeOutputFile() {
    // Open the file in write mode
    std::ofstream outFile("output.txt");
    
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }
    string status;
    if(vc->getBatterySteps()<1 && vc->getCurrentLoc() != h->getDockingStationLoc())
        status="DEAD";
    else if(stepDescriptor.back()=='F') status = "FINISHED";
    else status = "WORKING";
    // Write the data to the file
    outFile << "NumSteps = " << StepLog.size() << std::endl;
    outFile << "DirtLeft = " << h->getTotalDirtLeft() << std::endl;
    outFile << "Status = " << status << std::endl;
    outFile << "Steps:" << stepDescriptor <<std::endl;

    // Close the file
    outFile.close();
}