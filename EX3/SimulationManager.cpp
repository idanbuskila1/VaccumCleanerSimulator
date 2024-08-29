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
string SimulationManager::processHouseFile(const string& filename, const string& strippedName=""){
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
    houseFiles.emplace_back(strippedName,maxBattery, maxSteps, grid, dockingStation);
    return "";
}

//get path of the houses folder, and initialize houseFiles vector with all the .house files in the folder
void SimulationManager::initializeHouses(string path){
    string err;
    houseFiles.reserve(15);
    if (path == ".") {
        path = std::filesystem::current_path().string();
    }
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".house") {
            //cout<<entry.path().stem().string()<<endl;
            if ((err = processHouseFile(entry.path().string(),entry.path().stem().string() )) != "") {
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
    scores.resize(AlgorithmRegistrar::getAlgorithmRegistrar().count(), vector<int>(houseFiles.size(), 0));
}
auto SimulationManager::getAlgoByIndex(int idx){
    int i=0;
    for (const auto& algo: AlgorithmRegistrar::getAlgorithmRegistrar()){
        if(i==idx){
            return algo;
        }
        i++;
    }
    return *AlgorithmRegistrar::getAlgorithmRegistrar().begin();
}
int SimulationManager::getSimulationNumber(){
    int ret = simulationNo++;
    size_t algosCount = AlgorithmRegistrar::getAlgorithmRegistrar().count();
    if(ret>=static_cast<int>(algosCount*houseFiles.size())){
        isSimulationOver=true;
        return -1;
    }
    return ret;
}
std::vector<Simulator> SimulationManager::prepareAllSimulations(){
    std::vector<Simulator> simulators;
    size_t algosCount = AlgorithmRegistrar::getAlgorithmRegistrar().count();
    size_t simulationsCount = algosCount*houseFiles.size();
    simulators.reserve(simulationsCount);
    for(size_t i=0; i<simulationsCount; i++){
        simulators.emplace_back();
        int houseIdx, algIdx;
        houseIdx = i / algosCount;
        algIdx = i % algosCount;
        auto algo = getAlgoByIndex(algIdx);
        auto house = houseFiles[houseIdx];
        cout<<"house: "<<house.houseName<<" algo: "<<algo.name()<<"thread name:"<<std::this_thread::get_id()<<endl;
        simulators.back().setSimulationData(house);
        auto algorithm = algo.create();
        simulators.back().setAlgorithm(std::move(algorithm));
    }
    return simulators;
}

void SimulationManager::sumerrizeAllSimulations(std::vector<Simulator>& simulators){
    int N = static_cast<int>(simulators.size());
    for(int i=0; i<N; i++){
        int houseIdx, algoIdx;
        size_t algosCount = AlgorithmRegistrar::getAlgorithmRegistrar().count();
        houseIdx = i / algosCount;
        algoIdx = i % algosCount;
        if(!isSummaryOnly){
            string houseName = houseFiles[houseIdx].houseName;
            string algoName = getAlgoByIndex(algoIdx).name();
            simulators[i].makeOutputFile(houseName +"-"+ algoName+".txt");
            simulators[i].makeLog(houseName +"-"+ algoName+".txt");
        }
        scores[algoIdx][houseIdx] = simulators[i].calcScore();
    }
        
}
void SimulationManager::makeSummary(){
    std::ofstream summaryFile("summary.csv");
    if (summaryFile.is_open()) {
        // Write headers
        summaryFile << "Algorithm,";
        for(const auto& h:houseFiles){
            summaryFile << h.houseName << ",";
        }
        summaryFile << "\n";


        size_t i=0;
        for (const auto& algo : AlgorithmRegistrar::getAlgorithmRegistrar()) {
            summaryFile << algo.name() << ",";
            for (size_t j = 0; j < houseFiles.size(); ++j) {
                summaryFile << scores[i][j] << ",";
            }
            i++;
            summaryFile << "\n";
        }
        summaryFile.close();
    } else {
        std::cerr << "Error: Could not create summary file" << std::endl;
    }
}

void SimulationManager::closeAlgorithms(){
    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    for(void* handle:algorithmEntries){
        dlclose(handle);
    }
}

void SimulationManager::createErrFile(string errorFilePath, string msg){
    std::ofstream errorFile(errorFilePath);
    if (errorFile.is_open()) {
        errorFile << msg << std::endl;
        errorFile.close();
    } else {
        std::cerr << "Failed to create error file: " << errorFilePath << std::endl;
    }
}

void SimulationManager::openAlgorithms(string path){
    // Open the directory
    if (path == ".") {
        path = std::filesystem::current_path().string();
    }
    DIR* dir = opendir(path.c_str());
    size_t prev_count=0;
    if (dir) {
        // Read all the files in the directory
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename = entry->d_name;
            std::string filepath = path + "/" + filename;
            // Check if the file is a shared object (.so)
            if (filename.length() > 3 && filename.substr(filename.length() - 3) == ".so") {
                // Open the shared object with dlopen
                //cout<<"algo "<<entry->d_name<<endl;
                void* handle = dlopen(filepath.c_str(), RTLD_NOW);
                if (handle) {
                    algorithmEntries.push_back(handle);
                    if(AlgorithmRegistrar::getAlgorithmRegistrar().count() != prev_count+1){
                        createErrFile(filename.substr(0, filename.length() - 3) + ".error", "couldnt registar the algorithm (count did not updated after dlopen).");
                    }
                    prev_count++;
                } else createErrFile(filename.substr(0, filename.length() - 3) + ".error", dlerror());
            }
        }      
        // Close the directory
        closedir(dir);
    } else {
        std::cerr << "Failed to open directory: " << path << std::endl;
    }
}