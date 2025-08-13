#ifndef FUCNTION_NEXTION_H
#define FUNCTION_NEXTION_H

void trigger40() //to main page
{
  Serial.println("trigger40 - Main Page");
  ChangePageMenu = MAIN_PAGE;
  yield();
}

void trigger41() //to ManualMode Page
{
  Serial.println("trigger41 - ManualModePage");
  ChangePageMenu = MANUAL_MODE_PAGE;
  yield();
}

void trigger0()// SET values in ManualMode 
{
  Serial.println("trigger0 - SET");
  // Motor_Setpoint = ManualMode_SetRPM[Nextion.readNumber("SET_RPM.val")];
  // Heater_Setpoint = ManualMode_SetHeat[Nextion.readNumber("SET_TEMP.val")];
  Motor_Setpoint = Nextion.readNumber("SET_RPM.val");
  Setpoint_HEATER = Nextion.readNumber("SET_TEMP.val");
  yield();
}

void trigger1()
{
  Serial.println("trigger1 - STOP");
  Motor_Setpoint = 0;
  Setpoint_HEATER = 0;
  yield();
}

#endif