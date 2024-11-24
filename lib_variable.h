#ifndef LIB_VARIABLE_H
#define LIB_VARIABLE_H

#include <PID_v1.h>
#include <movingAvg.h>
#include "EasyNextionLibrary.h"
//#include "Nextion.h"

#define INTERRUPTPIN 4 //pin of the optical sensor that is used as an encoder
#define PIN_OUTPUT 12 //pin of the DC motor


const int resolution_encoder = 20; //resolution of the current encoder. 20 slots are used here. 

//Define Variables of the PID
double Setpoint = 0, Input, Output;
unsigned long lastTempUpdate;
const int refLOWrpm = Setpoint - 100;
const int refHIGHrpm = Setpoint + 100;

//double Kp=0.003, Ki=0.03, Kd=0.005;
double Kp=0.001, Ki=0.05, Kd=0.005;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

//variables to count the pulse from the optical sensor 
volatile int rpmcount;


movingAvg inputavg(10);    // use 10 data points for the moving average
movingAvg outputavg(10);
int outputVal;

//partie Nextion

EasyNex myNex(Serial2);   // Create an object of EasyNex class with the name < myNex >
const int REFRESH_TIME = 100;           // time to refresh the Nextion page every 100 ms
unsigned long refresh_timer = millis();  // timer for refreshing Nextion's page


#endif