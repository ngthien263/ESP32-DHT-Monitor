# ESP32 DHT22 Monitoring System

A modular sensor monitoring system built with ESP-IDF and FreeRTOS, following clean architecture principles and observer design pattern.

---

## 🧱 Architecture Overview

- **Sensor Layer:** DHT22 driver (1-wire protocol) periodically samples temperature and humidity.
- **Subject Layer:** Implements observer pattern (subscribe, notify).
- **Observer Layer:** Includes logger, HTTP client, and optionally LCD display.
- **Communication:** Uses FreeRTOS primitives — `mutex`, `event group`, `queue` — to synchronize data flow.

👉 See [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) for full architecture description.

---

## ⚙️ Build & Run

```bash
idf.py build
idf.py flash monitor

```

##  Testing

Built with **Unity Test Framework**, decoupled from ESP32 hardware using mocks.  

### ✅ Test coverage includes:
- **DHT22 data decoding**: valid/invalid checksum, NaN, boundary values
- **Subject/Observer**: subscribe, notify, unsubscribe, memory safety
- **FreeRTOS mocks**: correct usage of `xTaskNotifyGive`, `xQueueSend`

```bash
cd test
make
./test_dht22.o

```

##  Roadmap

 Secure Wi-Fi (WPA2/TLS, SNTP)

 LCD SPI display (ST7735/7789)

 OTA update (ESP-IDF built-in)
