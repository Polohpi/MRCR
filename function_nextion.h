#ifndef FUCNTION_NEXTION_H
#define FUNCTION_NEXTION_H

void trigger40() //to main page
{
  SerialUSB.println("trigger40 - Main Page");
  ChangePageMenu = MAIN_PAGE;
  yield();
}

void trigger41() //to ManualMode Page
{
  SerialUSB.println("trigger41 - ManualModePage");
  ChangePageMenu = MANUAL_MODE_PAGE;
  yield();
}

void trigger0()// SET values in ManualMode 
{
  //SerialUSB.println("trigger0");
  // Motor_Setpoint = ManualMode_SetRPM[Nextion.readNumber("SET_RPM.val")];
  // Heater_Setpoint = ManualMode_SetHeat[Nextion.readNumber("SET_TEMP.val")];
  Motor_Setpoint = Nextion.readNumber("SET_RPM.val");
  Heater_Setpoint = Nextion.readNumber("SET_TEMP.val");
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