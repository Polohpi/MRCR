/********************************************************
This code execute a basic run-in bench for nitro engine. The engine is put in motion in a heated oil vat for a few hours. The RM varies from 400 to 900rpm.

This setupe use a nextion screen, a DC motor with a optic encoder to measure RPM, 2 heating cartridges and a DS18B20 to measure the vat temperature. The 
cartridge and DC motor are controlled with some IRF540N mosfets. 

After setup the Acceuil() function is luanched. This countains a loop for the screen reaction, printed varables ect... The same principle will applie for
other pages. 

further info : runinbench.iishp@passmail.net

 ********************************************************/

#include <Arduino.h>
#include "lib_variable.h"
#include "function_hardware.h"
#include "function_nextion.h"
#include "page.h"

void setup()
{
  Serial.begin(9600); //Serial for debugging

  myNex.begin(9600); //Hardware Serial 2 for the nextion UART 
  
  pinMode(PIN_OUTPUT, OUTPUT);
  attachInterrupt(INTERRUPTPIN, rpm_isr, FALLING); 
  
  //ledc replace analog.write. 
  ledcSetup(0, 20000, 8); //channel 0, 20kHz coded on 8bit
  ledcAttachPin(PIN_OUTPUT, 0); //still on channel 0

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255); //limit of the pid output. a PWM signal with a value of 255 is basically ON

  //moving average filter for the PID
  inputavg.begin(); //average of the input value  to smooth the output signal
  outputavg.begin(); //same thing for the output signal

}


void loop() 
{
Acceuil(); //see page.h
}