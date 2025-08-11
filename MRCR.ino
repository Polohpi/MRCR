#include "lib_variable.h"
#include "function_hardware.h"
#include "page.h"
#include "function_nextion.h"



void setup() {
  
  analogWriteResolution(14); //14 because 16 make the PID and the motor very very long to respond
  SerialUSB.begin(9600);

  SerialUSB.println("Setup");
  Scheduler.startLoop(ntc_update);
  Scheduler.startLoop(backend_nextion);
  Scheduler.startLoop(backend_PID);
  Scheduler.startLoop(RPM_update);
  Scheduler.startLoop(HEAT_PID_update);
  Scheduler.startLoop(debug);

  Nextion.begin(9600); // Begin the object with a baud rate of 9600

  Motor_PID.SetMode(AUTOMATIC);
  Heater_PID.SetMode(AUTOMATIC);

  Motor_PID.SetOutputLimits(0, 6700); //see backend_PID()
  Heater_PID.SetOutputLimits(0,6700);

  Avg_Temp_ntc_oil.begin(); // start movingavg for heating, motor and ntc sensor
  Avg_MotorRPM.begin();
  Avg_Temp_ntc_mosfet_motor_1.begin();
  Avg_Temp_ntc_mosfet_motor_2.begin();
  Avg_Temp_ntc_mosfet_heat_1.begin();
  Avg_Temp_ntc_mosfet_heat_2.begin();
  Avg_MotorRPM.reset();

  pinMode(PIN_NTC_MOSFET_MOTOR_1, INPUT);      // set pinmode
  pinMode(PIN_NTC_MOSFET_MOTOR_2, INPUT);
  pinMode(PIN_NTC_MOSFET_HEAT_1, INPUT);
  pinMode(PIN_NTC_MOSFET_HEAT_2, INPUT);
//  pinMode(PIN_NTC_MOTOR, INPUT_PULLUP); //pullup here to avaoid erratic reaing before the pcb v2 wich will include a ntc sensor for the DC motor itself
  pinMode(PIN_NTC_OIL, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_HEATER, OUTPUT); 
  pinMode(PIN_IR_SENSOR, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_IR_SENSOR), RPM_interrupt, FALLING);
  yield();
}

void loop() 
{
  Nextion.writeStr("page 1");
  //ChangePageMenu = 0;
  while (1)
  {  
    if(ChangePageMenu == MANUAL_MODE_PAGE)
    {
      ManualModePage();
    }
    yield();
  }
  yield();
}
