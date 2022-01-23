/****************************************************************************************************************************
  RPM_Measure.ino
  For ESP32_C3 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP32_C3_TimerInterrupt
  Licensed under MIT license

  The ESP32-C3 has two timer groups, each one with one general purpose hardware timer. All the timers are based on 64 bits
  counters and 16 bit prescalers. The timer counters can be configured to count up or down and support automatic reload
  and software reload. They can also generate alarms when they reach a specific value, defined by the software. The value
  of the counter can be read by the software program.

  Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
  unsigned long miliseconds), you just consume only one ESP32-C3 timer and avoid conflicting with other cores' tasks.
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.
*****************************************************************************************************************************/
/*
   Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.

   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written
*/

#if !( ARDUINO_ESP32C3_DEV )
  #error This code is intended to run on the ESP32_C3 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         1
#define _TIMERINTERRUPT_LOGLEVEL_     0

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ESP32_C3_TimerInterrupt.h"

// Don't use PIN_D1 in core v2.0.0 and v2.0.1. Check https://github.com/espressif/arduino-esp32/issues/5868
#define PIN_D2              2         // Pin D2 mapped to pin GPIO2/ADC12/TOUCH2/LED_BUILTIN of ESP32
#define PIN_D3              3         // Pin D3 mapped to pin GPIO3/RX0 of ESP32
#define PIN_D4              4         // Pin D4 mapped to pin GPIO4/ADC10/TOUCH0 of ESP32

unsigned int SWPin = PIN_D4;

#define TIMER0_INTERVAL_MS        1
#define DEBOUNCING_INTERVAL_MS    80

#define LOCAL_DEBUG               1

// Init ESP32 timer 0
ESP32Timer ITimer0(0);

volatile unsigned long rotationTime = 0;

// Not using float => using RPM = 100 * real RPM
float RPM       = 0;
float avgRPM    = 0;
//uint32_t RPM       = 0;
//uint32_t avgRPM    = 0;

volatile int debounceCounter;

// With core v2.0.0+, you can't use Serial.print/println in ISR or crash.
// and you can't use float calculation inside ISR
// Only OK in core v1.0.6-
bool IRAM_ATTR TimerHandler0(void * timerNo)
{ 
  if ( !digitalRead(SWPin) && (debounceCounter >= DEBOUNCING_INTERVAL_MS / TIMER0_INTERVAL_MS ) )
  {
    //min time between pulses has passed
    // Using float calculation / vars in core v2.0.0 and core v2.0.1 will cause crash
    // Not using float => using RPM = 100 * real RPM
    RPM = ( 6000000 / ( rotationTime * TIMER0_INTERVAL_MS ) );

    avgRPM = ( 2 * avgRPM + RPM) / 3;

    rotationTime = 0;
    debounceCounter = 0;
  }
  else
  {
    debounceCounter++;
  }

  //if (rotationTime >= 5000)
  if (rotationTime >= 1000)
  {
    // If idle, set RPM to 0, don't increase rotationTime
    RPM = 0;

    avgRPM = ( avgRPM + 3 * RPM) / 4;
     
    rotationTime = 0;
  }
  else
  {
    rotationTime++;
  }

  return true;
}

void setup()
{
  pinMode(SWPin, INPUT_PULLUP);
  
  Serial.begin(115200);
  while (!Serial);

  delay(100);
  
  Serial.print(F("\nStarting RPM_Measure on ")); Serial.println(ARDUINO_BOARD);
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

  Serial.flush();   
}

void loop()
{
  if (avgRPM > 0)
  {
    Serial.print(F("RPM  = ")); Serial.print((float) RPM / 100.f); Serial.print(F(", avgRPM  = ")); Serial.println((float) avgRPM / 100.f);
  }

  delay(1000);
}
