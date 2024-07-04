/**
 * @brief Power Monitor
 *   (This was developed on ESP32 DOIT KIT V1. Arduino
 * device (nano?) should work EXCEPT for WatchDog time
 * controls (see watchDog.* class).
 * 
 * Use an ACS712 current monitor chip to watch the current 
 * draw on a main (120 volt circuit) in a 'main' circuit 
 * (a.k.a. a table saw), and turn on an auxillary circuit
 * (e.g.: Vacuum system) when the 'main' circuit is on,
 * using a solid state relay.
 *
 * The auxilary circuit is kept on until 
 * 2 seconds after the main circuit stops drawing power.
 * 
 * To drive the auxillary circuit, We are using a Solid State Relay.
 *
 * Both the arduino and the ACS712 use a 5 volt supply; a USB 'wall wort' 
 * should be adaquate - The digital power should be under 100 ma.
 *
 * A simple LED indicator tells the status:
 *   - Blinking - Main Power is available, but turned off.
 *   - On Steady - Power to the auxilary circuit is ON.
 * In addition,  the power can be toggled by a touch? push button? switch.
 *
 * 7/3/2024 Adding watchdog timer
 */
 #include "config.h"
 // #include "esp_system.h"
 #include "ACS712.h"
 #include "Pwr.h"
 #include "watchdog.h"

 
 // For testing ACS712 
 //#define TESTME

ACS712  acs712; // our device
Pwr pwr;        // Power monitor
Watchdog watchdog; // watch dog timer

void setup() {
  Serial.begin(19200);
  while (! Serial) ; // wait for serial port
  Serial.println("Serial port is ready");
  Serial.flush();
  delay(250);

  // Configure the relay
  pinMode(SSRELAY_PIN, OUTPUT);

  // Configure the ACS712
  acs712.enableAcFilter(true);
  acs712.enableFilter(false);
  acs712.begin( ACS712_MODEL, ACS712_INPUT_PIN);
  acs712.setZeroOffset();

  pwr.begin(&acs712, SSRELAY_CYCLE_TIME);
  watchdog.setup();
  Serial.println("END OF INIT");
}
 
unsigned long startTime = 0;
void loop() {
  //acs712.loop();
  pwr.loop();
  watchdog.iAmAlive();
  delay(250);
}
