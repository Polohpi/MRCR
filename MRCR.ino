#include "lib_variable.h"
#include "function_hardware.h"

void setup() {

  SerialUSB.begin(9600);

  Scheduler.startLoop(ntc_update);

}

void loop() 
{
  SerialUSB.println("NTC MOTOR 1 = " + String(temp_ntc_motor_1));
  SerialUSB.println("NTC MOTOR 2 = " + String(temp_ntc_motor_2));
  SerialUSB.println("NTC HEAT 1 = " + String(temp_ntc_heat_1));
  SerialUSB.println("NTC HEAT 2 = " + String(temp_ntc_heat_2));
  SerialUSB.println("NTC OIL = " + String(temp_ntc_oil));
  yield();
}
