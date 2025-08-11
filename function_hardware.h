#ifndef FUNCTION_HARDWARE_H
#define FUNCTION_HARDWARE_H

/* BACKEND PID

Function used to run in the background using the scheduler library.
Compute the PIDS

*/
void backend_PID()
{
    if(Motor_Setpoint == 0)
    {
        /*
        The PID library doesnt include a function to force the pid output to 0. The output only change based on the input and tthe output.
        This means when we set output == 0 when Setpoint == 0 (to stop the motor), the ouput comput from the pid dont go to 0.
        Logic : the RPM = 0 and the setpoint = 0 so no need the change the output. We will lure the PID with a constant high RPM so it will go to 0.
        */
        //Motor_Input = 1200; // the make the PID the motor run forever to it will lower again and again the output until it goes to 0.
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
    but can not manage to get the motor to run to 1200 RPM. This take ages.
    This was the only solution I got to work without make the PID unstable. So i fix it with some duck tape I guess */
    
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
    Nextion.writeNum("GLOBAL_VAL.NTC_MOTOR1.val",  avg_temp_ntc_mosfet_motor_1);
    Nextion.writeNum("GLOBAL_VAL.NTC_MOTOR2.val",  avg_temp_ntc_mosfet_motor_2);
//    Nextion.writeNum("GLOBAL_VAL.NTC_MOTOR.val",  temp_ntc_motor);
    Nextion.writeNum("GLOBAL_VAL.NTC_HEATER1.val",  avg_temp_ntc_mosfet_heat_1);
    Nextion.writeNum("GLOBAL_VAL.NTC_HEATER2.val",  avg_temp_ntc_mosfet_heat_1);

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

void RPM_update()
{
  if((millis() - RPM_refresh_timer) > RPM_REFRESH_TIME)
  {
    
    avg_Motor_RPM = Avg_MotorRPM.reading( ( IR_sensor_count / (millis() - RPM_refresh_timer) )* (60000 / ENCODER) );   
    Motor_Input = avg_Motor_RPM;
    IR_sensor_count = 0;
    RPM_refresh_timer = millis();
  }
  
  yield();
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

void debug()
{
  if((millis() - debug_refresh_timer) > DEBUG_REFRESH_TIME)
  {
  //       SerialUSB.println("Input MOTOR = " + String(Motor_Input));
//       SerialUSB.println("Input HEATER = " + String(Heater_Input));
//       SerialUSB.println("Setpoint MOTOR = " + String(Motor_Setpoint));
//       SerialUSB.println("Setpoint HEATER = " + String(Heater_Setpoint));
//       SerialUSB.println("Output MOTOR = " + String(Motor_Output));
//       SerialUSB.println("Output HEATER = " + String(Heater_Output));
    
//       SerialUSB.println("NTC MOTOR 1 = " + String(avg_temp_ntc_mosfet_motor_1));
//       SerialUSB.println("NTC MOTOR 2 = " + String(avg_temp_ntc_mosfet_motor_2));
// //      SerialUSB.println("NTC MOTOR = " + String(temp_ntc_motor));
//       SerialUSB.println("NTC HEAT 1 = " + String(avg_temp_ntc_mosfet_heat_1));
//       SerialUSB.println("NTC HEAT 2 = " + String(avg_temp_ntc_mosfet_heat_2));
//       SerialUSB.println("NTC OIL = " + String(avg_temp_ntc_oil));
      SerialUSB.print("NTC_OIL:");
      SerialUSB.println(String(avg_temp_ntc_oil));
      //SerialUSB.println("IR sensor count = " + String(IR_sensor_count));
      //SerialUSB.println("Motor RPM avg = " + String(MotorRPM_avg));
      debug_refresh_timer = millis();
  }
yield();
}

#endif