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

#include "dht22.h"
#include "dht22_private.h"
#include "common_subject.h"  
#define MAX_SUBSCRIBERS 16

// Subject operations table
static void dht22_init(void* self);
static void dht22_cleanup(void* self);
static void dht22_subscribe(void* self, void* client, subject_notify_callback_t cb);
static void dht22_unsubscribe(void* self, subject_notify_callback_t cb);
static void dht22_notify(void* self);
static void dht22_get_data_generic(void* self, void* out);

static const subject_ops_t dht22_ops = {
    .init = dht22_init,
    .cleanup = dht22_cleanup,
    .subscribe = dht22_subscribe,
    .unsubscribe = dht22_unsubscribe,
    .notify = dht22_notify,
    .get_data = dht22_get_data_generic
};


dht22_t* dht22_create(void) {
    dht22_t* self = (dht22_t*)calloc(1, sizeof(dht22_t));
    if (!self) return NULL;
    self->subject.ops = &dht22_ops;
    self->subject.self = self;
    self->mtx = xSemaphoreCreateMutex();
    dht22_init(self);
    return self;
}

void dht22_destroy(dht22_t* self) {
    if (!self) return;
    if (self->mtx) vSemaphoreDelete(self->mtx);
    free(self);
}

subject_t* dht22_subject(dht22_t* self) {
    return &self->subject;
}

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

void dht22_update_data(dht22_t* self, dht22_data_t* out) {
    if (!self || !out) return;
    if (xSemaphoreTake(self->mtx, portMAX_DELAY) == pdTRUE) {
        *out = self->data;
        xSemaphoreGive(self->mtx);
    }
}

static void dht22_init(void* self_ptr) {
    dht22_t* self = (dht22_t*)self_ptr;
    self->data.temperature = 0;
    self->data.humidity = 0;
    memset(self->subscribers, 0, sizeof(self->subscribers));
}

static void dht22_cleanup(void* self_ptr) {
    dht22_t* self = (dht22_t*)self_ptr;
    memset(self->subscribers, 0, sizeof(self->subscribers));
}

static void dht22_subscribe(void* self_ptr, void* client, subject_notify_callback_t cb) {
    dht22_t* self = (dht22_t*)self_ptr;
    notification_handle_t* nh = (notification_handle_t*)malloc(sizeof(notification_handle_t));
    nh->instance = client;
    nh->callback = cb;
    for(int pos = 0; pos < MAX_SUBSCRIBERS; pos++){
        if(!self->subscribers[pos]) {
            self->subscribers[pos] = nh;
            printf("DHT22: Subscribe success\n");
            return; 
        }
    }
    printf("DHT22: subscribe failed, max subscribers reached\n");
    free(nh);
}

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

static void dht22_notify(void* self_ptr) {
    dht22_t* self = (dht22_t*)self_ptr;
    for(int pos = 0; pos < MAX_SUBSCRIBERS; pos++){
        if(self->subscribers[pos])
            if(self->subscribers[pos]->callback){
                self->subscribers[pos]->callback(self->subscribers[pos]->instance);
            }
    }
}

static void dht22_get_data_generic(void* self_ptr, void* out) {
    dht22_t* self = (dht22_t*)self_ptr;
    dht22_update_data(self, (dht22_data_t*)out);
}
