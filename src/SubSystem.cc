/**
* Subsystem Process write temeprature periodically into shared memory
* with IPC APIs.
* @version 1.0
*
**/
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <map>
#include <mutex>
#include "SubSystemIPC.h"
using namespace std;
std::mutex subsysNumMutex;
volatile sig_atomic_t subSys_shutdown_flag = 1;

void GracefulSubSysExit(int signal_number)
{
    subSys_shutdown_flag = 0;
}

int main(int argc, char* argv[])
{
    double temperature;
    double prevTemp=0;
    bool value;
    // Register SIGTERM handler
    signal(SIGTERM, GracefulSubSysExit);
    unsigned int tmp = 0;
    string subSysId=argv[1];
    string shmIdTempChange = argv[2];
    SubSystemIPC subSysObj;
    srand(10*stoi(subSysId));
    while (subSys_shutdown_flag) {
        tmp += 1;
        usleep(100+(rand()%100));
	temperature = subSysObj.getSubSysTemperature();
        if(prevTemp != temperature){
	    // Write sub sys temperature memory 
            value = subSysObj.writeSubSysTemperature(stoi(subSysId),stoi(shmIdTempChange),temperature);
            prevTemp = temperature;
         }        
    }
    exit(EXIT_SUCCESS);
}

