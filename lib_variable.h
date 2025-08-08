#ifndef LIB_VARIABLE_H
#define LIB_VARIABLE_H

#include <Scheduler.h>
#include "EasyNextionLibrary.h"
#include <PID_v1.h>
#include <movingAvg.h>

#define PIN_NTC_MOTOR_1 A0
#define PIN_NTC_MOTOR_2 A1
#define PIN_NTC_HEAT_1 A2
#define PIN_NTC_HEAT_2 A3
#define PIN_NTC_OIL A4

#define PIN_MOTOR 8
#define PIN_HEATER 9 

#define PIN_IR_SENSOR 3

#define ENCODER 40

float temp_ntc_motor_1 = 0;
float temp_ntc_motor_2 = 0;
float temp_ntc_heat_1 = 0;
float temp_ntc_heat_2 = 0;
float temp_ntc_oil = 0;

const int MOTOR_PID_REFRESH_TIME = 50;           // time to refresh the motor PID every 50 ms
unsigned long Motor_PID_refresh_timer =  millis();  // timer for refreshing motor PID

const int HEATER_PID_REFRESH_TIME = 5000;           // time to refresh the heater PID every  5000ms
unsigned long Heater_PID_refresh_timer = millis();  // timer for refreshing heater PID

const int NEXTION_REFRESH_TIME = 1000;           // time to refresh the Nextion data every 1000 ms
unsigned long nextion_refresh_timer = millis();  // timer for refreshing Nextion's page

const int RPM_REFRESH_TIME = 50;           // time to refresh the RPM counter every 50 ms. This time period is used to count le top signal from the encoder (x signal in 100 ms)
unsigned long RPM_refresh_timer = millis();  // timer for refreshing RPM counter.

const int NEXTION_RESPONSE_REFRESH_TIME = 50;           // time to refresh the RPM counter every 100 ms. This time period is used to count le top signal from the encoder (x signal in 100 ms)
unsigned long nextion_response_refresh_timer = millis();  // timer for refreshing RPM counter.

//Define PID Variables 
double Motor_Setpoint=0, Motor_Input, Motor_Output;
double Heater_Setpoint=0, Heater_Input, Heater_Output;

double Motor_Kp=1, Motor_Ki=0.3, Motor_Kd=0.1;
//double Motor_Kp=0.01, Motor_Ki=0.05, Motor_Kd=0.01;
double Heater_Kp=0, Heater_Ki=0, Heater_Kd=0;

//create PIDs objects
PID Motor_PID(&Motor_Input, &Motor_Output, &Motor_Setpoint, Motor_Kp, Motor_Ki, Motor_Kd, DIRECT);
PID Heater_PID(&Heater_Input, &Heater_Output, &Heater_Setpoint, Heater_Kp, Heater_Ki, Heater_Kd, DIRECT);

// Set Nextion Serial
EasyNex Nextion(Serial1);


movingAvg Avg_Oil_Temp(10); // sample oil temp sensor to avoid noise
movingAvg Avg_MotorRPM(5); // sample RPM count to avoid noise 

float IR_sensor_count = 0; //vaue to count the signal from the encoder
uint16_t  MotorRPM_avg = 0; // value that will contain average data
int Oil_Temp_avg = 0; //same here

#endif