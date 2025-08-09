#ifndef PAGE_H
#define PAGE_H

void Acceuil()
{
  ChangePageMenu = 0;
  Nextion.writeStr("page 2");
  SerialUSB.println("Acceuil");
  while(1)
  {
    if(ChangePageMenu == 1)
    {
      loop();
    }
    if( (millis() - nextion_refresh_timer) > NEXTION_REFRESH_TIME)
    {

      SerialUSB.println("Input MOTOR = " + String(Motor_Input));
      SerialUSB.println("Input HEATER = " + String(Heater_Input));
      SerialUSB.println("Setpoint MOTOR = " + String(Motor_Setpoint));
      SerialUSB.println("Setpoint HEATER = " + String(Heater_Setpoint));
      SerialUSB.println("Output MOTOR = " + String(Motor_Output));
      SerialUSB.println("Output HEATER = " + String(Heater_Output));
    
      SerialUSB.println("NTC MOTOR 1 = " + String(temp_ntc_motor_1));
      SerialUSB.println("NTC MOTOR 2 = " + String(temp_ntc_motor_2));
      SerialUSB.println("NTC HEAT 1 = " + String(temp_ntc_heat_1));
      SerialUSB.println("NTC HEAT 2 = " + String(temp_ntc_heat_2));
      SerialUSB.println("NTC OIL = " + String(temp_ntc_oil));
      SerialUSB.println("IR sensor count = " + String(IR_sensor_count));
      SerialUSB.println("Motor RPM avg = " + String(MotorRPM_avg));
      nextion_refresh_timer = millis();
    }

    yield();
  }
  yield();
}

#endif