#ifndef FUCNTION_NEXTION_H
#define FUNCTION_NEXTION_H

//declaration of protoype
void trigger50();
void trigger40();
void trigger41();
void trigger0();
void trigger1();

void trigger0()// SET values in ManualMode 
{
  Serial.println("trigger0 - SET");
  Motor_Setpoint = ManualMode_SetRPM[Nextion.readNumber("SET_RPM.val")];
  Setpoint_HEATER = ManualMode_SetHeat[Nextion.readNumber("SET_TEMP.val")];
  Setpoint_Timer = ManualMode_SetTimer[Nextion.readNumber("timer_select.val")];
  if(Setpoint_Timer != 0)
  {
    New_Manual_Timer(Setpoint_Timer);
    MANUALTIMER_STATE = true;
  }
  else
  {
    MANUALTIMER_STATE = false;
    New_Manual_Timer(Setpoint_Timer);
  }
  yield();
}
//trigger when button "set" is pressed in manualmode
void trigger1()
{
  Serial.println("trigger1 - STOP");
  Stop();
  yield();
}

void trigger40() //to main page
{
  Serial.println("trigger40 - Main Page");
  MANUAL_MODE_PAGE = false;
  MAIN_PAGE = true;
  Nextion.writeStr("page Main");
  yield();
}

void trigger41() //to ManualMode Page
{
  //Serial.println("trigger41 - ManualModePage");
  MAIN_PAGE = false;
  MANUAL_MODE_PAGE = true;
  Nextion.writeStr("page Manual_Mode");
  yield();
}

void trigger50() //Alerte
{
  Serial.println("trigger50 - Alarm");
  MANUAL_MODE_PAGE = false;
  MAIN_PAGE = true;
  Nextion.writeStr("page DiagSensor");

  yield();
}



#endif