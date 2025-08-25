/**
* FanController Header file for some defintions and macros to read/write
* fan hw registers.
* 
* @version 1.0
*
**/


#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using namespace std;

constexpr int MAXFANS = 6;
constexpr int LO_TEMP_CELCIUS=25;
constexpr  int HI_TEMP_CELCIUS= 75;
// Start address of FAN registers
volatile unsigned int FANPWM[MAXFANS];
#define FAN_PWM_ADDR       &FANPWM[0]
#define READ_FAN_PWM(num)     (*(volatile unsigned int *)(FAN_PWM_ADDR+num*sizeof(unsigned int)))
#define WRITE_FAN_PWM(num, val) ((*(volatile uint32_t *)(FAN_PWM_ADDR+num*sizeof(unsigned int))) = (val))
