/*
 * This controls the on-board LED
 */
 #ifndef L_E_D__H
 #define L_E_D__H

 #ifndef ON
 #define ON true
 #endif

 #ifndef OFF
 #define OFF false
 #endif

 class Led{
  private:
    bool ledState;

  public:
  Led();
  ~Led();
  void set(bool state);
  void toggle();
 };
 #endif