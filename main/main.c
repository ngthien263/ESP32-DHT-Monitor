// /**
//  * @file main.c
//  * @brief Entry point of ESP32 DHT22 monitoring application.
//  *
//  * This application demonstrates:
//  * - Reading data from DHT22 sensor.
//  * - Logging data to console.
//  * - Sending sensor data via HTTP client.
//  * - Running in FreeRTOS tasks with observer–notify pattern.
//  */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "dht22.h"
#include "wifi.h"
#include "http_client.h"
#define DHT22_PORT_NUM  4


void app_main() {  
   subject_t* sensor1 = dht22_create();
   http_client_t http_client = {0}; 
   observer_set_subject(&http_client.base_observer, sensor1);
   http_client_init(&http_client, "http://192.168.4.2:8000/sensor");
   http_client_subscribe(&http_client);
}