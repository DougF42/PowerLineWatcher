/**
 * @file watchdogs.cpp
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include "config.h"
#ifdef ESP32
//#include "freertos/FreeRTOS.h"
//#include "freertos/task.h"
#include "soc/rtc_wdt.h"
#endif

#ifdef ARDUINO_AVR_UNO
#include <Arduino.h>
#include <avr/wdt.h>
#endif

#include "watchdog.h"

Watchdog::Watchdog()
{
// basic init.
#ifdef ESP32
        rtc_wdt_protect_off();
        rtc_wdt_disable();
#endif
}

/**
 * @brief make sure the WDT is
 *
 */
void Watchdog::setup()
{
#ifdef ESP32
        rtc_wdt_set_length_of_reset_signal(RTC_WDT_SYS_RESET_SIG, RTC_WDT_LENGTH_3_2us);
        // Only one of the following...
        //  rtc_wdt_set_stage(RTC_WDT_STAGE0, 	RTC_WDT_STAGE_ACTION_RESET_SYSTEM);
        rtc_wdt_set_stage(RTC_WDT_STAGE0, RTC_WDT_STAGE_ACTION_RESET_SYSTEM);
        rtc_wdt_set_time(RTC_WDT_STAGE0, WDT_TIMEOUT); // timeout rtd_wdt 7000ms.
        rtc_wdt_enable();
        rtc_wdt_protect_on();
#endif
#ifdef ARDUINO_AVR_UNO
        wdt_disable();       /* Disable the watchdog and wait for more than 2 seconds */
        delay(3000);         /* Done so that the Arduino doesn't keep resetting infinitely in case of wrong configuration */
        wdt_enable(WDTO_2S); /* Enable the watchdog with a timeout of 2 seconds */
#endif
}

Watchdog::~Watchdog()
{
}

/**
 * @brief Called this to let the timer know
 *    you are stil alive.
 *
 */
void Watchdog::iAmAlive()
{
#ifdef ESP32
        rtc_wdt_feed();
#endif
#ifdef ARDUINO_AVR_UNO
        wdt_reset();  /* Reset the watchdog */
#endif
}