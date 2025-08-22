/**
 * @file dht22.c
 * @brief Implementation of the DHT22 driver with Subject/Observer support.
 *
 * This file provides the concrete implementation of the DHT22 sensor driver,
 * including: Subject interface operations (init, cleanup, subscribe, unsubscribe, notify, update).
 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "esp_log.h"
#include "dht22.h"
#include "dht22_private.h"
#include "common_subject.h"  

static const char *TAG = "DHT22 SENSOR";

/**
 * @brief Forward declarations of Subject operation callbacks.
 */

static void dht22_init(void* self);
static void dht22_cleanup(void* self);
static void dht22_subscribe(void* self, void* client, subject_notify_callback_t cb);
static void dht22_unsubscribe(void* self, subject_notify_callback_t cb);
static void dht22_notify(void* self);
static void dht22_update_data_generic(void* self, void* out);


/**
 * @brief Subject operation table for the DHT22 driver.
 *
 * Maps Subject interface methods to DHT22-specific implementations.
 */
static const subject_ops_t dht22_ops = {
    .init = dht22_init,
    .cleanup = dht22_cleanup,
    .subscribe = dht22_subscribe,
    .unsubscribe = dht22_unsubscribe,
    .notify = dht22_notify,
    .update_data = dht22_update_data_generic
};

/**
 * @brief Initialize a DHT22 instance.
 *
 * Initilized data = 0, assigned interface operations, assisgned dht22 pointer to self, clears subscriber slots.
 *
 * @param[in,out] self_ptr Pointer to dht22_t instance.
 */

static void dht22_init(void* self_ptr) {
    dht22_t* self = (dht22_t*)self_ptr;
    self->data.temperature = 0;
    self->data.humidity = 0;
    self->subject.ops = &dht22_ops;
    self->subject.self = self;
    self->mtx = xSemaphoreCreateMutex();
    memset(self->subscribers, 0, sizeof(self->subscribers));
}

/**
 * @brief Cleanup a DHT22 instance and release its resources.
 *
 * Deletes the internal mutex and frees the DHT22 instance.
 *
 * @param[in,out] self_ptr Pointer to dht22_t instance.
 */

static void dht22_cleanup(void* self_ptr) {
    dht22_t* self = (dht22_t*)self_ptr;
    if (!self) return;
    if (self->mtx) vSemaphoreDelete(self->mtx);
    free(self);
}

/** 
 * @brief Create a new DHT22 instance and return its Subject interface.
 *
 * Allocates memory for the driver, initializes internal state,
 * and returns the abstract Subject pointer for observer usage.
 *
 * @return subject_t* Pointer to subject interface, or NULL on allocation failure.
 */
subject_t* dht22_create(void) {
    dht22_t* self = (dht22_t*)malloc(sizeof(dht22_t));
    if (!self) return NULL;
    dht22_init(self);
    return &self->subject;
}

/**
 * @brief Trigger a sensor measurement and update internal data.
 *
 * Performs one-wire transaction:
 *  1. Send start signal.
 *  2. Wait for sensor response.
 *  3. Read raw data.
 * Updates the internal data structure atomically and notifies subscribers.
 *
 * @param[in,out] self DHT22 instance handle.
 * @retval DHT22_OK          Success.
 * @retval DHT22_ERROR_TIMEOUT Timeout waiting for sensor or mutex.
 */
dht22_error_t dht22_get_data(dht22_t* self) {
    dht22_data_t tmp;
    __start_signal();
	if (__wait_respond_signal() != DHT22_OK){
		return DHT22_ERROR_TIMEOUT;
	}
    __read_data(&tmp);
    if(xSemaphoreTake(self->mtx, portMAX_DELAY) == pdTRUE){
        self->data.temperature = tmp.temperature;
        self->data.humidity = tmp.humidity;
        xSemaphoreGive(self->mtx);
    }
    else return DHT22_ERROR_TIMEOUT;
    dht22_notify(self);
	return DHT22_OK;
}


/**
 * @brief Copy the measured data to the output 
 *
 * Thread-safe: acquires the mutex before copying.
 *
 * @param[in]  self DHT22 instance handle.
 * @param[out] out  Destination buffer for sensor data (must not be NULL).
 */
void dht22_update_data(dht22_t* self, dht22_data_t* out) {
    if (!self || !out) return;
    if (xSemaphoreTake(self->mtx, portMAX_DELAY) == pdTRUE) {
        *out = self->data;
        xSemaphoreGive(self->mtx);
    }
}

/**
 * @brief Subject interface wrapper for ::dht22_update_data.
 *
 * Used by the generic Subject API.
 *
 * @param[in]  self_ptr Pointer to dht22_t instance.
 * @param[out] out      Output buffer cast to dht22_data_t.
 */
static void dht22_update_data_generic(void* self_ptr, void* out) {
    dht22_t* self = (dht22_t*)self_ptr;
    dht22_update_data(self, (dht22_data_t*)out);
}

/**
 * @brief Subscribe a client to DHT22 notifications.
 *
 * Allocates a notification handle and stores client,callback in the subscriber list.
 *
 * @param[in,out] self_ptr Pointer to dht22_t instance.
 * @param[in] client       Pointer to client instance (observer).
 * @param[in] cb           Callback function to notify.
 */
static void dht22_subscribe(void* self_ptr, void* client, subject_notify_callback_t cb) {
    dht22_t* self = (dht22_t*)self_ptr;
    notification_handle_t* nh = (notification_handle_t*)malloc(sizeof(notification_handle_t));
    nh->instance = client;
    nh->callback = cb;
    for(int pos = 0; pos < MAX_SUBSCRIBERS; pos++){
        if(!self->subscribers[pos]) {
            self->subscribers[pos] = nh;
            ESP_LOGI(TAG, "Subscribe success %p %p", nh->instance, nh->callback);
            return; 
        }
    }
    ESP_LOGI(TAG, "Subscribe failed, max subscribers reached\n");
    free(nh);
}

/**
 * @brief Unsubscribe a client callback from DHT22 notifications.
 *
 * Frees the associated notification handle.
 *
 * @param[in,out] self_ptr Pointer to dht22_t instance.
 * @param[in] cb           Callback function to remove.
 */
static void dht22_unsubscribe(void* self_ptr, subject_notify_callback_t cb) {
    dht22_t* self = (dht22_t*)self_ptr;
    for(int pos = 0; pos < MAX_SUBSCRIBERS; pos++){
        if(self->subscribers[pos])
            if(self->subscribers[pos]->callback == cb) {
                free(self->subscribers[pos]);
                self->subscribers[pos] = NULL;
            }
    }
}

/**
 * @brief Notify all subscribed clients with the latest data.
 *
 * Iterates through the subscriber list and calls each registered callback.
 *
 * @param[in] self_ptr Pointer to dht22_t instance.
 */
static void dht22_notify(void* self_ptr) {
    dht22_t* self = (dht22_t*)self_ptr;
    for(int pos = 0; pos < MAX_SUBSCRIBERS; pos++){
        if(self->subscribers[pos])
            if(self->subscribers[pos]->callback){
                self->subscribers[pos]->callback(self->subscribers[pos]->instance);
            }
    }
}

