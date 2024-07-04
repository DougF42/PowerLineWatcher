/*
 * Read from the ACS712
 * zeroOffset is intended to compensate for minor imbalence
 *    in the voltage reading. A 10-second average is
 *    automatically taken when the class is initialized and
 *    used as the 'zero' value.
 *
 * The acFilterFlag (if true) assumes that current flowing in
 *       either direction is the same; we return the absolute 
 *      value of the reading.
 *
 * If 'filterFlag' is true, then loop must be called as frequently
 *     as possible - we continually take readings, and do a 
 *     moving window average of the values read. The 'getCurrentReading'
 *     method returns the average in this case - otherwise, it does
 *     an analog read an returns the current.
 *
 * IF the 'force(value)' method is set, then getRawAnalogData will always return the 
 *      given value, until changed by another call to force, or calling
 *     the cancelForce() method.
 */
#include <Arduino.h>
#include "config.h"
#include "ACS712.h"

/**
 * Same as arduino 'map()', but defined for double.
 */
double ACS712::reMap(double x, double in_min, double in_max, double out_min, double out_max) {
  return (out_min + (x - in_min) * ((out_max - out_min) / (in_max - in_min)));
}


/**
 * @brief: Initialize class.
 */
ACS712::ACS712() {
  zeroOffset = 0;
  acFilterFlag = true;  // default - filter on
  filterFlag = false;
  forceFlag = false;
  nxtWin = 0;  // index of next window position
  total = 0;   // current total
  for (int i = 0; i < ACS712_FILTER_WINDOW_SIZE; i++) window[i] = 0;
  return;
}


/**
 * Define the specific model of ACS712 (its range) and the pin number
 * @param: model - the model of ACS712. (See ACS712_MODEL_e enumeration)
 * @param: pin   - the analog pin number the ACS712 is on.
 * @NOTE: The data range is in milliamps, NOT amps!
 * Anaalog conversion is 0...1024. For each range, 1 bit change
 * is as follows:
 *   MODEL   Range       1 BIT
 *   -----   ----------  -------
 *    X5A    +/-  5 amp   0.97 ma
 *    X20A   +/- 20 amp   3.9 ma
 *    X30A   +/- 30 amp   5.8 ma
 */
void ACS712::begin(enum ACS712_MODEL_e model, PIN_t pin) {
  analogPinNo = pin;
  switch (model) {  // Note: with 0...1024 analog conversion,
    case (ACS712_X5A):
      device_min = -500.0;
      device_max = 500.0;
      break;

    case (ACS712_X20A):
      device_min = -2000.0;
      device_max = 2000.0;
      break;

    case (ACS712_X30A):
      device_min = -3000.0;
      device_max = 3000.0;
      break;

    default:
      Serial.println("ERROR: Unknown model type in ACS712::begin");
      break;
  }
  // Ref on Uno and Nano is 5V (Vcc).
  //analogReference(DEFAULT);
}


// default destructor
ACS712::~ACS712() {
  return;
}


/**
 * @brief Set the Zero offset
 *
 * We take readings for 10 seconds, and average them.
 * the result an offset applied to the raw reading.
 *  to get a 'true' reading.
 */
int ACS712::setZeroOffset() {
  zeroOffset = 0;
#define ZERO_COUNTER 20
  Serial.println("Checking for zero");
  int work = 0;
  for (int i = 0; i < ZERO_COUNTER; i++) {
    work += analogRead(analogPinNo);
    //work+=getRawAnalogData();
    delay(250);  // check at 1/2 sec intervals
  }

  zeroOffset = work / ZERO_COUNTER;
  Serial.print("Zero detected at ");
  Serial.print(zeroOffset);
  Serial.print(" Ideal is 512. Diff is ");
  Serial.println(512 - zeroOffset);

  //zeroOffset=512;
  return (zeroOffset);
}


/**
 * @brief: Get the raw analog reading, with the zero offset applied.
 * Because we are reading AC, we actually take a bunch of readings, and
 * use the highest.
 * This applies the zero offset, so the range of a reading is -512 to 512
 * Also, we apply the acFilterFlag here (if set)
 */
int ACS712::getRawAnalogData() {
  int reading;
  int max = 0;
  unsigned long int timeout = millis() + 32;  // 32 msec is two 60-=hz cycles

  while (millis() < timeout) {
    reading = analogRead(analogPinNo) ;
    //Serial.print("Last reading was :");Serial.print(reading);Serial.print(" ");
    reading -= zeroOffset;
    if (acFilterFlag)              reading = abs(reading);
    //Serial.print(" With offset ="); Serial.println(reading);
    if (reading > max) max = reading;
  }
  //  Serial.print("MAX IS:"); Serial.println(max);
  return (max);
}


/**
 * @brief continuously build up a running average
 */
void ACS712::loop() {
  total -= window[nxtWin];                            // remove old reading from total
  window[nxtWin] = getRawAnalogData();                // replace old with new reading
  total += window[nxtWin];                            // and add new reading into the total
  nxtWin = (nxtWin + 1) % ACS712_FILTER_WINDOW_SIZE;  // next...
  // NOTE to save processor cycles, we dont calculate the average until we
  //    actually need it (in getCurrentReading).
}


/**
   * @brief: Get the current reading from the sensor (in milliamps)
   * @return: The current in milliamps.
   
   */
double ACS712::getCurrentReading() {
  double res;
  if (filterFlag) {
    res = total / ACS712_FILTER_WINDOW_SIZE;
    res = reMap(res, -512, 512, device_min, device_max);
  } else {
    int reading = getRawAnalogData();
    if (acFilterFlag)
    {
      res = reMap(reading, 0, 512,  0, device_max);
    } else {
      res = reMap(reading, -512, 512, device_min, device_max);
    }
  }

  return (res);
}


void ACS712::enableAcFilter(bool state) {
  acFilterFlag = state;
}


bool ACS712::getAcFilterFlag() {
  return (acFilterFlag);
}


void ACS712::enableFilter(bool state) {
  filterFlag = state;
}


bool ACS712::getFilterFlag() {
  return (filterFlag);
}

void ACS712::force(int val) {
  forceVal = val;
  forceFlag = true;
}

void ACS712::cancelForce() {
  forceFlag = false;
  forceVal = 0;
}