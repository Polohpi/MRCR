#ifndef FUNCTION_HARDWARE_H
#define FUNCTION_HARDWARE_H


float ReadTempNTC(int pin)
{
  int Ro = 10, B =  3435; //Nominal resistance 10K, Beta constant
  int Rseries = 10;// Series resistor 10K
  float To = 298.15; // Nominal Temperature

/*Read analog outputof NTC module,
   i.e the voltage across the thermistor */
  float Vi = 5.0 - analogRead(pin) * (5.0 / 1023.0);
  //Convert voltage measured to resistance value
  //All Resistance are in kilo ohms.
  float R = (Vi * Rseries) / (5 - Vi);
  /*Use R value in steinhart and hart equation
    Calculate temperature value in kelvin*/
  float T =  1 / ((1 / To) + ((log(R / Ro)) / B));
  float Tc = T - 273.15; // Converting kelvin to celsius
  return Tc;
  yield();

}


void ntc_update()
{
  float avg_temp_ntc_motor_1 = 0;
  float avg_temp_ntc_motor_2 = 0;
  float avg_temp_ntc_heat_1 = 0;
  float avg_temp_ntc_heat_2 = 0;
  float avg_temp_ntc_oil = 0;

  for(int i = 0; i<50; i++)
  {
    avg_temp_ntc_motor_1 = avg_temp_ntc_motor_1 + ReadTempNTC(PIN_NTC_MOTOR_1);
    avg_temp_ntc_motor_2 = avg_temp_ntc_motor_2 + ReadTempNTC(PIN_NTC_MOTOR_2);
    avg_temp_ntc_heat_1 = avg_temp_ntc_heat_1 + ReadTempNTC(PIN_NTC_HEAT_1);
    avg_temp_ntc_heat_2 = avg_temp_ntc_heat_2 + ReadTempNTC(PIN_NTC_HEAT_2);
    avg_temp_ntc_oil = avg_temp_ntc_oil + ReadTempNTC(PIN_NTC_OIL);
  }


  temp_ntc_motor_1 = avg_temp_ntc_motor_1 /50;
  temp_ntc_motor_2 = avg_temp_ntc_motor_2 /50;
  temp_ntc_heat_1 = avg_temp_ntc_heat_1 /50;
  temp_ntc_heat_2 = avg_temp_ntc_heat_2 /50;
  temp_ntc_oil = avg_temp_ntc_oil / 50;
  yield();

}

#endif