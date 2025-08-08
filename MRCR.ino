#include "lib_variable.h"
#include "function_hardware.h"
#include "page.h"
#include "function_nextion.h"



void setup() {
  analogWriteResolution(14);
  SerialUSB.begin(9600);

  SerialUSB.println("Setup");
  Scheduler.startLoop(ntc_update);
  Scheduler.startLoop(backend_nextion);
  Scheduler.startLoop(backend_PID);
  Scheduler.startLoop(RPM_counter);

  Nextion.begin(9600); // Begin the object with a baud rate of 9600

  Motor_PID.SetMode(AUTOMATIC);
  Heater_PID.SetMode(AUTOMATIC);

  Motor_PID.SetOutputLimits(0, 50000);
  Heater_PID.SetOutputLimits(0,1023);

  Avg_Oil_Temp.begin(); // start movingavg for heating and motor
  Avg_MotorRPM.begin();

  pinMode(PIN_NTC_MOTOR_1, INPUT);      // set pinmode
  pinMode(PIN_NTC_MOTOR_2, INPUT);
  pinMode(PIN_NTC_HEAT_1, INPUT);
  pinMode(PIN_NTC_HEAT_2, INPUT);
  pinMode(PIN_NTC_OIL, INPUT);
  pinMode(PIN_MOTOR, OUTPUT);
  pinMode(PIN_HEATER, OUTPUT); 
  pinMode(PIN_IR_SENSOR, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(PIN_IR_SENSOR), RPM_interrupt, FALLING);
  yield();
}

void loop() 
{
  SerialUSB.println("loop");
  Acceuil();
  yield();
}
