#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "dht22.h"

#define DHT22_PORT_NUM  4

void read_sensor1_task(void* pvParameters) {
    dht22_t* sensor = (dht22_t*) pvParameters;
    while (1) {
        dht22_get_data(sensor);
        vTaskDelay(pdMS_TO_TICKS(3000)); 
    }
}

void app_main() {
    nvs_flash_init();  
    dht22_t* sensor1 = dht22_create();
    xTaskCreate(read_sensor1_task, "read_sensor1", 2048, sensor1, 5, NULL);
}
