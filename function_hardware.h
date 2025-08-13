#ifndef FUNCTION_HARDWARE_H
#define FUNCTION_HARDWARE_H

/* BACKEND PID

Function used to run in the background using the scheduler library.
Compute the PIDS

*/
void backend_PID()
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
      Motor_Output = 0;
    }
    else
    {
      Motor_PID.Compute();
    }
    analogWrite(PIN_MOTOR, (Motor_Output/10)*( (Motor_Output/6000)*(Motor_Output/6000)* 20) ); 
    Motor_PID_refresh_timer = millis();

    //analogWrite(PIN_MOTOR, Motor_Output/10);
    /* this weird line is not my fault i promess. 
    The problem is : the pid can control most of the speed of the motor (0 RPM<output<~ 550 RPM) 
    but can not manage to get the motor to run to 1200 RPM. This take ages.
    This was the only solution I got to work without make the PID unstable. So i fix it with some duck tape I guess */
  }

  if(Heater_Setpoint == 0)
  {
    Heater_PID.Compute();
    Heater_Output = 0;
  }
  else
  {
    Heater_PID.Compute();
  }
  analogWrite(PIN_HEATER, (Heater_Output)); 
  yield();

}

void backend_nextion()
{
  if( (millis() - nextion_response_refresh_timer) > NEXTION_RESPONSE_REFRESH_TIME)
  {
    //send current RPM and oil temp
    Nextion.writeNum("GLOBAL_VAL.CURRENT_RPM.val", avg_Motor_RPM);
    Nextion.writeNum("GLOBAL_VAL.CURRENT_TEMP.val",  avg_temp_ntc_oil);

    //send all temp values from security NTC sensor
//     Nextion.writeNum("GLOBAL_VAL.NTC_MOTOR1.val",  avg_temp_ntc_mosfet_motor_1);
//     Nextion.writeNum("GLOBAL_VAL.NTC_MOTOR2.val",  avg_temp_ntc_mosfet_motor_2);
//     Nextion.writeNum("GLOBAL_VAL.NTC_MOTOR.val",  temp_ntc_motor);
//     Nextion.writeNum("GLOBAL_VAL.NTC_HEATER1.val",  avg_temp_ntc_mosfet_heat_1);
//     Nextion.writeNum("GLOBAL_VAL.NTC_HEATER2.val",  avg_temp_ntc_mosfet_heat_1);

    // //send PID output
    // Nextion.writeNum("GLOBAL_VAL.MOTOR_OUTPUT.val",  Motor_Output);
    // Nextion.writeNum("GLOBAL_VAL.HEATER_OUTPUT.val",  Heater_Output);

    // Nextion.writeNum("GLOBAL_VAL.IR_SENSOR.val",  IR_sensor_count);

    // current_year = Nextion.readNumber("GLOBAL_VAL.ANNEE.val");
    // current_month = Nextion.readNumber("GLOBAL_VAL.MONTH.val");
    // current_day = Nextion.readNumber("GLOBAL_VAL.DAY.val");
    // current_hour = Nextion.readNumber("GLOBAL_VAL.CURRENT_HOUR.val");
    // current_minute = Nextion.readNumber("GLOBAL_VAL.CURRENT_MIN.val");
    // current_second = Nextion.readNumber("GLOBAL_VAL.CURRENT_SEC.val");

    Nextion.NextionListen();
    nextion_response_refresh_timer = millis();
  }
  yield();
}



void ntc_update()
{
  if( (millis() - NTC_refresh_timer) > NTC_REFRESH_TIME)
  {
    avg_temp_ntc_oil = Avg_Temp_ntc_oil.reading(ReadTempNTC(PIN_NTC_OIL));
    avg_temp_ntc_mosfet_motor_1 = Avg_Temp_ntc_mosfet_motor_1.reading(ReadTempNTC(PIN_NTC_MOSFET_MOTOR_1));
    avg_temp_ntc_mosfet_motor_2 = Avg_Temp_ntc_mosfet_motor_2.reading(ReadTempNTC(PIN_NTC_MOSFET_MOTOR_2));
    avg_temp_ntc_mosfet_heat_1 = Avg_Temp_ntc_mosfet_heat_1.reading(ReadTempNTC(PIN_NTC_MOSFET_HEAT_1));
    avg_temp_ntc_mosfet_heat_2 = Avg_Temp_ntc_mosfet_heat_2.reading(ReadTempNTC(PIN_NTC_MOSFET_HEAT_2));
  }
  yield();
}

void RPM_interrupt()
{
  IR_sensor_count++;
}


void HEAT_PID_update() //this refresh the pid heat input
{
  if((millis() - Heater_PID_refresh_timer) > HEATER_PID_REFRESH_TIME)
  {
    Heater_Input = avg_temp_ntc_oil;
    Heater_PID_refresh_timer = millis();
  }
  yield();
}



#endif