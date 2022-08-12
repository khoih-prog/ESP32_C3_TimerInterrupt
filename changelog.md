# ESP32_C3_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP32_C3_TimerInterrupt.svg?)](https://www.ardu-badge.com/ESP32_C3_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP32_C3_TimerInterrupt.svg)](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP32_C3_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP32_C3_TimerInterrupt.svg)](http://github.com/khoih-prog/ESP32_C3_TimerInterrupt/issues)


---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.7.0](#releases-v170)
  * [Releases v1.6.0](#releases-v160)
  * [Releases v1.5.0](#releases-v150)
  * [Releases v1.4.0](#releases-v140)

---
---

## Changelog

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

