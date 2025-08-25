/**
* Subsystem helper IPC functions to read/write to shared memory
* 
* @version 1.0
*
**/

#include "SubSystemIPC.h"
std::mutex mtx;

//Constructor
SubSystemIPC::SubSystemIPC(){}

// Destructor
SubSystemIPC::~SubSystemIPC(){}


int SubSystemIPC::createSubSysIPC(size_t size){
    key_t key = IPC_PRIVATE;
    // shmget returns an identifier in shmid
    int subSysShmid= shmget(key,size,0666|IPC_CREAT);
    return subSysShmid; 
}

double SubSystemIPC::getSubSysTemperature(){
    // Generate random double number between 0 to 100
    double temp = LoTemp + static_cast <double> (rand()) /( static_cast <double> (RAND_MAX/(HiTemp-LoTemp)));
    return temp;
}

bool SubSystemIPC::writeSubSysTemperature(int subSysId, int shmId, double data){
    mtx.lock();
    double *temp = (double*) shmat(shmId,(void*)0,0);
    temp[subSysId]=data;
    //detach from shared memory 
    shmdt(temp);
    mtx.unlock();
    return true;

}


double SubSystemIPC::readSubSysTemperature(int subSysId, int shmId){
    double temperature;
    mtx.lock();
    double *temp= (double*) shmat(shmId,(void*)0,0);
    temperature = temp[subSysId];
    //detach from shared memory 
    shmdt(temp);
    mtx.unlock();
    return temperature;
}


void SubSystemIPC::destroyIPC(int shmId){ 
    // destroy the shared memory
    shmctl(shmId,IPC_RMID,NULL);
}


