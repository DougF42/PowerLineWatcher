
/**
 * @brief control the auxilary circuit
 */
#include <Arduino.h>
#include "Pwr.h"



Pwr::Pwr() {
  acs712 = nullptr;
  rate = 0L;
  nextTimeStamp = 0L;
  cooldownTimeout = 0L;
  control_state = CNTRL_OFF;
  digitalWrite(SSRELAY_PIN, false);
}


/** @brief Destructor
 *
 */
Pwr::~Pwr() {
}


void Pwr::begin(ACS712 *acsPtr, long int _rate) {
  acs712 = acsPtr;
  rate = _rate;
  nextTimeStamp = 0;
  digitalWrite(SSRELAY_PIN, false);
}


/**
 * @brief: Call frequently
 *   We return immediatly if its not yet time to do something...
 */
void Pwr::loop() {
  if (acs712 == nullptr) Serial.println("PANIC! acs712 not defined in Pwr::loop !");
  acs712->loop();
  if (nextTimeStamp > millis()) return;  // Not yet
  nextTimeStamp = millis() + rate;       // This can be overridden by the state machine!

  // STATE MACHINE
  int currentReading = acs712->getCurrentReading();
  Serial.print(" control state:");
  output_state();
  Serial.print(" Reading: ");
  Serial.println(currentReading);

  switch (control_state) {
    case (CNTRL_OFF):  // should we turn on?
      if (currentReading > ON_THREASHOLD) {
        led.set(ON);
        digitalWrite(SSRELAY_PIN, true);
        control_state = CNTRL_ON;
      }
      break;


    case (CNTRL_ON):  // Are we still on? if not, start ofcooldown timer
      if (currentReading < OFF_THREASHOLD) {
        cooldownTimeout = millis() + COOLDOWN_TIME;
        control_state = CNTRL_ALMOST_OFF;
      }
      break;


    case (CNTRL_ALMOST_OFF):                 // can we turn it off yet?
      if (currentReading > ON_THREASHOLD) {  // oops - he started it again. back to 'on' state.
        control_state = CNTRL_ON;
      }

      if (millis() > cooldownTimeout) {  // Ahhh cooldown expired, shut it off.
        digitalWrite(SSRELAY_PIN, false);
        led.set(OFF);
        control_state = CNTRL_OFF;
      }
      break;
  }
}

// Output the current state on stdout
void Pwr::output_state() {
  switch (control_state) 
  {
    case(CNTRL_OFF):
    Serial.print("CNTRL_OFF");
    break;

    case(CNTRL_ON):
    Serial.print("CNTRL_ON");
    break;

    case(CNTRL_ALMOST_OFF):
    Serial.print("CNTRL_ALMOST_OFF");
    break;
  }
}