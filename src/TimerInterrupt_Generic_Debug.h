/****************************************************************************************************************************
  TimerInterrupt_Generic_Debug.h
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

  Version: 1.4.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.4.0   K Hoang      29/07/2021 Initial coding. Sync with ESP32_S2_TimerInterrupt v1.4.0
 *****************************************************************************************************************************/

#pragma once

#ifndef TIMERINTERRUPT_GENERIC_DEBUG_H
#define TIMERINTERRUPT_GENERIC_DEBUG_H

#ifdef TIMERINTERRUPT_DEBUG_PORT
  #define TISR_DBG_PORT      TIMERINTERRUPT_DEBUG_PORT
#else
  #define TISR_DBG_PORT      Serial
#endif

// Change _TIMERINTERRUPT_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _TIMERINTERRUPT_LOGLEVEL_
  #define _TIMERINTERRUPT_LOGLEVEL_       1
#endif

#define TISR_LOGERROR(x)         if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.println(x); }
#define TISR_LOGERROR0(x)        if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print(x); }
#define TISR_LOGERROR1(x,y)      if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(y); }
#define TISR_LOGERROR2(x,y,z)    if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(z); }
#define TISR_LOGERROR3(x,y,z,w)  if(_TIMERINTERRUPT_LOGLEVEL_>0) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(z); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(w); }

#define TISR_LOGWARN(x)          if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.println(x); }
#define TISR_LOGWARN0(x)         if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print(x); }
#define TISR_LOGWARN1(x,y)       if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(y); }
#define TISR_LOGWARN2(x,y,z)     if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(z); }
#define TISR_LOGWARN3(x,y,z,w)   if(_TIMERINTERRUPT_LOGLEVEL_>1) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(z); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(w); }

#define TISR_LOGINFO(x)          if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.println(x); }
#define TISR_LOGINFO0(x)         if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print(x); }
#define TISR_LOGINFO1(x,y)       if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(y); }
#define TISR_LOGINFO2(x,y,z)     if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(z); }
#define TISR_LOGINFO3(x,y,z,w)   if(_TIMERINTERRUPT_LOGLEVEL_>2) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(z); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(w); }

#define TISR_LOGDEBUG(x)         if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.println(x); }
#define TISR_LOGDEBUG0(x)        if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print(x); }
#define TISR_LOGDEBUG1(x,y)      if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(y); }
#define TISR_LOGDEBUG2(x,y,z)    if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(z); }
#define TISR_LOGDEBUG3(x,y,z,w)  if(_TIMERINTERRUPT_LOGLEVEL_>3) { TISR_DBG_PORT.print("[TISR] "); TISR_DBG_PORT.print(x); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(y); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.print(z); TISR_DBG_PORT.print(" "); TISR_DBG_PORT.println(w); }

#endif    //TIMERINTERRUPT_GENERIC_DEBUG_H
