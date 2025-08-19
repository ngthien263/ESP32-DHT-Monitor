/**
 * @file wifi.h
 * @brief WiFi setup and initialization API for ESP32.
 *
 * Provides functions to configure ESP32 as Access Point (AP) or Station (STA).
 */

#ifndef WIFI_H
#define WIFI_H

#include "esp_netif.h"
#include "esp_wifi.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct wifi_setup_t
 * @brief Encapsulates WiFi configuration and mode.
 */

typedef struct {
    wifi_config_t wconfig;
    wifi_mode_t mode;
} wifi_setup_t;

/**
 * @brief Initialize ESP32 as a WiFi Access Point.
 * 
 * @param wsetup Pointer to WiFi setup structure.
 */
void wifi_init_ap(wifi_setup_t* wsetup);


/**
 * @brief Initialize ESP32 as a WiFi Station.
 * 
 * @param wsetup Pointer to WiFi setup structure.
 */
void wifi_init_sta(wifi_setup_t* wsetup);

#ifdef __cplusplus
}
#endif

#endif // WIFI_H
