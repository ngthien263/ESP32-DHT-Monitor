# ESP32 DHT22 Monitoring System

A modular sensor monitoring system built with ESP-IDF and FreeRTOS, following clean architecture principles and observer design pattern.

---

## 🧱 Architecture Overview

- **Subject Layer:** Implements interface for subject (eg: sensor, ...).
- **Observer Layer:** Implements interface for observer (eg: HTTP client, LCD display, ... ).
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
- **Subject/Observer**: subscribe, notify, unsubscribe, memory safety
- **FreeRTOS mocks**: correct usage of `xTaskNotifyGive`, `xQueueSend`

```bash
cd test
make
./test_dht22.o

```

