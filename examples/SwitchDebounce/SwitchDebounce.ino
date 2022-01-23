/****************************************************************************************************************************
  SwitchDebounce.ino
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

//These define's must be placed at the beginning before #include "TimerInterrupt.h"
// Don't define TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG      1

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ESP32_C3_TimerInterrupt.h"

// Don't use PIN_D1 in core v2.0.0 and v2.0.1. Check https://github.com/espressif/arduino-esp32/issues/5868
#define PIN_D2              2         // Pin D2 mapped to pin GPIO2/ADC12/TOUCH2/LED_BUILTIN of ESP32
#define PIN_D4              4         // Pin D4 mapped to pin GPIO4/ADC10/TOUCH0 of ESP32

unsigned int SWPin = PIN_D4;

#define TIMER1_INTERVAL_MS        20
#define DEBOUNCING_INTERVAL_MS    100
#define LONG_PRESS_INTERVAL_MS    5000

// Init ESP32 timer 1
ESP32Timer ITimer1(1);

volatile bool SWPressed     = false;
volatile bool SWLongPressed = false;

volatile uint64_t lastSWPressedTime     = 0;
volatile uint64_t lastSWLongPressedTime = 0;

volatile bool lastSWPressedNoted     = true;
volatile bool lastSWLongPressedNoted = true;

void IRAM_ATTR lastSWPressedMS()
{
  lastSWPressedTime   = millis();
  lastSWPressedNoted  = false;
}

void IRAM_ATTR lastSWLongPressedMS()
{
  lastSWLongPressedTime   = millis();
  lastSWLongPressedNoted  = false;
}

// With core v2.0.0+, you can't use Serial.print/println in ISR or crash.
// and you can't use float calculation inside ISR
// Only OK in core v1.0.6-
bool IRAM_ATTR TimerHandler1(void * timerNo)
{ 
  static unsigned int debounceCountSWPressed  = 0;
  static unsigned int debounceCountSWReleased = 0;

  if ( (!digitalRead(SWPin)) )
  {
    // Start debouncing counting debounceCountSWPressed and clear debounceCountSWReleased
    debounceCountSWReleased = 0;

    if (++debounceCountSWPressed >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS)
    {
      // Call and flag SWPressed
      if (!SWPressed)
      {
        SWPressed = true;
        // Do something for SWPressed here in ISR
        // But it's better to use outside software timer to do your job instead of inside ISR
        //Your_Response_To_Press();
        lastSWPressedMS();
      }

      if (debounceCountSWPressed >= LONG_PRESS_INTERVAL_MS / TIMER1_INTERVAL_MS)
      {
        // Call and flag SWLongPressed
        if (!SWLongPressed)
        {
          SWLongPressed = true;
          // Do something for SWLongPressed here in ISR
          // But it's better to use outside software timer to do your job instead of inside ISR
          //Your_Response_To_Long_Press();
          lastSWLongPressedMS();
        }
      }
    }
  }
  else
  {
    // Start debouncing counting debounceCountSWReleased and clear debounceCountSWPressed
    if ( SWPressed && (++debounceCountSWReleased >= DEBOUNCING_INTERVAL_MS / TIMER1_INTERVAL_MS))
    {
      SWPressed     = false;
      SWLongPressed = false;

      // Do something for !SWPressed here in ISR
      // But it's better to use outside software timer to do your job instead of inside ISR
      //Your_Response_To_Release();

      // Call and flag SWPressed
      debounceCountSWPressed = 0;
    }
  }

  return true;
}

void setup()
{
  pinMode(SWPin, INPUT_PULLUP);
  
  Serial.begin(115200);
  while (!Serial);

  delay(100);
  
  Serial.print(F("\nStarting SwitchDebounce on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP32_C3_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Using ESP32  => 80 / 160 / 240MHz CPU clock ,
  // For 64-bit timer counter
  // For 16-bit timer prescaler up to 1024

  // Interval in microsecs
  if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
  {
    Serial.print(F("Starting  ITimer1 OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer1. Select another freq. or timer"));
}

void loop()
{
  if (!lastSWPressedNoted)
  {
    lastSWPressedNoted = true;
    Serial.print(F("lastSWPressed @ millis() = ")); Serial.println(lastSWPressedTime);
  }

  if (!lastSWLongPressedNoted)
  {
    lastSWLongPressedNoted = true;
    Serial.print(F("lastSWLongPressed @ millis() = ")); Serial.println(lastSWLongPressedTime);
  }

  delay(500);
}
