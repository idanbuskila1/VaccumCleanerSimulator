#include "index.h"

int updateDataFromFile(const string fileName, tuple<int, int> &dockingStationLoc, vector<vector<int>> &houseMap, int &maxBatterySteps, int &maxSteps)
{
        ifstream file(fileName);

    if (file.is_open()) {
        string line;
        if (std::getline(file, line)) { // Read the first line with docking station location and max steps
            istringstream iss(line);
            int row, col;
            int count = 0;
            while (iss >> (count == 0 ? row : (count == 1 ? col : (count == 2 ? maxBatterySteps : maxSteps)))) {
                count++;
            }
            if (count != 4 || row < 0 || col < 0 || maxBatterySteps <= 0 || maxSteps <= 0) {
                cerr << "Error: The first line must contain exactly four positive integers." << endl;
                file.close();
                return 1;
            }

            get<0>(dockingStationLoc) = row;
            get<1>(dockingStationLoc) = col;
        } else {
            cerr << "Error: Empty file." << endl;
            file.close();
            return 1;
        }

        houseMap.clear(); // Clear the existing houseMap

        int row = 1; // Start from row 1 (assuming 0-based indexing)
        vector<int> rowLengths;
        while (getline(file, line)) {
            vector<int> rowData;
            istringstream iss(line);
            int num;
            while (iss >> num) {
                if (num < -1 || num > 9){
                    cerr << "Error: Invalid value "<<num<<" detected at line " << row << "." << endl;
                    file.close();
                    return 1;
                }
                rowData.push_back(num);
            }
            if (!rowData.empty()) {
                houseMap.push_back(rowData);
                rowLengths.push_back(rowData.size());
            } else {
                cerr << "Error: Empty row detected at line " << row << "." << endl;
                file.close();
                return 1;
            }

            row++;
        }
        file.close();
        if (houseMap.empty() || houseMap[0].empty()) {
            cerr << "Error: houseMap is empty or not properly populated" << endl;
            return 1;
        }
        // Check if all rows have the same length
        for (size_t i = 1; i < rowLengths.size(); i++) {
            if (rowLengths[i] != rowLengths[0]) {
                cerr << "Error: Row lengths are not consistent at row " << i + 1 << ", or it has wrong characters in it." << endl;
                return 1;
            }
        }
        int rows = houseMap.size();
        int cols = houseMap[0].size();

        //  check that the docking station location is within the houseMap boundaries (withput first and last row/col beacuse the house is sorrounded by walls) and has a value of 0
        if (get<0>(dockingStationLoc) >= rows-1 || get<1>(dockingStationLoc) >= cols-1 || get<0>(dockingStationLoc)<=0|| get<1>(dockingStationLoc)<=0) {
            cerr << "Error: Docking station location is out of bounds. It must not be outside of the house coordinates, or on the first/last row or column (house is sorrounded by walls)." << endl;
            return 1;
        }
        if (houseMap[get<0>(dockingStationLoc)][get<1>(dockingStationLoc)] != 0) {
            cerr << "Error: Docking station location must have a value of 0 on the house map.(no dirt and no walls)" << endl;
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
        cerr << "Error: Failed to open file " << fileName << "." << endl;
        return 1;
    }
    return 0;
}

int makeOutputFile(const string fileName,const VaccumCleaner &vc, const House &h, const vector<string> &stepLog)
{
    ofstream file("output_"+fileName);
    if (file.is_open()) {
        file << "Total steps: " << stepLog.size() << "." << "\n";
        file << "Total dirt left: " << h.getTotalDirtLeft() << "." << "\n";
        file << "Vacuum Cleaner battery is: " << vc.getBatterySteps() << "." << "\n";
        file << "House's state after cleaning:"<<"\n"<<h.toString();
        tuple<int, int> curLoc = vc.getCurrentLoc();
        tuple<int, int> dockLoc = h.getDockingStationLoc();

        if (h.getTotalDirtLeft() > 0 || curLoc != dockLoc) {
            file << "Clean wasn't successful. There is more dirt left and/or the cleaner is not in the docking station." << endl;
        } else {
            file << "Clean was successful. The house is clean and the Vacuum Cleaner is at the docking station." << endl;
        }

        file << endl;
        file << "Logging of all steps performed by the Vacuum Cleaner:" << endl;
        for (const auto& log : stepLog) {
            file << log << endl;
        }
        file.close();
        return 0;
    } else {
        cerr << "Error: Failed to open file result.txt." << endl;
        return 1;
    }
}

//returns 0 for success, 1 for error.
int main(int argc, char *argv[]){ 
    if(argc!=2 || !argv[1]){
        cerr << "Error:must pass exactly 1 argument of input file name." << endl;
        return 1;
    }
    srand(time(0));//set seed for randomness betwween program runs
    //initialize data from input file
    tuple<int,int> dockingStationLoc;
    vector<vector<int> > houseMap;
    int maxBatterySteps=0, MaxSteps=0;
    int isError = updateDataFromFile(argv[1],dockingStationLoc,houseMap,maxBatterySteps,MaxSteps);
    if(isError)
        return 1;
    //initialzie class objects.
    House h(dockingStationLoc,houseMap);
    VaccumCleaner vc(maxBatterySteps,dockingStationLoc);
    CleaningAlgorithm alg(maxBatterySteps,MaxSteps);
    //clean house h with vaccum cleaner vc according to algorithm alg
    const vector<string> stepLog = vc.cleanHouse(h,alg,MaxSteps);
    //create output dile from the log, free memory  and finish with success.
    isError = makeOutputFile(argv[1],vc, h, stepLog);
    return isError;
}