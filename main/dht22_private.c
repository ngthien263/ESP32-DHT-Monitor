#include "dht22_private.h"
#include "dht22_types.h"

#ifdef UNIT_TEST
// Mock các hàm hardware khi test
#include <stdint.h>
#include <stdbool.h>

#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_INPUT 0

int64_t esp_timer_get_time() {
    static int64_t fake_time = 0;
    return fake_time += 50;  
}

int gpio_get_level(int pin) {
    return 1;  
}

void gpio_set_direction(int pin, int mode) {}
void gpio_set_level(int pin, int level) {}
void esp_rom_delay_us(int us) {}

#endif

#ifndef UNIT_TEST
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#endif

static int get_signal_level(int timeout_us, bool level) {
    int64_t start_time = esp_timer_get_time();  
    while (gpio_get_level(DHT22_PORT_NUM) == level) {
        if ((esp_timer_get_time() - start_time) > timeout_us) {
            return -1;
        }
    }
    return (int)(esp_timer_get_time() - start_time);
}

dht22_error_t __start_signal(){
    gpio_set_direction(DHT22_PORT_NUM, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT22_PORT_NUM, 0);
    esp_rom_delay_us(5000);
    gpio_set_level(DHT22_PORT_NUM, 1);
    esp_rom_delay_us(40);
    return DHT22_OK;
}

dht22_error_t __wait_respond_signal(){
    gpio_set_direction (DHT22_PORT_NUM, GPIO_MODE_INPUT);
    int uSec = get_signal_level( 85, 0 );
    if( uSec<0 ) return DHT22_ERROR_TIMEOUT; 
    uSec = get_signal_level( 85, 1 );
    if( uSec<0 ) return DHT22_ERROR_TIMEOUT; 
    return DHT22_OK;
}

dht22_error_t __read_data(dht22_data_t* data){
    uint8_t cur_byte = 0;
    uint8_t bytelist[5] = {0};
    uint8_t i = 0;
    int uSec; 
    while (i < 40)
    {
        if(!(i & 7)) {
            bytelist[(i/8) - 1] = cur_byte;
            cur_byte = 0;
        }
        uSec = get_signal_level( 50, 0 );
        if( uSec<0 ) return DHT22_ERROR_TIMEOUT; 
        int64_t time = esp_timer_get_time();
        while(gpio_get_level(DHT22_PORT_NUM)){
            if ((esp_timer_get_time() - time) > 100)   
                return DHT22_ERROR_TIMEOUT;
        }
        cur_byte = (esp_timer_get_time() - time > 30) ? (cur_byte << 1) | (1) :cur_byte << 1;
        i++; 
    }
    bytelist[4] = cur_byte;
    data->humidity = ((bytelist[0] << 8) | bytelist[1]) / 10.0;
    data->temperature = ((bytelist[2] << 8) | bytelist[3]) / 10.0;
    if (((bytelist[0] + bytelist[1] + bytelist[2] + bytelist[3]) & 0xFF) == bytelist[4]){
        return DHT22_OK;
    }
    return DHT22_ERROR_CHECKSUM;
}