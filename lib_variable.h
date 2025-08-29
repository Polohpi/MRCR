#ifndef LIB_VARIABLE_H
#define LIB_VARIABLE_H

//check README.md for more infos
#include <Scheduler.h>
#include "EasyNextionLibrary.h"
#include <PID_v1.h>
#include <movingAvg.h>



//defone hardware analog pins
#define PIN_NTC_MOSFET_MOTOR_1 A0
#define PIN_NTC_MOSFET_MOTOR_2 A1
#define PIN_NTC_MOSFET_HEAT_1 A2
#define PIN_NTC_MOSFET_HEAT_2 A3
#define PIN_NTC_OIL A6
//#define PIN_NTC_MOTOR A7

//define hardware pins
#define PIN_MOTOR 10 
#define PIN_HEATER 11 
#define PIN_IR_SENSOR 3
//#define PIN_BUZER 4

#define ENCODER 40 //number of teeth on the motor dc target

//set variables for each page. When called ChangePageMenu = 1, the screen will change to the main page
#define MAIN_PAGE 1 
#define MANUAL_MODE_PAGE 2

//define variable to get the nextion RTC values (hour minutes etc)
int current_year;
int current_month;
int current_day;
int current_hour;
int current_minute;
int current_second;

//deine various timer
const int MOTOR_PID_REFRESH_TIME = 50;           // time to refresh the motor PID every 50 ms
unsigned long Motor_PID_refresh_timer =  millis();  // timer for refreshing motor PID

const int HEATER_RAMP_REFRESH_TIME = 2000;           // time to refresh the heater PID every  5000ms
unsigned long Heater_ramp_refresh_timer = millis();  // timer for refreshing heater PID

const int NEXTION_REFRESH_TIME = 1000;           // time to refresh the Nextion data every 100 ms
unsigned long nextion_refresh_timer = millis();  // timer for refreshing Nextion's page

const int RPM_REFRESH_TIME = 50;           // time to refresh the RPM counter every 50 ms. This time period is used to count le top signal from the encoder (x signal in 100 ms)
unsigned long RPM_refresh_timer = millis();  // timer for refreshing RPM counter.

const int NEXTION_RESPONSE_REFRESH_TIME = 500;           // time to refresh the RPM counter every 50 ms. This time period is used to count le top signal from the encoder (x signal in 100 ms)
unsigned long nextion_response_refresh_timer = millis();  // timer for refreshing RPM counter.

const int NTC_REFRESH_TIME = 1000;           // time to refresh the RPM counter every 50 ms. This time period is used to count le top signal from the encoder (x signal in 100 ms)
unsigned long NTC_refresh_timer = millis();  // timer for refreshing RPM counter.

const int DEBUG_REFRESH_TIME = 1000;           // time to refresh the RPM counter every 50 ms. This time period is used to count le top signal from the encoder (x signal in 100 ms)
unsigned long debug_refresh_timer = millis();  // timer for refreshing RPM counter.

//Define PID Variables 
double Motor_Setpoint=0, Motor_Input, Motor_Output;

//Define PID values
double Motor_Kp=0.3, Motor_Ki=0.3, Motor_Kd=0.1;

//create PIDs objects
PID Motor_PID(&Motor_Input, &Motor_Output, &Motor_Setpoint, Motor_Kp, Motor_Ki, Motor_Kd, DIRECT);

// Set Nextion Serial
EasyNex Nextion(Serial1);
int ChangePageMenu = 0;

//define moving avg. Ths avoid noise for the PIDs
movingAvg Avg_Temp_ntc_oil(10); // sample oil temp sensor to avoid noise
movingAvg Avg_MotorRPM(5); // sample RPM count to avoid noise 
movingAvg Avg_Temp_ntc_mosfet_motor_1(10);
movingAvg Avg_Temp_ntc_mosfet_motor_2(10);
movingAvg Avg_Temp_ntc_mosfet_heat_1(10);
movingAvg Avg_Temp_ntc_mosfet_heat_2(10);

//other variable
float IR_sensor_count = 0; //value to count the signal from the encoder
int  avg_Motor_RPM = 0; // value that will contain average data
float avg_temp_ntc_oil = 0; //same here
float avg_temp_ntc_motor = 0; //same here
float avg_temp_ntc_mosfet_motor_1 =0;
float avg_temp_ntc_mosfet_motor_2 =0;
float avg_temp_ntc_mosfet_heat_1 = 0;
float avg_temp_ntc_mosfet_heat_2 = 0;
int ManualMode_SetRPM[] = {0, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950};
int ManualMode_SetHeat[] = {0, 30, 40, 50, 60, 70, 80, 90, 100, 110};


double Setpoint_HEATER = 0.0;                       // consigne en °C
const double HYST_HEATER = 1.0;                // ±1 °C autour de SP
bool heat = false;                      // état chauffage (sortie)

#endif