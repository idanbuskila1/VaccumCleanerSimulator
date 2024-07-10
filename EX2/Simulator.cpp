#include "Simulator.h"
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

void Simulator::setAlgorithm(unique_ptr<Algorithm> algorithm){
    alg = std::move(algorithm);
    alg->setBatteryMeter(BatteryMeterObject(vc));
    alg->setDirtSensor(DirtSensorObject(h,vc));
    alg->setWallsSensor(WallsSensorObject(h,vc));
    alg->setMaxSteps(maxSteps);
}
int Simulator::readHouseFile(const string& filename){
    int maxSteps;
    int maxBattery;
    int rows;
    int cols;
    vector<vector<int>> grid; // Changed to store int values for wall=-1, ' '=0, D=-2, and dirt levels as int
    pair<int, int> dockingStation; // Pair to store row and column of the docking station

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file" << endl;
        return 1;
    }

    string line;
    int lineNumber = 0;
    int currentRows = 0;
    bool foundDockingStation = false;

    while (getline(file, line)) {
        lineNumber++;

        if (lineNumber == 2) {
            // Parse MaxSteps
            size_t pos = line.find('=');
            if (pos == string::npos) {
                cerr << "Error: MaxSteps line format is invalid" << endl;
                file.close();
                return 1;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                cerr << "Error: MaxSteps value is not a valid integer" << endl;
                file.close();
                return 1;
            }
            maxSteps = stoi(value);
        } else if (lineNumber == 3) {
            // Parse MaxBattery
            size_t pos = line.find('=');
            if (pos == string::npos) {
                cerr << "Error: MaxBattery line format is invalid" << endl;
                file.close();
                return 1;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                cerr << "Error: MaxBattery value is not a valid integer" << endl;
                file.close();
                return 1;
            }
            maxBattery = stoi(value);
        } else if (lineNumber == 4) {
            // Parse Rows
            size_t pos = line.find('=');
            if (pos == string::npos) {
                cerr << "Error: Rows line format is invalid" << endl;
                file.close();
                return 1;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                cerr << "Error: Rows value is not a valid integer" << endl;
                file.close();
                return 1;
            }
            rows = stoi(value);
        } else if (lineNumber == 5) {
            // Parse Cols
            size_t pos = line.find('=');
            if (pos == string::npos) {
                cerr << "Error: Cols line format is invalid" << endl;
                file.close();
                return 1;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                cerr << "Error: Cols value is not a valid integer" << endl;
                file.close();
                return 1;
            }
            cols = stoi(value);
            grid.resize(rows, vector<int>(cols, 0));
        } else if (lineNumber >= 6 && currentRows < rows) {
            // Parse house grid rows
            if (line.size() < cols) {
                line.append(cols - line.size(), ' '); // Pad with spaces if line is shorter
            }
            for (int i = 0; i < cols; ++i) {
                char c = line[i];
                if (c == ' ') {
                    grid[currentRows][i] = 0; // Empty space
                } else if (c == 'W') {
                    grid[currentRows][i] = -1; // Wall
                } else if (c == 'D') {
                    if (foundDockingStation) {
                        cerr << "Error: More than one docking station found" << endl;
                        file.close();
                        return 1;
                    }
                    grid[currentRows][i] = -2; // Docking station
                    dockingStation = {currentRows, i}; // Save docking station coordinates
                    foundDockingStation = true;
                } else if (c >= '0' && c <= '9') {
                    grid[currentRows][i] = c - '0'; // Dirt levels (convert char to int)
                } else {
                    cerr << "Error: Invalid character in house map" << endl;
                    file.close();
                    return 1;
                }
            }
            currentRows++;
        }
    }

    file.close();

    if (!foundDockingStation) {
        cerr << "Error: No docking station found" << endl;
        return 1;
    }
    return 0;
}
