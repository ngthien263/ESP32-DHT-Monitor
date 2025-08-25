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