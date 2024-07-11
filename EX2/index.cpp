#include "index.hpp"
using std::cerr, std::endl;
int main(int argc, char *argv[]){ 
    if(argc!=2 || !argv[1]){
        cerr << "Error:must pass exactly 1 argument of input file name." << endl;
        return 1;
    }
}