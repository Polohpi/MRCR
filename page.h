#ifndef PAGE_H
#define PAGE_H

#include <Arduino.h>
#include "lib_variable.h"
#include "function_hardware.h"
#include "function_nextion.h"

/*
Acceuil is for now the only page. This print a graph of the actual RPM and the setpoint used. You can select the setpoint by using + and - on the sceen and then press "SET" 
*/
void Acceuil()
{
    Serial.println("Page Acceuil"); 

    while(1) //stay in this loop until a change of page is asked
    {

    updateRPM();
    myPID.Compute();
    outputVal = outputavg.reading(Output);

    Serial.println(">Input:" +String(Input));
    Serial.println(">SetPoint:" + String(Setpoint));
    Serial.println(">Output:" + String(outputVal));

    ledcWrite(0, outputVal); //Set the right setpoint to keep the RPM steady and on target. 0 is the channel number. Not relevent for us. See ledc function for more info
    myNex.NextionListen(); // WARNING: This function must be called repeatedly to response touch events


    if((millis()-refresh_timer) > REFRESH_TIME){ //IMPORTANT do not have serial print commands in the loop without a delay
               
      myNex.writeNum("RPM.val", Input); // RPM.val is a variable within the nextion screen. The screen then compute this variable and update the graph  
      myNex.writeNum("n0.val", Input);   // n0.val is a number field on screen to print the current RPM of the DC motor
      refresh_timer = millis();  // Set the timer equal to millis, create a time stamp to start over the "delay"
     
    } 
    }
}
#endif