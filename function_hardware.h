#ifndef FUNCTION_HARDWARE_H
#define FUNCTION_HARDWARE_H


void backend_PID()
{
    if(Motor_Setpoint == 0)
    {
      Motor_PID.Compute();
      Motor_Output = 0;
    }
    else
    {
      Motor_PID.Compute();
    }
    analogWrite(PIN_MOTOR, (Motor_Output/10)*( (Motor_Output/6000)*(Motor_Output/6000)* 20) ); 
    /* this weird line is not my fault i promess. 
    The problem is : the pid can control most of the speed of the motor (0 RPM<output<~ 550 RPM) 
    but can not manage to get the motor to run to 1200 RPM. 
    This take ages. This was the only solution I got to work without make the PID unstable. So i fix it with some duck tape I guess */
    Motor_PID_refresh_timer = millis();

    if(Heater_Setpoint == 0)
    {
      Heater_PID.Compute();
      Heater_Output = 0;
    }
    else
    {
      Heater_PID.Compute();
    }
  yield();
  
}

void backend_nextion()
{
  if( (millis() - nextion_response_refresh_timer) > NEXTION_RESPONSE_REFRESH_TIME)
  {
    Nextion.writeNum("GLOBAL_VAL.CURRENT_RPM.val", MotorRPM_avg);
    Nextion.writeNum("GLOBAL_VAL.CURRENT_TEMP.val",  temp_ntc_oil);
    Nextion.writeNum("GLOBAL_VAL.NTC_MOTOR1.val",  temp_ntc_motor_1);
    Nextion.writeNum("GLOBAL_VAL.NTC_MOTOR2.val",  temp_ntc_motor_2);
    Nextion.writeNum("GLOBAL_VAL.NTC_HEATER1.val",  temp_ntc_heat_1);
    Nextion.writeNum("GLOBAL_VAL.NTC_HEATER2.val",  temp_ntc_heat_2);
    Nextion.NextionListen();
    nextion_response_refresh_timer = millis();
  }
  yield();
}

float ReadTempNTC(int pin)
{
  int Ro = 10, B =  3435; //Nominal resistance 10K, Beta constant
  int Rseries = 10;// Series resistor 10K
  float To = 298.15; // Nominal Temperature
  const float V = 3.3; //voltage

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


void ntc_update()
{
  float avg_temp_ntc_motor_1 = 0;
  float avg_temp_ntc_motor_2 = 0;
  float avg_temp_ntc_heat_1 = 0;
  float avg_temp_ntc_heat_2 = 0;
  float avg_temp_ntc_oil = 0;

  for(int i = 0; i<10; i++)
  {
    avg_temp_ntc_motor_1 = avg_temp_ntc_motor_1 + ReadTempNTC(PIN_NTC_MOTOR_1);
    avg_temp_ntc_motor_2 = avg_temp_ntc_motor_2 + ReadTempNTC(PIN_NTC_MOTOR_2);
    avg_temp_ntc_heat_1 = avg_temp_ntc_heat_1 + ReadTempNTC(PIN_NTC_HEAT_1);
    avg_temp_ntc_heat_2 = avg_temp_ntc_heat_2 + ReadTempNTC(PIN_NTC_HEAT_2);
    avg_temp_ntc_oil = avg_temp_ntc_oil + ReadTempNTC(PIN_NTC_OIL);
    yield();
  }

  temp_ntc_motor_1 = avg_temp_ntc_motor_1 /10;
  temp_ntc_motor_2 = avg_temp_ntc_motor_2 /10;
  temp_ntc_heat_1 = avg_temp_ntc_heat_1 /10;
  temp_ntc_heat_2 = avg_temp_ntc_heat_2 /10;
  temp_ntc_oil = avg_temp_ntc_oil / 10;
  yield();
}

void RPM_interrupt()
{
  IR_sensor_count++;
}

void RPM_counter()
{
  if((millis() - RPM_refresh_timer) > RPM_REFRESH_TIME)
  {
    
    MotorRPM_avg = Avg_MotorRPM.reading( ( IR_sensor_count / (millis() - RPM_refresh_timer) )* (60000 / ENCODER) );
    //if(MotorRPM_avg > 2500)
    //{
      //MotorRPM_avg =0;
    //}
    
    Motor_Input = MotorRPM_avg;
    IR_sensor_count = 0;
    RPM_refresh_timer = millis();
  }
  
  yield();
}

#endif