#ifndef C_O_N_F_I_G___H
#define C_O_N_F_I_G___H

typedef int PIN_t;
typedef unsigned long TIME_t;

// Watchdog timer TIMEOUT - in milliseconds
#define WDT_TIMEOUT 3000

// Which model ACS712  - 5amp, 20 amp, 30 amp?
// (definitions in ACS712.h)
#define ACS712_MODEL ACS712::ACS712_X30A

// What pin is the ACS712 input? (analog)
#define ACS712_INPUT_PIN 7

// How big is the input filter window?
#define ACS712_FILTER_WINDOW_SIZE 5

// An input so we can control this manually
#define SWITCH_PIN   4


// The LED indicator. Rate is in milliseconds
#define LED_PIN      2
#define LED_BLINK_RATE 500


// The Digital output pin that drives the Solid State Relay
#define SSRELAY_PIN 3

// How often (millis) to check the ACS712 and update the SSR?
#define SSRELAY_CYCLE_TIME 16

// Above this reading (current in ma) the main device is running.
//#define ON_THREASHOLD 700
#define ON_THREASHOLD 40

// Below this reading (current in ma) the main device is shut down.
//#define OFF_THREASHOLD 500
#define OFF_THREASHOLD 25

// How long(milliseconds) to hold the auxilary circuit live after the 
// main shuts down
#define COOLDOWN_TIME 2000

#endif