/****************************************************************************************************************************
  TimerInterruptTest.ino
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

// These define's must be placed at the beginning before #include "TimerInterrupt_Generic.h"
// _TIMERINTERRUPT_LOGLEVEL_ from 0 to 4
// Don't define _TIMERINTERRUPT_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     4

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ESP32_C3_TimerInterrupt.h"

// Don't use PIN_D1 in core v2.0.0 and v2.0.1. Check https://github.com/espressif/arduino-esp32/issues/5868
// Don't use PIN_D2 with ESP32_C3 (crash)
#define PIN_D19             19        // Pin D19 mapped to pin GPIO9 of ESP32
#define PIN_D3              3         // Pin D3 mapped to pin GPIO3/RX0 of ESP32

// With core v2.0.0+, you can't use Serial.print/println in ISR or crash.
// and you can't use float calculation inside ISR
// Only OK in core v1.0.6-
bool IRAM_ATTR TimerHandler0(void * timerNo)
{
	static bool toggle0 = false;

	//timer interrupt toggles pin PIN_D19
	digitalWrite(PIN_D19, toggle0);
	toggle0 = !toggle0;

	return true;
}

bool IRAM_ATTR TimerHandler1(void * timerNo)
{
	static bool toggle1 = false;

	//timer interrupt toggles outputPin
	digitalWrite(PIN_D3, toggle1);
	toggle1 = !toggle1;

	return true;
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
	pinMode(PIN_D19, OUTPUT);
	pinMode(PIN_D3, OUTPUT);

	Serial.begin(115200);

	while (!Serial && millis() < 5000);

  delay(500);

	Serial.print(F("\nStarting TimerInterruptTest on "));
	Serial.println(ARDUINO_BOARD);
	Serial.println(ESP32_C3_TIMER_INTERRUPT_VERSION);
	Serial.print(F("CPU Frequency = "));
	Serial.print(F_CPU / 1000000);
	Serial.println(F(" MHz"));

	// Using ESP32  => 80 / 160 / 240MHz CPU clock ,
	// For 64-bit timer counter
	// For 16-bit timer prescaler up to 1024

	// Interval in microsecs
	if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
	{
		Serial.print(F("Starting  ITimer0 OK, millis() = "));
		Serial.println(millis());
	}
	else
		Serial.println(F("Can't set ITimer0. Select another freq. or timer"));

	// Interval in microsecs
	if (ITimer1.attachInterruptInterval(TIMER1_INTERVAL_MS * 1000, TimerHandler1))
	{
		Serial.print(F("Starting  ITimer1 OK, millis() = "));
		Serial.println(millis());
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
			Serial.print(F("Start ITimer0, millis() = "));
			Serial.println(millis());
			ITimer0.restartTimer();
		}
		else
		{
			Serial.print(F("Stop ITimer0, millis() = "));
			Serial.println(millis());
			ITimer0.stopTimer();
		}

		timer0Stopped = !timer0Stopped;
	}

	if (millis() - lastTimer1 > TIMER1_DURATION_MS)
	{
		lastTimer1 = millis();

		if (timer1Stopped)
		{
			Serial.print(F("Start ITimer1, millis() = "));
			Serial.println(millis());
			ITimer1.restartTimer();
		}
		else
		{
			Serial.print(F("Stop ITimer1, millis() = "));
			Serial.println(millis());
			ITimer1.stopTimer();
		}

		timer1Stopped = !timer1Stopped;
	}
}
