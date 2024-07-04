/**
 * @file watchdogs.h
 * @author Doug Fajardo (doug.fajardo@gmail.com)
 * @brief  configure and enable the watchdog timer
 * @version 0.1
 * @date 2024-07-03
 * 
 * @copyright Copyright (c) 2024
 * 
 * Future note: The intial version was written 
 * for the pre-processor target 'ESP32'.
 * 
 */

#ifndef W_A_T_C_H__D_O_G__H
#define W_A_T_C_H__D_O_G__H

class Watchdog
{
    public:
        Watchdog(); // basic init.
        void setup();    // Run-time setup.
        ~Watchdog(); // stop.
        void iAmAlive(); // call this to let the timer know we are still alive.
        #ifdef ESP32
        const char *xxx;
        #endif
};
#endif