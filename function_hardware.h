#ifndef FUNCTION_HARDWARE_H
#define FUNCTION_HARDWARE_H


void MOTOR_PID();
void backend_nextion();
void ntc_update();
void RPM_interrupt();
void HEAT_ramp();



//compute the PID for the motor
void MOTOR_PID()
{
  if( (millis() - Motor_PID_refresh_timer) > MOTOR_PID_REFRESH_TIME)
  {

      if(Motor_Setpoint == 0)
      {
        /*
        The PID library doesnt include a function to force the pid output to 0. The output only change based on the input and tthe output.
        This means when we set output == 0 when Setpoint == 0 (to stop the motor), the ouput comput from the pid dont go to 0.
        Logic : the RPM = 0 and the setpoint = 0 so no need the change the output. We will lure the PID with a constant high RPM so it will go to 0.
        */
        Motor_PID.Compute();
      }
      else
      {
        if(Emergency_stop == false)
        {
          Motor_PID.Compute();
        }
      }
      analogWrite(PIN_MOTOR, (Motor_Output/10)*( (Motor_Output/6000)*(Motor_Output/6000)* 20) ); 
      Motor_PID_refresh_timer = millis();

      /* this weird line is not my fault I promess. 
      The problem is : the pid can control most of the speed of the motor (0 RPM<output<~ 550 RPM) 
      but can not manage to get the motor to run to 1200 RPM. This take ages.
      This was the only solution I got to work without make the PID unstable. So i fix it with some duck tape I guess */
    
  } 
  yield();

}

//manage all global data flow between MCU and nextion
void backend_nextion()
{
  if( (millis() - nextion_response_refresh_timer) > NEXTION_RESPONSE_REFRESH_TIME)
  {
    //send current RPM and oil temp
    Nextion.writeNum("GLB_VAL.CUR_RPM.val", avg_Motor_RPM);
    Nextion.writeNum("GLB_VAL.CUR_HEAT.val",  avg_temp_ntc_oil);

    //send all temp values from security NTC sensor
    Nextion.writeNum("GLB_VAL.NTC_MTR1.val",  avg_temp_ntc_mosfet_motor_1);
    Nextion.writeNum("GLB_VAL.NTC_MTR2.val",  avg_temp_ntc_mosfet_motor_2);
    Nextion.writeNum("GLB_VAL.NTC_MTR.val",  avg_temp_ntc_motor);
    Nextion.writeNum("GLB_VAL.NTC_HTR1.val",  avg_temp_ntc_mosfet_heat_1);
    Nextion.writeNum("GLB_VAL.NTC_HTR2.val",  avg_temp_ntc_mosfet_heat_1);

    //send PID output
    Nextion.writeNum("GLB_VAL.MTR_OTP.val",  Motor_Output);
    Nextion.writeNum("GLB_VAL.IR_SNSR.val",  IR_sensor_count);
    Nextion.writeStr("GLB_VAL.HEAT_STE.txt",  digitalRead(PIN_HEATER) ? "ON" : "OFF");


    current_year = Nextion.readNumber("GLB_VAL.CUR_YEAR.val");
    current_month = Nextion.readNumber("GLB_VAL.CUR_MONTH.val");
    current_day = Nextion.readNumber("GLB_VAL.CUR_DAY.val");
    current_hour = Nextion.readNumber("GLB_VAL.CUR_HOUR.val");
    current_minute = Nextion.readNumber("GLB_VAL.CUR_MIN.val");
    current_second = Nextion.readNumber("GLB_VAL.CUR_SEC.val");

    Nextion.NextionListen();
    nextion_response_refresh_timer = millis();
  }
  yield();
}


//update all ntc temperature
void ntc_update()
{
  if( (millis() - NTC_refresh_timer) > NTC_REFRESH_TIME)
  {
    avg_temp_ntc_oil = Avg_Temp_ntc_oil.reading(ReadTempNTC(PIN_NTC_OIL));
    avg_temp_ntc_motor = Avg_Temp_ntc_motor.reading(ReadTempNTC(PIN_NTC_MOTOR));
    avg_temp_ntc_mosfet_motor_1 = Avg_Temp_ntc_mosfet_motor_1.reading(ReadTempNTC(PIN_NTC_MOSFET_MOTOR_1));
    avg_temp_ntc_mosfet_motor_2 = Avg_Temp_ntc_mosfet_motor_2.reading(ReadTempNTC(PIN_NTC_MOSFET_MOTOR_2));
    avg_temp_ntc_mosfet_heat_1 = Avg_Temp_ntc_mosfet_heat_1.reading(ReadTempNTC(PIN_NTC_MOSFET_HEAT_1));
    avg_temp_ntc_mosfet_heat_2 = Avg_Temp_ntc_mosfet_heat_2.reading(ReadTempNTC(PIN_NTC_MOSFET_HEAT_2));
  }
  yield();
}

//interrupt to add one touth from the dc motor target
void RPM_interrupt()
{
  IR_sensor_count++;
}

//Heat ramp ON OFF type
void HEAT_ramp()
{
  if(Emergency_stop == false)
  {
    static unsigned long heaterPulseTimer = 0;
    static bool heaterPulseState = false;

    if ((millis() - Heater_ramp_refresh_timer) > HEATER_RAMP_REFRESH_TIME)
    {
      Heater_ramp_refresh_timer = millis();

      // Décision de commande avec hystérésis
      bool wantOn  = (avg_temp_ntc_oil <= (Setpoint_HEATER - HYST_HEATER));
      bool wantOff = (avg_temp_ntc_oil >= (Setpoint_HEATER + HYST_HEATER));

      if (heat && wantOff)
      {
        heat = false;
      }
      else if (!heat && wantOn)
      {
        heat = true;
        heaterPulseTimer = millis();     // réinitialise le cycle à l'activation
        heaterPulseState = true;         // commence par chauffer
      }

      if (heat)
      {
        unsigned long elapsed = millis() - heaterPulseTimer;
        if (heaterPulseState && elapsed >= HEATER_ON_TIME)
        {
          heaterPulseState = false;
          heaterPulseTimer = millis();
        }
        else if (!heaterPulseState && elapsed >= HEATER_OFF_TIME)
        {
          heaterPulseState = true;
          heaterPulseTimer = millis();
        }

        digitalWrite(PIN_HEATER, heaterPulseState ? HIGH : LOW);
      }
      else
      {
        digitalWrite(PIN_HEATER, LOW);
      }
    }
    //yield();
  }
  yield();
}




#endif