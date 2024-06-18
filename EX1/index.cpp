#include "index.h"

/*
//randomly chosses 1 of 4 directions, and if there is no wall 1 step in this direction-return it.
int chooseNextDirection(House h, std::tuple<int,int> loc){
    int lb=0,ub=3;
    int ret = (rand() % (ub - lb + 1)) + lb;
    while(h.isWallInDirection(ret,loc))
        ret = (rand() % (ub - lb + 1)) + lb;
    return ret;
}
//go back to docking station according to the path in the stack. if dontCahrge==false stay until fully charged.
//return number of steps taken in the process.
int goToDockingStation(int maxStepsAllowed,std::stack<int>& path,VaccumCleaner& vc, House& h,bool dontCharge=false){
    int curDir;
    int stepsToDocking = path.size();
    //reach docking station.
    while (path.size()>0 && maxStepsAllowed>0)
    {
        curDir=path.top();
        vc.move(curDir);
        path.pop();
        maxStepsAllowed--;
    }
    if(maxStepsAllowed==0) return stepsToDocking-path.size();//if we stoped because steps limit, stop and return the steps we made.
    if(dontCharge) return stepsToDocking;//dont charge, only go back to docking.
    //stay until full battery or steps limit is reached.
    int stepsStayed= 0;
    float maxBattery = vc.getMaxBatterySteps();
    float chargePerStep = vc.getChargePerStep();
    while(maxStepsAllowed>0 && vc.getBatterySteps()+chargePerStep<=maxBattery){
        vc.stay(h);
        stepsStayed++;
        maxStepsAllowed--;
    }
    return stepsToDocking + stepsStayed;
}

void cleaningAlgorithm(VaccumCleaner& vc, House& h, int maxSteps) {
    int steps = 0;
    int dir = -1;
    int stepsInGoToCharge;
    std::stack<int> pathToDocking;
    std::vector<std::string> stepLog; // Vector to store steps log

    while (steps < maxSteps) {
        std::ostringstream oss;
        oss << "step: " << steps << ". battery: " << vc.getBatterySteps() 
            << " location: [" << std::get<0>(vc.getCurrentLoc()) << "," 
            << std::get<1>(vc.getCurrentLoc()) << "]. direction: " << dir << ".";
        std::string stepDetails = oss.str();
        std::cout << stepDetails;
        stepLog.push_back(stepDetails);
        if(vc.getBatterySteps()<=0 && pathToDocking.size()>0){
             std::cout << "failure. battery is empty and not on docking station" << std::endl;
             stepLog.push_back("\tfailure. battery is empty and not on docking station" );
             break;
        }
        if(maxSteps-steps==pathToDocking.size()){
             // The way back to the docking station is exactly as long as the steps left. Go back to docking station and finish (without charging) so we wont fail in mission.
            std::cout << "finished. return to docking station." << std::endl;
            stepLog.push_back("\tfinished. return to docking station.");
            steps += goToDockingStation(maxSteps - steps,pathToDocking,vc,h, true);
            continue;
        }
        if (vc.getBatterySteps() - pathToDocking.size() <2 ) {
            //we have exactly the battery to go back to charge before dying - go charge to full.
            int pathLen = pathToDocking.size();
            float oldBattery = vc.getBatterySteps();
            std::ostringstream chargeOss;
            chargeOss<<"\tCHARGE. went to the docking station in "<<pathLen<<" steps, and stayed charging for ";
            stepsInGoToCharge = goToDockingStation(maxSteps - steps,pathToDocking,vc,h);
            steps+=stepsInGoToCharge;
            chargeOss<<stepsInGoToCharge-pathLen<<" steps. battery charged from "<<oldBattery<<" to "<<vc.getBatterySteps();
            dir = -1;
            stepLog.push_back(chargeOss.str());
            std::cout<<chargeOss.str()<<std::endl;
            continue;
        }

        if (h.getDirtLevel(vc.getCurrentLoc()) != DIRT0) {
            steps++;
            vc.stay(h);
            std::cout << "CLEAN." << std::endl;
            stepLog.push_back("\tCLEAN.");
            continue;
        }

        // Having charge, maxSteps boundary is not reached and current location is clean- advance to another spot. 
        if (dir == -1)//Decide on direction to move forward.
            dir = chooseNextDirection(h, vc.getCurrentLoc());
        vc.move(dir); //move 1 step in this direction.
        steps++;
        //update pathToDocking stack
        if(!pathToDocking.empty() && dir == pathToDocking.top())//if we just made the first step in the stack - pop it.
                pathToDocking.pop();
        else{//push the oposite direction to the stack
            if(dir == NORTH) pathToDocking.push(SOUTH);
            else if(dir == SOUTH) pathToDocking.push(NORTH);
            else if(dir == EAST) pathToDocking.push(WEST);
            else if(dir == WEST) pathToDocking.push(EAST);
        }

        std::ostringstream moveOss;
        moveOss << "\tMOVED IN DIR " << dir << "." ;
        std::string moveDetails = moveOss.str();
        std::cout << moveDetails << std::endl;
        stepLog.push_back(moveDetails);
        // If there is a wall 1 step towards dir in the new location, discard dir. Else vc will continue to go in direction dir.
        if (h.isWallInDirection(dir, vc.getCurrentLoc()))
            dir = -1;

    }
    std::cout << "Total steps: " << steps << std::endl;
    std::cout << "Total dirt left: " << h.getTotalDirtLeft() << std::endl;
    // Write an output file called result.txt that includes all the steps, the number of steps, the amount of dirt left in the house,
    // an indication if the battery is dead, and an indication if there is more dirt or the clean was successful.
    std::ofstream file("result.txt");
    if (file.is_open()) {
        file << "Total steps: " << steps <<"."<< std::endl;
        file << "Total dirt left: " << h.getTotalDirtLeft() <<"."<< std::endl;
        file << "Vaccum Cleaner battery is: " <<vc.getBatterySteps()<<"."<< std::endl;
        if (h.getTotalDirtLeft() > 0 || pathToDocking.size()>0) {
            file << "Clean wasn't successful. There is more dirt left and/or the cleaner is not in the docking station." << std::endl;
        } else {
            file << "Clean was successful. the house is clean and the Vaccum Cleaner is at the docking station." << std::endl;
        }
        file<<std::endl;
        file<<"Logging of all steps performed by the Vaccum Cleaner:"<<std::endl;
        for (const auto& log : stepLog) {
            file << log << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Error: Failed to open file result.txt." << std::endl;
    }
}*/



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
                std::cerr << "Error: Row lengths are not consistent at row " << i + 1 << "." << std::endl;
                return 1;
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
        return 1;
    }
    return 0;
}


// void makeOutputFile(std::vector<std::string>* stepLog){
//     //#TODO
// }

int main(int argc, char *argv[]){ 
    if(argc!=2 || !argv[1]){
        std::cerr << "Error:must pass exactly 1 argument of input file name." << std::endl;
        return 1;
    }
    std::tuple<int,int> dockingStationLoc;
    std::vector<std::vector<int> > houseMap;
    int maxBatterySteps=0, MaxSteps=0;
    int isError = updateDataFromFile(argv[1],dockingStationLoc,houseMap,maxBatterySteps,MaxSteps);
    if(isError)
        return 1;
    House h(dockingStationLoc,houseMap);
    VaccumCleaner vc(maxBatterySteps,dockingStationLoc);
    CleaningAlgorithm alg(maxBatterySteps,MaxSteps);
    std::vector<std::string>* stepLog = vc.cleanHouse(h,alg,MaxSteps);
    //makeOutputFile(stepLog)  ;
    delete stepLog;
    return 0;
}