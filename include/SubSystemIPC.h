/**
* SubSystemIPC class declaration. 
* 
* @author  Jyoti Wagholikar
* @version 1.0
*
**/


#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <atomic>
#include <filesystem>
#include <vector>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <mutex>

using namespace std;
const double LoTemp=0;
const double HiTemp=100;

using std::cout; using std::endl;
using std::vector; using std::string;
using std::filesystem::exists;

class SubSystemIPC {
        public:
                SubSystemIPC();
                ~SubSystemIPC();
                int createSubSysIPC(size_t size);
                double getSubSysTemperature();
                bool writeSubSysTemperature(int subSysId, int shmId, double data);
                double readSubSysTemperature(int subSysId, int shmId);
                void destroyIPC(int shmId);
                

};

