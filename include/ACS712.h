/*
 * HEADER for ACS712 driver.
 (currently set up for ARDIUNO based analog I/O)
 */
#include "config.h"
#ifndef A_C_S_7_1_2__H
#define A_C_S_7_1_2__H

class ACS712 {
private:
  PIN_t analogPinNo;
  double reMap(double x, double in_min, double in_max, double out_min, double out_max);
  float device_min, device_max;
  int zeroOffset;
  bool acFilterFlag;
  bool filterFlag;
  bool forceFlag; 
  int forceVal;
  int window[ACS712_FILTER_WINDOW_SIZE];
  int nxtWin;  // index of next window position
  int total;   // current total

public:
  enum ACS712_MODEL_e { ACS712_X5A = 1,
                        ACS712_X20A,
                        ACS712_X30A };

  ACS712();
  ~ACS712();
  void begin(enum ACS712_MODEL_e model, PIN_t pin);
  void loop();
  void enableAcFilter(bool state);
  bool getAcFilterFlag();
  void enableFilter(bool state);
  bool getFilterFlag();

  int setZeroOffset();
  int getRawAnalogData();
  double getCurrentReading();
  void force(int val); // force value. (good for testing)
  void cancelForce();  // cancel forced value.
};

#endif
