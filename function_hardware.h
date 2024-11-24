#ifndef FUNCTION_HARDWARE_H
#define FUNCTION_HARDWARE_H


void IRAM_ATTR rpm_isr() // ISR funcion to count the number of pulse from the optical sensor
{
      rpmcount++;
}

void updateRPM() 
{
    if(rpmcount>=resolution_encoder) 
    {
      Input = inputavg.reading((60*rpmcount/40) / ((millis()-lastTempUpdate) *0.001)); //rpm en tr/min
      //Input = (60*rpmcount/40) / ((millis()-lastTempUpdate) *0.001); //rpm en tr/min

      lastTempUpdate = millis();
      rpmcount = 0;
    }
}

#endif