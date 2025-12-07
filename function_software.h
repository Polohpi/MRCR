#ifndef FUCNTION_SOFTWARE_H
#define FUNCTION_SOFTWARE_H

//declaration of protoype
float ReadTempNTC(int pin);
void RPM_update();
void debug();
void New_Manual_Timer(int min);
void Manual_Timer();

//compute ntc voltage in °C
float ReadTempNTC(int pin)
{
  int Ro = 10, B =  3435; //Nominal resistance 10K, Beta constant
  int Rseries = 10;// Series resistor 10K
  float To = 298.15; // Nominal Temperature
  const float V = 5; //voltage

  /*Read analog outputof NTC module,
   i.e the voltage across the thermistor */
  float Vi = V - analogRead(pin) * (V / 1023.0);
  //Convert voltage measured to resistance value
  //All Resistance are in kilo ohms.
  float R = (Vi * Rseries) / (V - Vi);
  /*Use R value in steinhart and hart equation
    Calculate temperature value in kelvin*/
  float T =  1 / ((1 / To) + ((log(R / Ro)) / B));
  float Tc = T - 273.15; // Converting kelvin to celsius
  return Tc;
  yield();

}

//compute the rpm
void RPM_update()
{
  if((millis() - RPM_refresh_timer) > RPM_REFRESH_TIME )
  {
    avg_Motor_RPM = Avg_MotorRPM.reading( ( IR_sensor_count / (millis() - RPM_refresh_timer) )* (60000 / ENCODER) );
    //avg_Motor_RPM = ( IR_sensor_count / (millis() - RPM_refresh_timer) )* (60000 / ENCODER);

    Motor_Input = avg_Motor_RPM;
    IR_sensor_count = 0;
    RPM_refresh_timer = millis();
  }
  yield();
}

//debug in serial
void debug()
{
  if((millis() - debug_refresh_timer) > DEBUG_REFRESH_TIME)
  {

      Serial.println("InPut Motor = " + String(Motor_Input));
      Serial.println("Setpoint Motor = " + String(Motor_Setpoint));
      Serial.println("Setpoint Heater = " + String(Setpoint_HEATER));
      Serial.println("Output Motor = " + String(Motor_Output));
      Serial.println(String("Heat active = ") + (digitalRead(PIN_HEATER) ? "ON" : "OFF"));
      
      Serial.println("NTC Motor 1= " + String(avg_temp_ntc_mosfet_motor_1));
      Serial.println("NTC Motor 2= " + String(avg_temp_ntc_mosfet_motor_2));
      Serial.println("NTC MOTOR = " + String(avg_temp_ntc_motor));
      Serial.println("NTC Heat 1= " + String(avg_temp_ntc_mosfet_heat_1));
      Serial.println("NTC Heat 2= " + String(avg_temp_ntc_mosfet_heat_2));
      Serial.println("NTC OIL= " + String(avg_temp_ntc_oil));
      Serial.println("IR cnt= " + String(IR_sensor_count));
      Serial.println("Motor RPM avg= " + String(avg_Motor_RPM));
      Serial.println("Timeleft = " + String(Timeleft));
      Serial.println("current hour = " + String(current_hour));
      Serial.println("current minute = " + String(current_minute));
      debug_refresh_timer = millis();
  }
  yield();
}

//compute the hour to stop the manual timer
void New_Manual_Timer(int min)
{
    int total = current_hour * 60 + current_minute + min;

    // normalisation 24h
    total = total % (24 * 60);

    timerOFF_hour = total / 60;
    timerOFF_minute = total % 60;
    timerOFF_second = current_second;
}

//it's THE MANUAL COUNTDOWWNNNNNNNNN DAAAA TADA DA DA DA TAAADADA DA DA DA DA DA DA DA DA
void Manual_Timer()
{
  if(MANUALTIMER_STATE == true)
  {
    if((millis() - manualtimer_refresh_timer) > MANUALTIMER_REFRESH_TIME)
    {
      Timeleft = ((timerOFF_hour * 60 + timerOFF_minute) - (current_hour * 60 + current_minute) + 1440) % 1440;
  
      if(current_hour == timerOFF_hour && current_minute == timerOFF_minute && current_second == timerOFF_second)
      {
        Motor_Setpoint = 0;
        Setpoint_HEATER = 0;
        Timeleft = 0;
        MANUALTIMER_STATE = false;
      }

      manualtimer_refresh_timer = millis();
    }
  }

  yield();
}


#endif