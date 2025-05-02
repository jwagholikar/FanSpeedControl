/**
* Top Process which read input parameters validates and forks fancontroller
* and subsystem processes. 
* 
* @author  Jyoti Wagholikar
* @version 1.0
*
**/


#include <iostream>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <atomic>
#include <filesystem>
#include <string>
#include <fstream>
#include "SubSystemIPC.h"
#include "FanController.h"

using namespace std;
constexpr int MAXSUBSYS = 10;

pid_t spawnChild(const char* program, char** arg_list)
{
    pid_t ch_pid = fork();
    if (ch_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (ch_pid > 0) {
        cout << "spawn child with pid - " << ch_pid << endl;
        return ch_pid;
    } else {
        execve(program, arg_list, nullptr);
        perror("execve");
        exit(EXIT_FAILURE);
    }
}


int main(){
     int numSubSys;
     int numFans;
     SubSystemIPC subSysObj;
     int shmTempChangeId;
     vector <unsigned int> fanPWM;
     unsigned int pwmCount;
     fstream newTest;

     // Read Only Test File  
     newTest.open("Test.txt",ios::in);

     if (newTest.is_open()){
      string tp;
      newTest >> numSubSys;  
      newTest >> numFans;
      while (newTest >> pwmCount){
        fanPWM.push_back(pwmCount);
      }
      newTest.close();
    }
    // Input Sanity Check. 
    if(numSubSys == 0 || numSubSys > MAXSUBSYS){
        cout<<"SubSystem Numbers: " << numSubSys << "out of range"<<" 1 to  "<<MAXSUBSYS;
        return -1;
     }
    if(numFans == 0 || numFans > MAXFANS){
        cout<<"Fan Numbers: " << numFans << "out of range"<<" 1 to  "<<MAXFANS;
        return -1;
     }
    if(numFans != fanPWM.size()){
        cout<<"The number of fans " << numFans <<"and PWM counts " << fanPWM.size() <<" do not match"<<endl;
       return -1;
    }

    //Create shared memory to write sub system temperatures.
    shmTempChangeId = subSysObj.createSubSysIPC(numSubSys*sizeof(double));

    
    //Send ShmId for to fan and subsystem processes.     
    string shmIdForTempChange(to_string(shmTempChangeId));

    string totalSubSys(to_string(numSubSys));
    string totalFans(to_string(numFans));
    std::stringstream ss;
    for(size_t i = 0; i < fanPWM.size(); ++i){
        if(i != 0) ss << ",";
        ss << fanPWM[i];
    }
    string fanPwmStr = ss.str();

    // Fork number of subsystem processes.
    string subsys_program_name("SubSystem");
    vector<int> children;
    children.reserve(numSubSys);

    if (!std::filesystem::exists(subsys_program_name)){
        cout << "Program file 'SubSystem' does not exist in current directory!\n";
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numSubSys; ++i){
        string subSysId(to_string(i));
        char *arg_list[] = {subsys_program_name.data(), subSysId.data(), shmIdForTempChange.data(), nullptr};
        children[i] = spawnChild(subsys_program_name.c_str(), arg_list);
        cout << endl;
    }

    // Fork FanController Process with input params
    string program_name("FanController");
    char *arg_list[] = {program_name.data(), totalSubSys.data(), totalFans.data(),shmIdForTempChange.data(), fanPwmStr.data(), nullptr};
    pid_t fanController;
    fanController =  spawnChild(program_name.c_str(), arg_list);

    if (!std::filesystem::exists(program_name)){
        cout << "Program file 'FanController' does not exist in current directory!\n";
        exit(EXIT_FAILURE);
    }

    pid_t child_pid;
    while ((child_pid = wait(nullptr)) > 0)
        cout << "child " << child_pid << " terminated" << endl;      
}
