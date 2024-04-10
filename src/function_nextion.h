#ifndef FUNCTION_NEXTION_H
#define FUNCTION_NEXTION_H
#include "lib_variable.h"


void trigger0(){
  // To call this void send from Nextion's component's Event:  printh 23 02 54 00
  // In this exmaple, we send this command from the Release Event of b0 button (see the HMI of this example)
  // You can send  the same `printh` command, to call the same function, from more than one component, depending on your needs
  // This is used for the "SET" button
    uint32_t number;
    number = myNex.readNumber("n1.val");   // We read the value of n0 and store it to number variable
    Setpoint = int(number);
}

#endif