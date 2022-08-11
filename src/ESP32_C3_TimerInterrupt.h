
/****************************************************************************************************************************
  ESP32_C3_TimerInterrupt.h
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

  Version: 1.6.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.4.0   K Hoang      29/07/2021 Initial coding. Sync with ESP32_S2_TimerInterrupt v1.4.0
  1.5.0   K.Hoang      23/01/2022 Avoid deprecated functions. Fix `multiple-definitions` linker error
  1.6.0   K Hoang      10/08/2022 Suppress errors and warnings for new ESP32 core
*****************************************************************************************************************************/

#pragma once

#ifndef ESP32_C3_TIMERINTERRUPT_H
#define ESP32_C3_TIMERINTERRUPT_H

#if !( ARDUINO_ESP32C3_DEV )
  #error This code is intended to run on the ESP32-C3 platform! Please check your Tools->Board setting.
#else
  #define USING_ESP32_C3_TIMER_INTERRUPT        true  
#endif

#ifndef ESP32_C3_TIMER_INTERRUPT_VERSION
  #define ESP32_C3_TIMER_INTERRUPT_VERSION            "ESP32_C3_TimerInterrupt v1.6.0"
  
  #define ESP32_C3_TIMER_INTERRUPT_VERSION_MAJOR      1
  #define ESP32_C3_TIMER_INTERRUPT_VERSION_MINOR      6
  #define ESP32_C3_TIMER_INTERRUPT_VERSION_PATCH      0

  #define ESP32_C3_TIMER_INTERRUPT_VERSION_INT        1006000  
#endif

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
#endif

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#include "TimerInterrupt_Generic_Debug.h"

#include <driver/timer.h>

/*
  //ESP32 core v1.0.6, hw_timer_t defined in esp32/tools/sdk/include/driver/driver/timer.h:

  #define TIMER_BASE_CLK   (APB_CLK_FREQ)  //Frequency of the clock on the input of the timer groups 


 //@brief Selects a Timer-Group out of 2 available groups
 
typedef enum
{
  TIMER_GROUP_0 = 0, // Hw timer group 0
  TIMER_GROUP_1 = 1, // Hw timer group 1
  TIMER_GROUP_MAX,
} timer_group_t;


 //@brief Select a hardware timer from timer groups
 
typedef enum 
{
  TIMER_0 = 0, // Select timer0 of GROUPx
  TIMER_1 = 1, // Select timer1 of GROUPx
  TIMER_MAX,
} timer_idx_t;


 //@brief Decides the direction of counter
 
typedef enum 
{
  TIMER_COUNT_DOWN = 0, //Descending Count from cnt.high|cnt.low
  TIMER_COUNT_UP = 1,   //Ascending Count from Zero
  TIMER_COUNT_MAX
} timer_count_dir_t;


 //@brief Decides whether timer is on or paused
 
typedef enum 
{
  TIMER_PAUSE = 0,      //Pause timer counter
  TIMER_START = 1,      //Start timer counter
} timer_start_t;


 //@brief Decides whether to enable alarm mode
 
typedef enum 
{
  TIMER_ALARM_DIS = 0,  //Disable timer alarm
  TIMER_ALARM_EN = 1,   //Enable timer alarm
  TIMER_ALARM_MAX
} timer_alarm_t;


 //@brief Select interrupt type if running in alarm mode.
 
typedef enum 
{
  TIMER_INTR_LEVEL = 0,  //Interrupt mode: level mode
  //TIMER_INTR_EDGE = 1, //Interrupt mode: edge mode, Not supported Now
  TIMER_INTR_MAX
} timer_intr_mode_t;


 //@brief Select if Alarm needs to be loaded by software or automatically reload by hardware.
 
typedef enum 
{
  TIMER_AUTORELOAD_DIS = 0,  //Disable auto-reload: hardware will not load counter value after an alarm event
  TIMER_AUTORELOAD_EN = 1,   //Enable auto-reload: hardware will load counter value after an alarm event
  TIMER_AUTORELOAD_MAX,
} timer_autoreload_t;


 //@brief Data structure with timer's configuration settings
 
typedef struct 
{
  bool alarm_en;                    //Timer alarm enable 
  bool counter_en;                  //Counter enable 
  timer_intr_mode_t intr_type;      //Interrupt mode 
  timer_count_dir_t counter_dir;    //Counter direction  
  bool auto_reload;                 //Timer auto-reload 
  uint32_t divider;                 //Counter clock divider. The divider's range is from from 2 to 65536. 
} timer_config_t;

*/

