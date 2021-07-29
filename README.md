# ESP32_C3_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_C3_TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32_C3_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32_C3_TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32_C3_TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32_C3_TimerInterrupt/issues)

---
---

## Table of Contents

* [Why do we need this ESP32_C3_TimerInterrupt library](#why-do-we-need-this-ESP32_C3_TimerInterrupt-library)
  * [Features](#features)
  * [Why using ISR-based Hardware Timer Interrupt is better](#why-using-isr-based-hardware-timer-interrupt-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](#changelog)
  * [Releases v1.4.0](#releases-v140)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Note for Platform IO using ESP32 LittleFS](#note-for-platform-io-using-esp32-littlefs)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)](#howto-use-analogread-with-esp32-running-wifi-andor-bluetooth-btble)
  * [1. ESP32 has 2 ADCs, named ADC1 and ADC2](#1--esp32-has-2-adcs-named-adc1-and-adc2)
  * [2. ESP32 ADCs functions](#2-esp32-adcs-functions)
  * [3. ESP32 WiFi uses ADC2 for WiFi functions](#3-esp32-wifi-uses-adc2-for-wifi-functions)
* [More useful Information](#more-useful-information)
* [How to use](#how-to-use)
* [Examples](#examples)
  * [ 1. Argument_None](examples/Argument_None)
  * [ 2. Change_Interval](examples/Change_Interval).
  * [ 3. ISR_RPM_Measure](examples/ISR_RPM_Measure)
  * [ 4. RPM_Measure](examples/RPM_Measure)
  * [ 5. SwitchDebounce](examples/SwitchDebounce)
  * [ 6. TimerInterruptTest](examples/TimerInterruptTest)
  * [ 7. ISR_16_Timers_Array](examples/ISR_16_Timers_Array)
  * [ 8. ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex)
* [Example TimerInterruptTest](#example-timerinterrupttest)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_Timers_Array_Complex on ESP32C3_DEV](#1-isr_16_timers_array_complex-on-esp32c3_dev)
  * [2. TimerInterruptTest on ESP32C3_DEV](#2-timerinterrupttest-on-esp32c3_dev)
  * [3. Change_Interval on ESP32C3_DEV](#3-change_interval-on-esp32c3_dev)
  * [4. Argument_None on ESP32C3_DEV](#4-argument_none-on-esp32c3_dev)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Releases](#releases)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [ESP32_C3_TimerInterrupt library](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt)

## Features

This library enables you to use Interrupt from Hardware Timers on an ESP32_C3-based board.

The ESP32_C3 timer Interrupt control is different from that of ESP32, at least with the ESP32_C3 core v2.0.0-rc1. While the examples of current [ESP32TimerInterrupt library](https://github.com/khoih-prog/ESP32TimerInterrupt) can be compiled OK, they can't run yet. That's why this new [ESP32_C3_TimerInterrupt library](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt) has been created.

---

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based Timers, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based timers. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_Timer_Complex**](examples/ISR_Timer_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

### Why using ISR-based Hardware Timer Interrupt is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. ESP32_C3-based boards, such as ESP32C3_DEV, etc

---

### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.


---
---

## Changelog

### Releases v1.4.0

1. Initial coding to support ESP32_C3
2. Sync with [ESP32_S2_TimerInterrupt library v1.4.0](https://github.com/khoih-prog/ESP32_S2_TimerInterrupt)


---
---

## Prerequisites

1. [`Arduino IDE 1.8.15+` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`ESP32 Core 2.0.0-rc1+`](https://github.com/espressif/arduino-esp32) for ESP32_C3-based boards.

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**ESP32_C3_TimerInterrupt**](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_C3_TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32_C3_TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**ESP32_C3_TimerInterrupt**](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt) page.
2. Download the latest release `ESP32_C3_TimerInterrupt-master.zip`.
3. Extract the zip file to `ESP32_C3_TimerInterrupt-master` directory 
4. Copy whole `ESP32_C3_TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**ESP32_C3_TimerInterrupt** library](https://platformio.org/lib/show/12622/ESP32_C3_TimerInterrupt) by using [Library Manager](https://platformio.org/lib/show/12622/ESP32_C3_TimerInterrupt/installation). Search for **ESP32_C3_TimerInterrupt** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### Note for Platform IO using ESP32 LittleFS

#### Necessary only for esp32 core v1.0.6-

From esp32 core v1.0.6+, [`LittleFS_esp32 v1.0.6`](https://github.com/lorol/LITTLEFS) has been included and this step is not necessary anymore.

In Platform IO, to fix the error when using [`LittleFS_esp32 v1.0`](https://github.com/lorol/LITTLEFS) for ESP32-based boards with ESP32 core v1.0.4- (ESP-IDF v3.2-), uncomment the following line

from

```
//#define CONFIG_LITTLEFS_FOR_IDF_3_2   /* For old IDF - like in release 1.0.4 */
```

to

```
#define CONFIG_LITTLEFS_FOR_IDF_3_2   /* For old IDF - like in release 1.0.4 */
```

It's advisable to use the latest [`LittleFS_esp32 v1.0.5+`](https://github.com/lorol/LITTLEFS) to avoid the issue.

Thanks to [Roshan](https://github.com/solroshan) to report the issue in [Error esp_littlefs.c 'utime_p'](https://github.com/khoih-prog/ESPAsync_WiFiManager/issues/28) 

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using **xyz-Impl.h instead of standard xyz.cpp**, possibly creates certain `Multiple Definitions` Linker error in certain use cases. Although it's simple to just modify several lines of code, either in the library or in the application, the library is adding 2 more source directories

1. **scr_h** for new h-only files
2. **src_cpp** for standard h/cpp files

besides the standard **src** directory.

To use the **old standard cpp** way, locate this library' directory, then just 

1. **Delete the all the files in src directory.**
2. **Copy all the files in src_cpp directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

To re-use the **new h-only** way, just 

1. **Delete the all the files in src directory.**
2. **Copy the files in src_h directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

---
---

### HOWTO Use analogRead() with ESP32 running WiFi and/or BlueTooth (BT/BLE)

Please have a look at [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to have more detailed description and solution of the issue.

#### 1.  ESP32 has 2 ADCs, named ADC1 and ADC2

#### 2. ESP32 ADCs functions

- ADC1 controls ADC function for pins **GPIO32-GPIO39**
- ADC2 controls ADC function for pins **GPIO0, 2, 4, 12-15, 25-27**

#### 3.. ESP32 WiFi uses ADC2 for WiFi functions

Look in file [**adc_common.c**](https://github.com/espressif/esp-idf/blob/master/components/driver/adc_common.c#L61)

> In ADC2, there're two locks used for different cases:
> 1. lock shared with app and Wi-Fi:
>    ESP32:
>         When Wi-Fi using the ADC2, we assume it will never stop, so app checks the lock and returns immediately if failed.
>    ESP32S2:
>         The controller's control over the ADC is determined by the arbiter. There is no need to control by lock.
> 
> 2. lock shared between tasks:
>    when several tasks sharing the ADC2, we want to guarantee
>    all the requests will be handled.
>    Since conversions are short (about 31us), app returns the lock very soon,
>    we use a spinlock to stand there waiting to do conversions one by one.
> 
> adc2_spinlock should be acquired first, then adc2_wifi_lock or rtc_spinlock.


- In order to use ADC2 for other functions, we have to **acquire complicated firmware locks and very difficult to do**
- So, it's not advisable to use ADC2 with WiFi/BlueTooth (BT/BLE).
- Use ADC1, and pins GPIO32-GPIO39
- If somehow it's a must to use those pins serviced by ADC2 (**GPIO0, 2, 4, 12, 13, 14, 15, 25, 26 and 27**), use the **fix mentioned at the end** of [**ESP_WiFiManager Issue 39: Not able to read analog port when using the autoconnect example**](https://github.com/khoih-prog/ESP_WiFiManager/issues/39) to work with ESP32 WiFi/BlueTooth (BT/BLE).

---
---

## More useful Information

The ESP32_C3 has two timer groups, each one with only one general purpose hardware timer.  All the timers are based on 64 bits counters and 16 bit prescalers. 

The timer counters can be configured to count up or down and support automatic reload and software reload.

They can also generate alarms when they reach a specific value, defined by the software. The value of the counter can be read by 
the software program.

---

Now with these new `16 ISR-based timers` (while consuming only **1 hardware timer**), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The `ISR_Timer_Complex` example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 
You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---
---

## How to use

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

`Timer0, Timer1` are supported for ESP32_C3.

Using as follows:

```

#include "ESP32_C3_TimerInterrupt.h"

// Init ESP32_C3 timer 0 and 1
ESP32Timer ITimer0(0);
ESP32Timer ITimer1(1);

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
  delay(100);
  ...

  // Using ESP32  => 80 / 160 / 240MHz CPU clock ,
  // For 64-bit timer counter
  // For 16-bit timer prescaler up to 1024

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
  {
    Serial.print(F("Starting  ITimer0 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}
```

---
---

### Examples: 

1. [Argument_None](examples/Argument_None)
2. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
3. [RPM_Measure](examples/RPM_Measure)
4. [SwitchDebounce](examples/SwitchDebounce)
5. [TimerInterruptTest](examples/TimerInterruptTest)
6. [**Change_Interval**](examples/Change_Interval).
7. [**ISR_16_Timers_Array**](examples/ISR_16_Timers_Array)
8. [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex).

---
---

### Example [TimerInterruptTest](examples/TimerInterruptTest)

```
#if !( ARDUINO_ESP32C3_DEV )
  #error This code is intended to run on the ESP32_C3 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP32_C3_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can crash or hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     1

#include "ESP32_C3_TimerInterrupt.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2         // Pin D2 mapped to pin GPIO2/ADC12 of ESP32, control on-board LED
#endif

#define PIN_D8              8         // Pin D8 mapped to pin GPIO8 of ESP32_C3

void IRAM_ATTR TimerHandler0(void * timerNo)
{
  /////////////////////////////////////////////////////////
  // Always call this for ESP32-C3 before processing ISR
  TIMER_ISR_START(timerNo);
  /////////////////////////////////////////////////////////
  
  static bool toggle0 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(LED_BUILTIN, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("ITimer0 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles pin LED_BUILTIN
  digitalWrite(LED_BUILTIN, toggle0);
  toggle0 = !toggle0;

  /////////////////////////////////////////////////////////
  // Always call this for ESP32-C3 after processing ISR
  TIMER_ISR_END(timerNo);
  /////////////////////////////////////////////////////////
}

void IRAM_ATTR TimerHandler1(void * timerNo)
{
  /////////////////////////////////////////////////////////
  // Always call this for ESP32-C3 before processing ISR
  TIMER_ISR_START(timerNo);
  /////////////////////////////////////////////////////////
  
  static bool toggle1 = false;
  static bool started = false;

  if (!started)
  {
    started = true;
    pinMode(PIN_D8, OUTPUT);
  }

#if (TIMER_INTERRUPT_DEBUG > 0)
  Serial.print("ITimer1 called, millis() = "); Serial.println(millis());
#endif

  //timer interrupt toggles outputPin
  digitalWrite(PIN_D8, toggle1);
  toggle1 = !toggle1;

  /////////////////////////////////////////////////////////
  // Always call this for ESP32-C3 after processing ISR
  TIMER_ISR_END(timerNo);
  /////////////////////////////////////////////////////////
}

#define TIMER0_INTERVAL_MS        1000
#define TIMER0_DURATION_MS        5000

#define TIMER1_INTERVAL_MS        3000
#define TIMER1_DURATION_MS        15000

// Init ESP32 timer 0 and 1
ESP32Timer ITimer0(0);
ESP32Timer ITimer1(1);

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
  delay(100);
  
  Serial.print(F("\nStarting TimerInterruptTest on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_C3_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Using ESP32  => 80 / 160 / 240MHz CPU clock ,
  // For 64-bit timer counter
  // For 16-bit timer prescaler up to 1024

  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
  {
    Serial.print(F("Starting  ITimer0 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));

  Serial.flush();  
}

void loop()
{
  static unsigned long lastTimer0 = 0;
  static unsigned long lastTimer1 = 0;

  static bool timer0Stopped         = false;
  static bool timer1Stopped         = false;

  if (millis() - lastTimer0 > TIMER0_DURATION_MS)
  {
    lastTimer0 = millis();

    if (timer0Stopped)
    {
      Serial.print(F("Start ITimer0, millis() = ")); Serial.println(millis());
      ITimer0.restartTimer();
    }
    else
    {
      Serial.print(F("Stop ITimer0, millis() = ")); Serial.println(millis());
      ITimer0.stopTimer();
    }
    timer0Stopped = !timer0Stopped;
  }

  if (millis() - lastTimer1 > TIMER1_DURATION_MS)
  {
    lastTimer1 = millis();

    if (timer1Stopped)
    {
      Serial.print(F("Start ITimer1, millis() = ")); Serial.println(millis());
      ITimer1.restartTimer();
    }
    else
    {
      Serial.print(F("Stop ITimer1, millis() = ")); Serial.println(millis());
      ITimer1.stopTimer();
    }
    
    timer1Stopped = !timer1Stopped;
  }
}
```
---
---

### Debug Terminal Output Samples

### 1. ISR_16_Timers_Array_Complex on ESP32C3_DEV

The following is the sample terminal output when running example [ISR_16_Timers_Array_Complex](examples/ISR_16_Timers_Array_Complex) on **ESP32C3_DEV** to demonstrate of ISR Hardware Timer, especially when system is very busy or blocked. The 16 independent ISR timers are programmed to be activated repetitively after certain intervals, is activated exactly after that programmed interval !!!

```
Starting ISR_16_Timers_Array_Complex on ESP32C3_DEV
ESP32_C3_TimerInterrupt v1.4.0
CPU Frequency = 160 MHz
Starting ITimer OK, millis() = 2187
SimpleTimer : 2, ms : 12193, Dms : 10005
Timer : 0, programmed : 5000, actual : 5009
Timer : 1, programmed : 10000, actual : 0
Timer : 2, programmed : 15000, actual : 0
Timer : 3, programmed : 20000, actual : 0
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 22251, Dms : 10058
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15009
Timer : 3, programmed : 20000, actual : 20009
Timer : 4, programmed : 25000, actual : 0
Timer : 5, programmed : 30000, actual : 0
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 32310, Dms : 10059
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20009
Timer : 4, programmed : 25000, actual : 25009
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 0
Timer : 7, programmed : 40000, actual : 0
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 42369, Dms : 10059
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25009
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 0
Timer : 9, programmed : 50000, actual : 0
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 52429, Dms : 10060
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30009
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 0
Timer : 11, programmed : 60000, actual : 0
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 62490, Dms : 10061
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35009
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 0
Timer : 13, programmed : 70000, actual : 0
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 72551, Dms : 10061
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40009
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65009
Timer : 13, programmed : 70000, actual : 70009
Timer : 14, programmed : 75000, actual : 0
Timer : 15, programmed : 80000, actual : 0
SimpleTimer : 2, ms : 82613, Dms : 10062
Timer : 0, programmed : 5000, actual : 5000
Timer : 1, programmed : 10000, actual : 10000
Timer : 2, programmed : 15000, actual : 15000
Timer : 3, programmed : 20000, actual : 20000
Timer : 4, programmed : 25000, actual : 25000
Timer : 5, programmed : 30000, actual : 30000
Timer : 6, programmed : 35000, actual : 35000
Timer : 7, programmed : 40000, actual : 40000
Timer : 8, programmed : 45000, actual : 45009
Timer : 9, programmed : 50000, actual : 50009
Timer : 10, programmed : 55000, actual : 55009
Timer : 11, programmed : 60000, actual : 60009
Timer : 12, programmed : 65000, actual : 65009
Timer : 13, programmed : 70000, actual : 70009
Timer : 14, programmed : 75000, actual : 75009
Timer : 15, programmed : 80000, actual : 80009
```

---

### 2. TimerInterruptTest on ESP32C3_DEV

The following is the sample terminal output when running example [TimerInterruptTest](examples/TimerInterruptTest) to demonstrate how to start/stop Hardware Timers.

```
Starting TimerInterruptTest on ESP32C3_DEV
ESP32_C3_TimerInterrupt v1.4.0
CPU Frequency = 160 MHz
ITimer0 called, millis() = 287
Starting  ITimer0 OK, millis() = 290
ITimer1 called, millis() = 293
Starting  ITimer1 OK, millis() = 296
ITimer0 called, millis() = 1287
ITimer0 called, millis() = 2287
ITimer0 called, millis() = 3287
ITimer1 called, millis() = 3290
ITimer0 called, millis() = 4287
Stop ITimer0, millis() = 5001
ITimer1 called, millis() = 6290
ITimer1 called, millis() = 9290
Start ITimer0, millis() = 10002
ITimer0 called, millis() = 11002
ITimer0 called, millis() = 12002
ITimer1 called, millis() = 12290
ITimer0 called, millis() = 13002
ITimer0 called, millis() = 14002
Stop ITimer1, millis() = 15001
ITimer0 called, millis() = 15002
Stop ITimer0, millis() = 15003
Start ITimer0, millis() = 20004
ITimer0 called, millis() = 21004
ITimer0 called, millis() = 22004
ITimer0 called, millis() = 23004
ITimer0 called, millis() = 24004
ITimer0 called, millis() = 25004
Stop ITimer0, millis() = 25005
Start ITimer1, millis() = 30002
Start ITimer0, millis() = 30006
ITimer0 called, millis() = 31006
ITimer0 called, millis() = 32006
ITimer1 called, millis() = 33002
ITimer0 called, millis() = 33006
ITimer0 called, millis() = 34006
ITimer0 called, millis() = 35006
Stop ITimer0, millis() = 35007
ITimer1 called, millis() = 36002
ITimer1 called, millis() = 39002
Start ITimer0, millis() = 40008
```

---


### 3. Change_Interval on ESP32C3_DEV

The following is the sample terminal output when running example [Change_Interval](examples/Change_Interval) to demonstrate how to change Timer Interval on-the-fly

```
Starting Change_Interval on ESP32C3_DEV
ESP32_C3_TimerInterrupt v1.4.0
CPU Frequency = 160 MHz
ITimer0: millis() = 287
Starting  ITimer0 OK, millis() = 289
ITimer1: millis() = 291
Starting  ITimer1 OK, millis() = 294
ITimer0: millis() = 2287
ITimer0: millis() = 4287
ITimer1: millis() = 5289
ITimer0: millis() = 6287
ITimer0: millis() = 8287
Time = 10001, Timer0Count = 5, Timer1Count = 2
ITimer0: millis() = 10287
ITimer1: millis() = 10289
ITimer0: millis() = 12287
ITimer0: millis() = 14287
ITimer1: millis() = 15289
ITimer0: millis() = 16287
ITimer0: millis() = 18287
Time = 20002, Timer0Count = 10, Timer1Count = 4
Changing Interval, Timer0 = 4000,  Timer1 = 10000
ITimer0: millis() = 24002
ITimer0: millis() = 28002
ITimer1: millis() = 30002
Time = 30003, Timer0Count = 12, Timer1Count = 5
ITimer0: millis() = 32002
ITimer0: millis() = 36002
ITimer0: millis() = 40002
ITimer1: millis() = 40002
Time = 40004, Timer0Count = 15, Timer1Count = 6
Changing Interval, Timer0 = 2000,  Timer1 = 5000
ITimer0: millis() = 42004
ITimer0: millis() = 44004
ITimer1: millis() = 45004
ITimer0: millis() = 46004
ITimer0: millis() = 48004
ITimer0: millis() = 50004
ITimer1: millis() = 50004
Time = 50005, Timer0Count = 20, Timer1Count = 8
ITimer0: millis() = 52004
ITimer0: millis() = 54004
ITimer1: millis() = 55004
ITimer0: millis() = 56004
ITimer0: millis() = 58004
ITimer0: millis() = 60004
ITimer1: millis() = 60004
Time = 60006, Timer0Count = 25, Timer1Count = 10
Changing Interval, Timer0 = 4000,  Timer1 = 10000
ITimer0: millis() = 64006
ITimer0: millis() = 68006
ITimer1: millis() = 70006
Time = 70007, Timer0Count = 27, Timer1Count = 11
ITimer0: millis() = 72006
ITimer0: millis() = 76006
ITimer0: millis() = 80006
ITimer1: millis() = 80006
Time = 80008, Timer0Count = 30, Timer1Count = 12
Changing Interval, Timer0 = 2000,  Timer1 = 5000
ITimer0: millis() = 82008
ITimer0: millis() = 84008
ITimer1: millis() = 85008
ITimer0: millis() = 86008
ITimer0: millis() = 88008
ITimer0: millis() = 90008
ITimer1: millis() = 90008
Time = 90009, Timer0Count = 35, Timer1Count = 14
ITimer0: millis() = 92008
ITimer0: millis() = 94008
ITimer1: millis() = 95008
ITimer0: millis() = 96008
ITimer0: millis() = 98008
ITimer0: millis() = 100008
ITimer1: millis() = 100008
Time = 100010, Timer0Count = 40, Timer1Count = 16
Changing Interval, Timer0 = 4000,  Timer1 = 10000
ITimer0: millis() = 104010
ITimer0: millis() = 108010
ITimer1: millis() = 110010
Time = 110011, Timer0Count = 42, Timer1Count = 17
ITimer0: millis() = 112010
ITimer0: millis() = 116010
ITimer0: millis() = 120010
ITimer1: millis() = 120010
Time = 120012, Timer0Count = 45, Timer1Count = 18

```

---

### 4. Argument_None on ESP32C3_DEV

The following is the sample terminal output when running example [Argument_None](examples/Argument_None)

```
Starting Argument_None on ESP32C3_DEV
ESP32_C3_TimerInterrupt v1.4.0
CPU Frequency = 160 MHz
[TISR] ESP32_C3_TimerInterrupt: _timerNo = 0 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
ITimer0: millis() = 296
Starting  ITimer0 OK, millis() = 299
[TISR] ESP32_C3_TimerInterrupt: _timerNo = 1 , _fre = 1000000
[TISR] TIMER_BASE_CLK = 80000000 , TIMER_DIVIDER = 80
ITimer1: millis() = 311
Starting  ITimer1 OK, millis() = 315
ITimer0: millis() = 1294
ITimer0: millis() = 2294
ITimer0: millis() = 3294
ITimer0: millis() = 4294
ITimer0: millis() = 5294
ITimer1: millis() = 5310
ITimer0: millis() = 6294
ITimer0: millis() = 7294
ITimer0: millis() = 8294
ITimer0: millis() = 9294
ITimer0: millis() = 10294
ITimer1: millis() = 10310
ITimer0: millis() = 11294
ITimer0: millis() = 12294
ITimer0: millis() = 13294
ITimer0: millis() = 14294
ITimer0: millis() = 15294
ITimer1: millis() = 15310
ITimer0: millis() = 16294
ITimer0: millis() = 17294
ITimer0: millis() = 18294
ITimer0: millis() = 19294
ITimer0: millis() = 20294
ITimer1: millis() = 20310
ITimer0: millis() = 21294
ITimer0: millis() = 22294
ITimer0: millis() = 23294
ITimer0: millis() = 24294
ITimer0: millis() = 25294
ITimer1: millis() = 25310
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level (_TIMERINTERRUPT_LOGLEVEL_) from 0 to 4

```cpp
// These define's must be placed at the beginning before #include "ESP32_C3_TimerInterrupt.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

## Releases

### Releases v1.4.0

1. Initial coding to support ESP32_C3
2. Sync with [ESP32_S2_TimerInterrupt library v1.4.0](https://github.com/khoih-prog/ESP32_S2_TimerInterrupt)


---
---

### Issues

Submit issues to: [ESP32_C3_TimerInterrupt issues](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/issues)

---

## TO DO

1. Search for bug and improvement.



## DONE

1. Basic hardware timers for ESP32_C3.
2. More hardware-initiated software-enabled timers
3. Longer time interval
4. Similar features for remaining Arduino boards such as SAMD21, SAMD51, SAM-DUE, nRF52, ESP8266, STM32, etc.
5. Fix compiler errors due to conflict to some libraries.
6. Add complex examples.

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/blob/master/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


