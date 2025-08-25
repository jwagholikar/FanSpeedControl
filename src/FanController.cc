/**
* FanController Process controls fan durty cycle by
* setting hw registers with PWM count based on susbsystem temperature. 
* @version 1.0
*
**/

#include "SubSystemIPC.h"
#include "FanController.h"

volatile sig_atomic_t shutdown_flag = 1;

void GracefulExit(int signal_number)
{
    shutdown_flag = 0;
}
  
int main(int arc, char* argv[])
{
    vector<unsigned int> PWMCount;
    SubSystemIPC subSystemObj;
    double maxTemp=0, prevMaxTemp=0;;
    unsigned int number, newPWMCount;
     // Register SIGTERM handler
    signal(SIGTERM, GracefulExit);
    unsigned int tmp = 0;

    int subSys =stoi(argv[1]);

    int fanCount =stoi(argv[2]);
    
    string shmIdTempChangeStr =argv[3];
    int shmIdTempChange = stoi(shmIdTempChangeStr);

    string fanPwmCount = argv[4];
    stringstream fPwm(fanPwmCount);
  
    while(fPwm >> number){      
        PWMCount.push_back(number);
        if (fPwm.peek() == ',')
          fPwm.ignore();
     }
    while (shutdown_flag) {
        tmp += 1;
        maxTemp =0;
        usleep(100+rand()%50);
        for(int i=0; i<subSys; i++) {
            // read subsystem temperature 
            double temperature = subSystemObj.readSubSysTemperature(i, shmIdTempChange);
            cout<< "SubSystem id: " << i << " RD Temperature: "<<temperature<<endl;
            if(maxTemp < temperature) {
                 maxTemp = temperature;
            } 
        }
       if(maxTemp) {
        cout<<"MaxTemp: "<<maxTemp << " PrevMaxTemp: "<< prevMaxTemp <<endl;
        for(int i=0; i<fanCount; i++) {
             if(maxTemp <= LO_TEMP_CELCIUS) {
                 // calculate 20% of PWM count for 100%duty cycle.
                newPWMCount = 0.2*PWMCount[i];
            } else if( (LO_TEMP_CELCIUS < maxTemp) && (maxTemp <= HI_TEMP_CELCIUS)){
           // Interpolation for 25C<maxTemp<75C Interpolation PWM count 20%duty cycle and PWM count for 75% duty cycle.  
             newPWMCount = (0.2*PWMCount[i])+(((PWMCount[i] -(0.2*PWMCount[i]))/(HI_TEMP_CELCIUS-LO_TEMP_CELCIUS))*(maxTemp - LO_TEMP_CELCIUS));
           } else{ //PWM count for 110% Duty Cycle. 
              newPWMCount = PWMCount[i];
          }
         cout << "For Fan: "<< i <<" newPWMCount: "<< newPWMCount<<endl;
         WRITE_FAN_PWM(i, newPWMCount);
         prevMaxTemp = maxTemp;
      }
   }
 } 
       
    // destroy the shared memory
    subSystemObj.destroyIPC(shmIdTempChange);
     
    exit(EXIT_SUCCESS);
}