/*
  //ESP32 core v2.0.4, timer_config_t defined in tools/sdk/esp32/include/hal/include/hal/timer_types.h:
  #if SOC_TIMER_GROUP_SUPPORT_XTAL
  typedef enum {
    TIMER_SRC_CLK_APB = 0,  // Select APB as the source clock
    TIMER_SRC_CLK_XTAL = 1, // Select XTAL as the source clock
  } timer_src_clk_t;
  #endif
  typedef struct {
    timer_alarm_t alarm_en;           // Timer alarm enable
    timer_start_t counter_en;         // Counter enable
    timer_intr_mode_t intr_type;      // Interrupt mode
    timer_count_dir_t counter_dir;    // Counter direction
    timer_autoreload_t auto_reload;   // Timer auto-reload
    uint32_t divider;                 // Counter clock divider. The divider's range is from from 2 to 65536
  #if SOC_TIMER_GROUP_SUPPORT_XTAL
    timer_src_clk_t clk_src;          // Use XTAL as source clock
  #endif
  } timer_config_t;

*/

class ESP32TimerInterrupt;

typedef ESP32TimerInterrupt ESP32Timer;

#define MAX_ESP32_NUM_TIMERS      2

#define TIMER_DIVIDER             80                                //  Hardware timer clock divider
// TIMER_BASE_CLK = APB_CLK_FREQ = Frequency of the clock on the input of the timer groups
#define TIMER_SCALE               (TIMER_BASE_CLK / TIMER_DIVIDER)  // convert counter value to seconds


// In esp32/1.0.6/tools/sdk/esp32s2/include/driver/include/driver/timer.h
// typedef bool (*timer_isr_t)(void *);
//esp_err_t timer_isr_callback_add(timer_group_t group_num, timer_idx_t timer_num, timer_isr_t isr_handler, void *arg, int intr_alloc_flags);
//esp_err_t timer_isr_callback_remove(timer_group_t group_num, timer_idx_t timer_num);
//timer_deinit(timer_group_t group_num, timer_idx_t timer_num);
//esp_err_t timer_group_intr_enable(timer_group_t group_num, timer_intr_t intr_mask);
//esp_err_t timer_group_intr_disable(timer_group_t group_num, timer_intr_t intr_mask);

typedef bool (*esp32_timer_callback)  (void *);

typedef struct
{
  timer_idx_t         timer_idx;
  timer_group_t       timer_group;
  //int                 alarm_interval;
  //timer_autoreload_t  auto_reload;
} timer_info_t;

class ESP32TimerInterrupt
{
  private:
  
    timer_config_t stdConfig = 
    {
      .alarm_en     = TIMER_ALARM_EN,       //enable timer alarm
      .counter_en   = TIMER_START,          //starts counting counter once timer_init called
      .intr_type    = TIMER_INTR_MAX,
      .counter_dir  = TIMER_COUNT_UP,       //counts from 0 to counter value
      .auto_reload  = TIMER_AUTORELOAD_EN,  //reloads counter automatically
      .divider      = TIMER_DIVIDER,
#if SOC_TIMER_GROUP_SUPPORT_XTAL
      .clk_src      = TIMER_SRC_CLK_XTAL    //Use XTAL as source clock
#endif      
    };

    timer_idx_t       _timerIndex;
    timer_group_t     _timerGroup;
    uint32_t          interruptFlag;        // either TIMER_INTR_T0 or TIMER_INTR_T1
    
    uint8_t           _timerNo;

    esp32_timer_callback _callback;         // pointer to the callback function
    
    float             TIM_CLOCK_FREQ;       // Timer Clock
    float             _frequency;           // Timer frequency
    uint64_t          _timerCount;          // count to activate timer
    
    //xQueueHandle      s_timer_queue;

  public:

    ESP32TimerInterrupt(const uint8_t& timerNo)
    {     
      _callback = NULL;
        
      if (timerNo < MAX_ESP32_NUM_TIMERS)
      {
        _timerNo  = timerNo;
      
        // Always using TIMER_INTR_T0
        _timerIndex = (timer_idx_t)   ( (uint32_t) 0 );
        
        // timerNo == 0 => Group 0, timerNo == 1 => Group 1
        _timerGroup = (timer_group_t) ( (uint32_t) timerNo);     
      }
      else
      {
        _timerNo  = MAX_ESP32_NUM_TIMERS;
      }
    };

