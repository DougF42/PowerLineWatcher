/**
 * @file: Controls the LED state
 */
#include <Arduino.h>
#include "config.h"
#include "Led.h"
  Led::Led()
  {
    pinMode(LED_PIN,OUTPUT);    
    ledState=false;
    digitalWrite(LED_PIN, ledState);
  }


  Led::~Led()
  {
    ledState=false;
    digitalWrite(LED_PIN, ledState);
  }


  void Led::set(bool state)
  {
    ledState=state;
    digitalWrite(LED_PIN, ledState);
  }


  void Led::toggle()
  {
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
  }