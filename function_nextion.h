#ifndef FUCNTION_NEXTION_H
#define FUNCTION_NEXTION_H

void trigger40()
{
  SerialUSB.println("trigger40 - Main Page");
  ChangePageMenu = 1;
  yield();
}

void trigger41()
{
  SerialUSB.println("trigger41 - Acceuil");
  ChangePageMenu = 2;
  yield();
}

void trigger0()
{
  //SerialUSB.println("trigger0");
  Motor_Setpoint = Nextion.readNumber("SET_RPM.val");
  //Heater_Setpoint = Nextion.readNumber("SET_TEMP.val");
  yield();
}

void trigger1()
{
  SerialUSB.println("trigger1");
  Motor_Setpoint = 0;
  Heater_Setpoint = 0;
  yield();
}

#endif