    // frequency (in hertz) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to esp32-hal-timer.c
    bool setFrequency(const float& frequency, esp32_timer_callback callback)
    {
      if (_timerNo < MAX_ESP32_NUM_TIMERS)
      {      
        // select timer frequency is 1MHz for better accuracy. We don't use 16-bit prescaler for now.
        // Will use later if very low frequency is needed.
        _frequency = frequency;
        TIM_CLOCK_FREQ  = TIMER_BASE_CLK / TIMER_DIVIDER;   //1000000;
        _timerCount = (uint64_t) TIM_CLOCK_FREQ / frequency;
        // count up
        
        TISR_LOGWARN3(F("ESP32_C3_TimerInterrupt: _timerNo = "), _timerNo, F(", TIM_CLOCK_FREQ = "), TIM_CLOCK_FREQ);
        TISR_LOGWARN3(F("TIMER_BASE_CLK = "), TIMER_BASE_CLK, F(", TIMER_DIVIDER = "), TIMER_DIVIDER);
        TISR_LOGWARN3(F("_timerIndex = "), _timerIndex, F(", _timerGroup = "), _timerGroup);
        TISR_LOGWARN5(F("Timer freq = "), _frequency, F(", _count = "), (uint32_t) (_timerCount >> 32) , F("-"), (uint32_t) (_timerCount));
        TISR_LOGWARN1(F("timer_set_alarm_value = "), TIMER_SCALE / frequency);

        timer_init(_timerGroup, _timerIndex, &stdConfig);
        
        // Counter value to 0 => counting up to alarm value as .counter_dir == TIMER_COUNT_UP
        timer_set_counter_value(_timerGroup, _timerIndex , 0x00000000ULL);       
        
        timer_set_alarm_value(_timerGroup, _timerIndex, TIMER_SCALE / frequency);
               
        // enable interrupts for _timerGroup, _timerIndex
        timer_enable_intr(_timerGroup, _timerIndex);
        
        _callback = callback;
        
        // Register the ISR handler       
        // If the intr_alloc_flags value ESP_INTR_FLAG_IRAM is set, the handler function must be declared with IRAM_ATTR attribute
        // and can only call functions in IRAM or ROM. It cannot call other timer APIs.
       //timer_isr_register(_timerGroup, _timerIndex, _callback, (void *) (uint32_t) _timerNo, ESP_INTR_FLAG_IRAM, NULL);
        timer_isr_callback_add(_timerGroup, _timerIndex, _callback, (void *) (uint32_t) _timerNo, 0);

        timer_start(_timerGroup, _timerIndex);
  
        return true;
      }
      else
      {   
        TISR_LOGERROR(F("Error. Timer must be 0-1"));

        return false;
      }
    }

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to esp32-hal-timer.c
    bool setInterval(const unsigned long& interval, esp32_timer_callback callback)
    {
      return setFrequency((float) (1000000.0f / interval), callback);
    }

    bool attachInterrupt(const float& frequency, esp32_timer_callback callback)
    {
      return setFrequency(frequency, callback);
    }

    // interval (in microseconds) and duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    // No params and duration now. To be addes in the future by adding similar functions here or to esp32-hal-timer.c
    bool attachInterruptInterval(const unsigned long& interval, esp32_timer_callback callback)
    {
      return setFrequency( (float) ( 1000000.0f / interval), callback);
    }

    void detachInterrupt()
    { 
      timer_group_intr_disable(_timerGroup, TIMER_INTR_T0);
    }

    void disableTimer()
    {  
      timer_group_intr_disable(_timerGroup, TIMER_INTR_T0);
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void reattachInterrupt()
    { 
      timer_group_intr_enable(_timerGroup, TIMER_INTR_T0); 
    }

    // Duration (in milliseconds). Duration = 0 or not specified => run indefinitely
    void enableTimer()
    { 
      timer_group_intr_enable(_timerGroup, TIMER_INTR_T0);
    }

    // Just stop clock source, clear the count
    void stopTimer()
    {
      timer_pause(_timerGroup, _timerIndex);
    }

    // Just reconnect clock source, start current count from 0
    void restartTimer()
    {
      timer_set_counter_value(_timerGroup, _timerIndex , 0x00000000ULL);
      timer_start(_timerGroup, _timerIndex);
    }

    int8_t getTimer() __attribute__((always_inline))
    {
      return _timerIndex;
    };
    
    int8_t getTimerGroup() __attribute__((always_inline))
    {
      return _timerGroup;
    };

}; // class ESP32TimerInterrupt

#endif    // ESP32_C3_TIMERINTERRUPT_H

