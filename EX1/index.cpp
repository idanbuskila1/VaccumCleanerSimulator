#include "index.h"
int chooseNextDirection(House h, tuple<int,int> loc){
    int lb=0,ub=3;
    int ret = (rand() % (ub - lb + 1)) + lb;
    while(h.isWallInDirection(ret,loc))
        ret = (rand() % (ub - lb + 1)) + lb;
    return ret;
}
void cleaningAlgorithm(VaccumCleaner& vc, House& h, int maxSteps){
    int steps=0;
    int dir=-1;
    while(steps<maxSteps){
        cout<<"step: "<<steps<<". battery: "<<vc.getBatterySteps()<<"location: ["<<get<0>(vc.getCurrentLoc())<<","<<get<1>(vc.getCurrentLoc())<<"]. direction: "<<dir<<".";
        if(vc.getBatterySteps() -1 ==vc.getStepsFromDocking()){
            steps+= vc.goCharge(maxSteps-steps);
            dir=-1;
            cout<<"CHARGE."<<endl;
            continue;
        }
        if(h.getDirtLevel(vc.getCurrentLoc())!=DIRT0){
            steps++;
            vc.clean(h);
            cout<<"CLEAN."<<endl;
            continue;
        }
        if(h.getTotalDirtLeft()>0){
            //having charge and current location is clean, but there is more to clean. decide on direction to move forward.
            if(dir ==-1)
                dir = chooseNextDirection(h,vc.getCurrentLoc());
            //move in this direction.
            vc.move(dir);
            steps++;
            cout<<"MOVED IN DIR "<<dir<<"."<<endl;
            // if there is a wall 1 step towards dir in the new location, discard dir. else vc will continue to go in direction dir.
            if(h.isWallInDirection(dir,vc.getCurrentLoc()))
                dir=-1;
        }
        else{
            //having charge and nothing to clean. go back to docking station and finish (without charging).
            cout<<"finished."<<endl;
            steps+=vc.goCharge(maxSteps-steps,true);
        }
    }
}
void updateDataFromFile(string fileName, tuple<int,int> &dockingStationLoc,vector<vector<int> > &houseMap,int& maxBatterySteps,int& MaxSteps){
    dockingStationLoc = make_tuple(1,1);
    
    houseMap = {{-1,-1,-1,-1,-1},
                {-1,0,1,2,-1},
                {-1,3,2,3,-1},
                {-1,8,9,7,-1},
                {-1,-1,-1,-1,-1}};
    maxBatterySteps=34;
    MaxSteps=60;
    
}
/*
void makeOutputFile(VaccumCleaner& vc, House& h){
    //#TODO
}*/

int main(int argc, char *argv[]){
    if(argc!=2 && argv[1]){
        //throw error
    }
    tuple<int,int> dockingStationLoc;
    vector<vector<int> > houseMap;
    int maxBatterySteps=0, MaxSteps=0;
    updateDataFromFile(argv[1],dockingStationLoc,houseMap,maxBatterySteps,MaxSteps);
    House* h = new House(dockingStationLoc,houseMap);
    VaccumCleaner* vc = new VaccumCleaner(maxBatterySteps,dockingStationLoc);
        
    cleaningAlgorithm(*vc,*h,MaxSteps);
    //makeOutputFile(*vc,*h);
    return 0;
}