#ifndef FUNCTION_HARDWARE_H
#define FUNCTION_HARDWARE_H


void backend_PID()
{
  if((millis() - Motor_PID_refresh_timer) > MOTOR_PID_REFRESH_TIME)
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
    analogWrite(PIN_MOTOR, Motor_Output);
    //analogWrite(PIN_MOTOR, Motor_Output * (0.902f * exp(0.00412f * Motor_Output)));
    //analogWrite(PIN_MOTOR, Motor_Output * (1.0f + (m_high - 1.0f) / (1.0f + exp(-k * (Motor_Output - 300.0f)))) );
    Motor_PID_refresh_timer = millis();
  }

  if((millis() - Heater_PID_refresh_timer) > HEATER_PID_REFRESH_TIME)
  {
    if (Heater_Setpoint == 0)
    {
      Heater_PID.Compute();  
      Heater_Output = 0;
    }
    else
    {
      Heater_PID.Compute();
    }
    analogWrite(PIN_HEATER, Heater_Output);
    Motor_PID_refresh_timer = millis();    
  }
  yield();
  
}

void backend_nextion()
{
  if( (millis() - nextion_response_refresh_timer) > NEXTION_RESPONSE_REFRESH_TIME)
  {
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

  /*Read analog outputof NTC module,
   i.e the voltage across the thermistor */
  float Vi = 5.0 - analogRead(pin) * (5.0 / 1023.0);
  //Convert voltage measured to resistance value
  //All Resistance are in kilo ohms.
  float R = (Vi * Rseries) / (5 - Vi);
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
  //SerialUSB.println("RPM interrupt");
  IR_sensor_count++;
  //yield();
}

void RPM_counter()
{
  if((millis() - RPM_refresh_timer) > RPM_REFRESH_TIME)
  {
    
    MotorRPM_avg = Avg_MotorRPM.reading( ( IR_sensor_count / (millis() - RPM_refresh_timer) )* (60000 / ENCODER) );
    //MotorRPM_avg = ( IR_sensor_count / (millis() - RPM_refresh_timer) )* (60000 / ENCODER);
    Motor_Input = MotorRPM_avg;
    IR_sensor_count = 0;
    RPM_refresh_timer = millis();
  }
  
  yield();
}

#endif