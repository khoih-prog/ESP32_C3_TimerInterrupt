/****************************************************************************************************************************
  ESP32_C3_ISR_Timer.hpp
  For ESP32_C3 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_C3_TimerInterrupt
  Licensed under MIT license

  The ESP32-C3 has two timer groups, each one with one general purpose hardware timer. All the timers are based on 64 bits
  counters and 16 bit prescalers. The timer counters can be configured to count up or down and support automatic reload
  and software reload. They can also generate alarms when they reach a specific value, defined by the software. The value
  of the counter can be read by the software program.

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one ESP32-S2 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Based on SimpleTimer - A timer library for Arduino.
  Author: mromani@ottotecnica.com
  Copyright (c) 2010 OTTOTECNICA Italy

  Based on BlynkTimer.h
  Author: Volodymyr Shymanskyy

  Version: 1.8.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.4.0   K Hoang      29/07/2021 Initial coding. Sync with ESP32_S2_TimerInterrupt v1.4.0
  1.5.0   K.Hoang      23/01/2022 Avoid deprecated functions. Fix `multiple-definitions` linker error
  1.6.0   K Hoang      10/08/2022 Suppress errors and warnings for new ESP32 core
  1.7.0   K Hoang      11/08/2022 Suppress warnings and add support for more ESP32_C3 boards
  1.8.0   K Hoang      16/11/2022 Fix doubled time for ESP32_C3
*****************************************************************************************************************************/

#pragma once

#ifndef ISR_TIMER_GENERIC_HPP
#define ISR_TIMER_GENERIC_HPP

#if !( defined(ARDUINO_ESP32C3_DEV) || defined(ARDUINO_LOLIN_C3_MINI) || defined(ARDUINO_ADAFRUIT_QTPY_ESP32C3) || \
       defined(ARDUINO_AirM2M_CORE_ESP32C3) || defined(ARDUINO_XIAO_ESP32C3) )
  #error This code is intended to run on the ESP32-C3 platform! Please check your Tools->Board setting.
#endif

#ifndef ESP32_C3_TIMER_INTERRUPT_VERSION
  #define ESP32_C3_TIMER_INTERRUPT_VERSION            "ESP32_C3_TimerInterrupt v1.8.0"
  
  #define ESP32_C3_TIMER_INTERRUPT_VERSION_MAJOR      1
  #define ESP32_C3_TIMER_INTERRUPT_VERSION_MINOR      8
  #define ESP32_C3_TIMER_INTERRUPT_VERSION_PATCH      0

  #define ESP32_C3_TIMER_INTERRUPT_VERSION_INT        1008000  
#endif

#include "TimerInterrupt_Generic_Debug.h"

#define CONFIG_ESP32_APPTRACE_ENABLE

#if 0
  #ifndef configMINIMAL_STACK_SIZE
    #define configMINIMAL_STACK_SIZE    2048
  #else
    #undef configMINIMAL_STACK_SIZE
    #define configMINIMAL_STACK_SIZE    2048
  #endif
#endif

#include <stddef.h>

#include <inttypes.h>

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#define ESP32_ISR_Timer ESP32_ISRTimer

typedef void (*timer_callback)();
typedef void (*timer_callback_p)(void *);

class ESP32_ISR_Timer 
{

  public:
    // maximum number of timers
#define MAX_NUMBER_TIMERS        16
#define TIMER_RUN_FOREVER       0
#define TIMER_RUN_ONCE          1

    // constructor
    ESP32_ISR_Timer();

    void init();

    // this function must be called inside loop()
    void IRAM_ATTR run();

    // Timer will call function 'f' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setInterval(const unsigned long& d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setInterval(const unsigned long& d, timer_callback_p f, void* p);

    // Timer will call function 'f' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimeout(const unsigned long& d, timer_callback f);

    // Timer will call function 'f' with parameter 'p' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimeout(const unsigned long& d, timer_callback_p f, void* p);

    // Timer will call function 'f' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimer(const unsigned long& d, timer_callback f, const unsigned& n);

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setTimer(const unsigned long& d, timer_callback_p f, void* p, const unsigned& n);

    // updates interval of the specified timer
    bool changeInterval(const unsigned &numTimer, const unsigned long& d);

    // destroy the specified timer
    void deleteTimer(const unsigned& numTimer);

    // restart the specified timer
    void restartTimer(const unsigned& numTimer);

    // returns true if the specified timer is enabled
    bool isEnabled(const unsigned& numTimer);

    // enables the specified timer
    void enable(const unsigned& numTimer);

    // disables the specified timer
    void disable(const unsigned& numTimer);

    // enables all timers
    void enableAll();

    // disables all timers
    void disableAll();

    // enables the specified timer if it's currently disabled, and vice-versa
    void toggle(const unsigned& numTimer);

    // returns the number of used timers
    unsigned getNumTimers();

    // returns the number of available timers
    unsigned getNumAvailableTimers() 
    {
      return MAX_NUMBER_TIMERS - numTimers;
    };

  private:
    // deferred call constants
#define TIMER_DEFCALL_DONTRUN   0       // don't call the callback function
#define TIMER_DEFCALL_RUNONLY   1       // call the callback function but don't delete the timer
#define TIMER_DEFCALL_RUNANDDEL 2       // call the callback function and delete the timer

    // low level function to initialize and enable a new timer
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setupTimer(const unsigned long& d, void* f, void* p, bool h, const unsigned& n);

    // find the first available slot
    int findFirstFreeSlot();

    typedef struct 
    {
      unsigned long prev_millis;        // value returned by the millis() function in the previous run() call
      void*         callback;           // pointer to the callback function
      void*         param;              // function parameter
      bool          hasParam;           // true if callback takes a parameter
      unsigned long delay;              // delay value
      unsigned      maxNumRuns;         // number of runs to be executed
      unsigned      numRuns;            // number of executed runs
      bool          enabled;            // true if enabled
      unsigned      toBeCalled;         // deferred function call (sort of) - N.B.: only used in run()
    } timer_t;

    volatile timer_t timer[MAX_NUMBER_TIMERS];

    // actual number of timers in use (-1 means uninitialized)
    volatile int numTimers;

    // ESP32 is a multi core / multi processing chip. It is mandatory to disable task switches during ISR
    portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
};


#endif    // ISR_TIMER_GENERIC_HPP


