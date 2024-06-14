#include "index.h"
int chooseNextDirection(House h, tuple<int,int> loc){
    int lb=0,ub=3;
    int ret = (rand() % (ub - lb + 1)) + lb;
    while(h.isWallInDirection(ret,loc))
        ret = (rand() % (ub - lb + 1)) + lb;
    return ret;
}

void cleaningAlgorithm(VaccumCleaner& vc, House& h, int maxSteps) {
    int steps = 0;
    int dir = -1;
    std::string step;
    std::vector<std::string> stepLog; // Vector to store steps log

    while (steps < maxSteps && h.getTotalDirtLeft() > 0) {
        std::ostringstream oss;
        oss << "step: " << steps << ". battery: " << vc.getBatterySteps() 
            << " location: [" << std::get<0>(vc.getCurrentLoc()) << "," 
            << std::get<1>(vc.getCurrentLoc()) << "]. direction: " << dir << ".";
        std::string stepDetails = oss.str();
        std::cout << stepDetails;
        stepLog.push_back(stepDetails);

        if (vc.getBatterySteps() - 1 == vc.getStepsFromDocking()) {
            steps += vc.goCharge(maxSteps - steps);
            dir = -1;
            std::cout << "CHARGE." << std::endl;
            stepLog.push_back("CHARGE.");
            continue;
        }

        if (h.getDirtLevel(vc.getCurrentLoc()) != DIRT0) {
            steps++;
            vc.clean(h);
            std::cout << "CLEAN." << std::endl;
            stepLog.push_back("CLEAN.");
            continue;
        }

        if (h.getTotalDirtLeft() > 0) {
            // Having charge and current location is clean, but there is more to clean. Decide on direction to move forward.
            if (dir == -1)
                dir = chooseNextDirection(h, vc.getCurrentLoc());
            // Move in this direction.
            vc.move(dir);
            steps++;
            std::ostringstream moveOss;
            moveOss << "MOVED IN DIR " << dir << ".";
            std::string moveDetails = moveOss.str();
            std::cout << moveDetails << std::endl;
            stepLog.push_back(moveDetails);
            // If there is a wall 1 step towards dir in the new location, discard dir. Else vc will continue to go in direction dir.
            if (h.isWallInDirection(dir, vc.getCurrentLoc()))
                dir = -1;
        } else {
            // Having charge and nothing to clean. Go back to docking station and finish (without charging).
            std::cout << "finished." << std::endl;
            stepLog.push_back("finished.");
            steps += vc.goCharge(maxSteps - steps, true);
        }
    }
    std::cout << "Total steps: " << steps << std::endl;
    std::cout << "Total dirt left: " << h.getTotalDirtLeft() << std::endl;

    // Write an output file called result.txt that includes all the steps, the number of steps, the amount of dirt left in the house,
    // an indication if the battery is dead, and an indication if there is more dirt or the clean was successful.
    std::ofstream file("result.txt");
    if (file.is_open()) {
        for (const auto& log : stepLog) {
            file << log << std::endl;
        }
        file << "Total steps: " << steps << std::endl;
        file << "Total dirt left: " << h.getTotalDirtLeft() << std::endl;
        if (vc.getBatterySteps() <= 0) {
            file << "Battery is dead." << std::endl;
        }
        if (h.getTotalDirtLeft() > 0) {
            file << "There is more dirt left." << std::endl;
        } else {
            file << "Clean was successful." << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Failed to open file result.txt." << std::endl;
    }
}



void updateDataFromFile(const std::string& fileName, std::tuple<int, int>& dockingStationLoc, std::vector<std::vector<int>>& houseMap, int& maxBatterySteps, int& maxSteps) {
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
                return;
            }

            std::get<0>(dockingStationLoc) = row;
            std::get<1>(dockingStationLoc) = col;
        } else {
            std::cerr << "Error: Empty file." << std::endl;
            file.close();
            return;
        }

        houseMap.clear(); // Clear the existing houseMap

        int row = 1; // Start from row 1 (assuming 0-based indexing)
        std::vector<int> rowLengths;
        while (std::getline(file, line)) {
            std::vector<int> rowData;
            std::istringstream iss(line);
            int num;
            while (iss >> num) {
                rowData.push_back(num);
            }

            if (!rowData.empty()) {
                houseMap.push_back(rowData);
                rowLengths.push_back(rowData.size());
            } else {
                std::cerr << "Error: Empty row detected at line " << row << "." << std::endl;
                file.close();
                return;
            }

            row++;
        }
        file.close();

        // Check if all rows have the same length
        for (size_t i = 1; i < rowLengths.size(); i++) {
            if (rowLengths[i] != rowLengths[0]) {
                std::cerr << "Error: Row lengths are not consistent at row " << i + 1 << "." << std::endl;
                return;
            }
        }

        int rows = houseMap.size();
        int cols = houseMap[0].size();

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
            // Set the first row to -1
            for (int i = 0; i < cols; i++) {
                houseMap[0][i] = -1;
            }

            // Set the last row to -1
            for (int i = 0; i < cols; i++) {
                houseMap[rows - 1][i] = -1;
            }

            // Set the first and last columns to -1
            for (int i = 0; i < rows; i++) {
                houseMap[i][0] = -1;
                houseMap[i][cols - 1] = -1;
            }
        }
    } else {
        std::cerr << "Error: Failed to open file " << fileName << "." << std::endl;
    }
}


// void makeOutputFile(VaccumCleaner& vc, House& h){
//     //#TODO
// }

int main(int argc, char *argv[]){
    if(argc!=2 && argv[1]){
        //throw error
    }
    tuple<int,int> dockingStationLoc;
    vector<vector<int> > houseMap;
    int maxBatterySteps=0, MaxSteps=0;
    updateDataFromFile(argv[1],dockingStationLoc,houseMap,maxBatterySteps,MaxSteps);
    if (houseMap.empty() || houseMap[0].empty()) {
        std::cerr << "Error: houseMap is empty or not properly populated" << std::endl;
        return 1;
    }
    // print the map
    House* h = new House(dockingStationLoc,houseMap);
    VaccumCleaner* vc = new VaccumCleaner(maxBatterySteps,dockingStationLoc);
    cleaningAlgorithm(*vc,*h,MaxSteps);
    //makeOutputFile(*vc,*h);
    return 0;
}