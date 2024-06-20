#include "index.h"

int updateDataFromFile(const std::string& fileName, std::tuple<int, int>& dockingStationLoc, std::vector<std::vector<int>>& houseMap, int& maxBatterySteps, int& maxSteps) {
    std::ifstream file(fileName);

    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line)) { // Read the first line with docking station location and max steps
            std::istringstream iss(line);
            int row, col;
            int count = 0;
            while (iss >> (count == 0 ? row : (count == 1 ? col : (count == 2 ? maxBatterySteps : maxSteps)))) {
                count++;
            }
            if (count != 4 || row < 0 || col < 0 || maxBatterySteps <= 0 || maxSteps <= 0) {
                std::cerr << "Error: The first line must contain exactly four positive integers." << std::endl;
                file.close();
                return 1;
            }

            std::get<0>(dockingStationLoc) = row;
            std::get<1>(dockingStationLoc) = col;
        } else {
            std::cerr << "Error: Empty file." << std::endl;
            file.close();
            return 1;
        }

        houseMap.clear(); // Clear the existing houseMap

        int row = 1; // Start from row 1 (assuming 0-based indexing)
        std::vector<int> rowLengths;
        while (std::getline(file, line)) {
            std::vector<int> rowData;
            std::istringstream iss(line);
            int num;
            while (iss >> num) {
                if (num < -1 || num > 9){
                    std::cerr << "Error: Invalid value "<<num<<" detected at line " << row << "." << std::endl;
                    file.close();
                    return 1;
                }
                rowData.push_back(num);
            }
            if (!rowData.empty()) {
                houseMap.push_back(rowData);
                rowLengths.push_back(rowData.size());
            } else {
                std::cerr << "Error: Empty row detected at line " << row << "." << std::endl;
                file.close();
                return 1;
            }

            row++;
        }
        file.close();
        if (houseMap.empty() || houseMap[0].empty()) {
            std::cerr << "Error: houseMap is empty or not properly populated" << std::endl;
            return 1;
        }
        // Check if all rows have the same length
        for (size_t i = 1; i < rowLengths.size(); i++) {
            if (rowLengths[i] != rowLengths[0]) {
                std::cerr << "Error: Row lengths are not consistent at row " << i + 1 << ", or it has wrong characters in it." << std::endl;
                return 1;
            }
        }
        int rows = houseMap.size();
        int cols = houseMap[0].size();

        //  check that the docking station location is within the houseMap boundaries (withput first and last row/col beacuse the house is sorrounded by walls) and has a value of 0
        if (std::get<0>(dockingStationLoc) >= rows-1 || std::get<1>(dockingStationLoc) >= cols-1 || std::get<0>(dockingStationLoc)<=0|| std::get<1>(dockingStationLoc)<=0) {
            std::cerr << "Error: Docking station location is out of bounds. It must not be outside of the house coordinates, or on the first/last row or column (house is sorrounded by walls)." << std::endl;
            return 1;
        }
        if (houseMap[std::get<0>(dockingStationLoc)][std::get<1>(dockingStationLoc)] != 0) {
            std::cerr << "Error: Docking station location must have a value of 0 on the house map.(no dirt and no walls)" << std::endl;
            return 1;
        }
            
        // Check if the matrix already has -1 values on the edges
        bool hasEdges = true;
        for (int i = 0; i < cols; i++) {
            if (houseMap[0][i] != -1 || houseMap[rows - 1][i] != -1) {
                hasEdges = false;
                break;
            }
        }
        for (int i = 0; i < rows; i++) {
            if (houseMap[i][0] != -1 || houseMap[i][cols - 1] != -1) {
                hasEdges = false;
                break;
            }
        }

        // Modify the edges only if they don't have -1 values
        if (!hasEdges) {
            
            // Set the first and last rows to -1
            for (int i = 0; i < cols; i++) {
                houseMap[0][i] = -1;
                houseMap[rows - 1][i] = -1;
            }

            // Set the first and last columns to -1
            for (int i = 0; i < rows; i++) {
                houseMap[i][0] = -1;
                houseMap[i][cols - 1] = -1;
            }
        }
        
    } 
    else {
        std::cerr << "Error: Failed to open file " << fileName << "." << std::endl;
        return 1;
    }
    return 0;
}

void makeOutputFile(VaccumCleaner& vc, House& h, std::vector<std::string>* stepLog) {
    std::ofstream file("result.txt");
    if (file.is_open()) {
        file << "Total steps: " << stepLog->size() << "." << "\n";
        file << "Total dirt left: " << h.getTotalDirtLeft() << "." << "\n";
        file << "Vacuum Cleaner battery is: " << vc.getBatterySteps() << "." << "\n";
        file << "House's state after cleaning:"<<"\n"<<h.toString();
        std::tuple<int, int> curLoc = vc.getCurrentLoc();
        std::tuple<int, int> dockLoc = h.getDockingStationLoc();

        if (h.getTotalDirtLeft() > 0 || curLoc != dockLoc) {
            file << "Clean wasn't successful. There is more dirt left and/or the cleaner is not in the docking station." << std::endl;
        } else {
            file << "Clean was successful. The house is clean and the Vacuum Cleaner is at the docking station." << std::endl;
        }

        file << std::endl;
        file << "Logging of all steps performed by the Vacuum Cleaner:" << std::endl;
        for (const auto& log : *stepLog) {
            file << log << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Failed to open file result.txt." << std::endl;
    }
}



int main(int argc, char *argv[]){ 
    if(argc!=2 || !argv[1]){
        std::cerr << "Error:must pass exactly 1 argument of input file name." << std::endl;
        return 1;
    }
    srand(time(0));//set seed for randomness betwween program runs
    //initialize data from input file
    std::tuple<int,int> dockingStationLoc;
    std::vector<std::vector<int> > houseMap;
    int maxBatterySteps=0, MaxSteps=0;
    int isError = updateDataFromFile(argv[1],dockingStationLoc,houseMap,maxBatterySteps,MaxSteps);
    if(isError)
        return 1;
    //initialzie API objects and clean house h with vaccum cleaner vc according to algorithm alg
    House h(dockingStationLoc,houseMap);
    VaccumCleaner vc(maxBatterySteps,dockingStationLoc);
    CleaningAlgorithm alg(maxBatterySteps,MaxSteps);
    std::vector<std::string>* stepLog = vc.cleanHouse(h,alg,MaxSteps);
    //create output dile from the log, free memory  and finish with success.
    makeOutputFile(vc, h, stepLog);
    delete stepLog;
    return 0;
}