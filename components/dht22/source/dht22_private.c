#include <stdint.h>
#include "dht22_private.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"

/**
 * @brief Measure how long the GPIO stays at the specified level.
 *
 * Measure the duration (in µs) that the data line remains at `level`. 
 *
 * @param timeout_us Maximum time to wait (in microseconds)
 * @param level The expected GPIO level to measure (0 or 1)
 * @return Duration in µs if successful
 *         -1 if timeout occurred.
 */
static int get_signal_level(int timeout_us, bool level) {
    int64_t start_time = esp_timer_get_time();  
    while (gpio_get_level(DHT22_PORT_NUM) == level) {
        if ((esp_timer_get_time() - start_time) > timeout_us) {
            return -1;
        }
    }
    return (int)(esp_timer_get_time() - start_time);
}


/**
 * @brief Send start signal to the DHT22 sensor
 *
 * Pulls the data line LOW for >1ms, then HIGH 20-40us to wait for DHT22's response
 *
 * @return DHT22_OK: Success
 */
dht22_error_t __start_signal(){
    gpio_set_direction(DHT22_PORT_NUM, GPIO_MODE_OUTPUT);
    gpio_set_level(DHT22_PORT_NUM, 0);
    esp_rom_delay_us(2000);
    gpio_set_level(DHT22_PORT_NUM, 1);
    esp_rom_delay_us(40);
    return DHT22_OK;
}

/**
 * @brief Wait for response signal from DHT22.
 *
 * Waits for DHT22 respond: LOW (~80us) then HIGH (~80us).
 *
 * @return DHT22_OK: Success
 *         DHT22_ERROR_TIMEOUT: The signal was not received in time
 */

dht22_error_t __wait_respond_signal(){
    gpio_set_direction (DHT22_PORT_NUM, GPIO_MODE_INPUT);
    int uSec = get_signal_level( 85, 0 );
    if( uSec<0 ) return DHT22_ERROR_TIMEOUT; 
    uSec = get_signal_level( 85, 1 );
    if( uSec<0 ) return DHT22_ERROR_TIMEOUT; 
    return DHT22_OK;
}

/**
 * @brief Read raw 40-bit data from the DHT22 sensor.
 *
 *
 * Each bit starts with ~50µs LOW, followed by:
 * - ~26–28µs HIGH = 0
 * - ~70µs HIGH    = 1
 *
 * After reading all 40 bits, extracts humidity,
 * temperature, and verifies the checksum.
 *
 * @param[out] data Output structure for humidity and temperature.
 * @return DHT22_OK if checksum is correct, or error code.
 */

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