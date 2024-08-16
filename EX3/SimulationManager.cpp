#include "SimulationManager.hpp"


string SimulationManager::trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Function to check if a string represents a valid integer
bool SimulationManager::isInteger(const string& str) {
    if (str.empty()) return false;
    std::stringstream ss(str);
    int num;
    ss >> num;
    return !ss.fail() && ss.eof();
}
// Function to process a house file create a InputFileData object and add it to houseFiles vector
string SimulationManager::processHouseFile(const string& filename){
    size_t maxSteps;
    size_t maxBattery;
    size_t rows;
    size_t cols;
    vector<vector<int>> grid; // Changed to store int values for wall=-1, ' '=0, D=-2, and dirt levels as int
    pair<int, int> dockingStation; // Pair to store row and column of the docking station
    string error = "";
    std::ifstream file(filename);
    if (!file.is_open()) {
        error = "Error: Unable to open file";
        cerr << error << endl;
        return error;
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
                error = "Error: MaxSteps line format is invalid";
                cerr << error << endl;
                file.close();
                return error;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                error = "Error: MaxSteps value is not a valid integer";
                cerr << error << endl;
                file.close();
                return error;
            }
            maxSteps = stoi(value);
        } else if (lineNumber == 3) {
            // Parse MaxBattery
            size_t pos = line.find('=');
            if (pos == string::npos) {
                error = "Error: MaxBattery line format is invalid";
                cerr << error << endl;
                file.close();
                return error;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                error = "Error: MaxBattery value is not a valid integer";
                cerr << error << endl;
                file.close();
                return error;
            }
            maxBattery = stoi(value);
        } else if (lineNumber == 4) {
            // Parse Rows
            size_t pos = line.find('=');
            if (pos == string::npos) {
                error = "Error: Rows line format is invalid";
                cerr << error << endl;
                file.close();
                return error;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                error = "Error: Rows value is not a valid integer";
                cerr << error << endl;
                file.close();
                return error;
            }
            rows = stoi(value);
        } else if (lineNumber == 5) {
            // Parse Cols
            size_t pos = line.find('=');
            if (pos == string::npos) {
                error = "Error: Cols line format is invalid";
                cerr <<error << endl;
                file.close();
                return error;
            }
            string value = trim(line.substr(pos + 1));
            if (!isInteger(value)) {
                error = "Error: Cols value is not a valid integer";
                cerr << error << endl;
                file.close();
                return error;
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
                        error = "Error: More than one docking station found";
                        cerr << error << endl;
                        file.close();
                        return error;
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
        error = "Error: No docking station found";
        cerr << error << endl;
        return error;
    }
    //house file is valid - add to houseFiles
    string prefix = "houses/";
    string suffix = ".house";
    size_t prefixPos = filename.find(prefix);
    size_t suffixPos = filename.rfind(suffix);
    string trimmedFilename = filename.substr(prefixPos + prefix.length(), suffixPos - (prefixPos + prefix.length()));
    houseFiles.emplace_back(trimmedFilename,maxBattery, maxSteps, grid, dockingStation);
    return "";
}

//get path of the houses folder, and initialize houseFiles vector with all the .house files in the folder
void SimulationManager::initializeHouses(string path){
    string err;
    houseFiles.reserve(15);
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".house") {
            if ((err = processHouseFile(entry.path().string())) != "") {
                 // Create an error file with the same name but with .error extension
                std::string errorFilePath = entry.path().stem().string() + ".error";
                std::ofstream errorFile(errorFilePath);
                if (errorFile.is_open()) {
                    errorFile << "Error: Invalid house file " << entry.path().string() <<".\n"<<err<< std::endl;
                    errorFile.close();
                } else {
                    std::cerr << "Error: Could not create error file " << errorFilePath << std::endl;
                }
            }
        }
    }
}

void SimulationManager::operateSimulations(){
    //size_t numOfAlgos = AlgorithmRegistrar::getAlgorithmRegistrar().count();
    for (size_t i = 0; i < houseFiles.size(); ++i) {
        for (const auto& algo: AlgorithmRegistrar::getAlgorithmRegistrar()){
            cout<<"house: "<<houseFiles[i].houseName<<" algo: "<<algo.name()<<endl;
            Simulator simulator;
            simulator.setSimulationData(houseFiles[i]);
            auto algorithm = algo.create();
            simulator.setAlgorithm(std::move(algorithm));
            simulator.run();
            if (simulator.getIsRuntimeError()) {
                cerr << "Error: " << simulator.getErrorMessage() << endl;
            } else {
                cout<<houseFiles[i].houseName +"-"+ algo.name()+".txt"<<endl;

                simulator.makeOutputFile(houseFiles[i].houseName +"-"+ algo.name()+".txt");
                simulator.makeLog(houseFiles[i].houseName +"-"+ algo.name()+".txt");
                // simulator.makeOutputFile("outputs1.txt");
                // simulator.makeLog("2.txt");

            }
        }
    }
}