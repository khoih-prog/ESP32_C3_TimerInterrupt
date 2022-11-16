# ESP32_C3_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_C3_TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32_C3_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32_C3_TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32_C3_TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32_C3_TimerInterrupt/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>
<a href="https://profile-counter.glitch.me/khoih-prog/count.svg" title="Total khoih-prog Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog/count.svg" style="height: 30px;width: 200px;"></a>
<a href="https://profile-counter.glitch.me/khoih-prog-ESP32_C3_TimerInterrupt/count.svg" title="ESP32_C3_TimerInterrupt Visitor count"><img src="https://profile-counter.glitch.me/khoih-prog-ESP32_C3_TimerInterrupt/count.svg" style="height: 30px;width: 200px;"></a>

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.8.0](#releases-v180)
  * [Releases v1.7.0](#releases-v170)
  * [Releases v1.6.0](#releases-v160)
  * [Releases v1.5.0](#releases-v150)
  * [Releases v1.4.0](#releases-v140)

---
---

## Changelog

### Releases v1.8.0

1. Fix doubled time for `ESP32_C3`. Check [Error in the value defined by TIMER0_INTERVAL_MS #28](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/issues/28)
2. Modify examples to avoid using `LED_BUILTIN` / `GPIO2`, `GPIO1` as it can cause crash in some boards, such as `ESP32_C3`
3. Use `allman astyle` and add `utils`

### Releases v1.7.0

1. Add support to 
  - ESP32_C3 : LOLIN_C3_MINI, DFROBOT_BEETLE_ESP32_C3, ADAFRUIT_QTPY_ESP32C3, AirM2M_CORE_ESP32C3, XIAO_ESP32C3
  
### Releases v1.6.0

1. Suppress errors and warnings for new ESP32 core v2.0.4+

### Releases v1.5.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
2. Avoid deprecated functions.
3. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project.
4. Optimize library code by using `reference-passing` instead of `value-passing`
5. Update all examples


### Releases v1.4.0

1. Initial coding to support ESP32_C3
2. Sync with [ESP32_S2_TimerInterrupt library v1.4.0](https://github.com/khoih-prog/ESP32_S2_TimerInterrupt)

