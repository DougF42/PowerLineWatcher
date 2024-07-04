/**
 * This uses the (pre-established) ACS712
 */
#include <Arduino.h>
#include "config.h"
#include "ACS712.h"
#include "Led.h"

#ifndef P_O_W_E_R__C_N_T_R_L__H
#define P_O_W_E_R__C_N_T_R_L__H

class Pwr{
   
private:
  Led led;
  ACS712 *acs712;
  TIME_t rate; // min time (in millisecs) to check readings.
  TIME_t nextTimeStamp;    
  TIME_t cooldownTimeout; // when the cooldown time expires
  enum CNTRL_STATE {CNTRL_OFF, CNTRL_ON, CNTRL_ALMOST_OFF} control_state;
  

public:
  Pwr();
  ~Pwr();
  void begin(ACS712 *acsPtr, long intrate);
  void loop();
  void output_state(); // Output the current state on stdout
};
 #endif