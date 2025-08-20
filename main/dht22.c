#include "dht22.h"
#include "stdlib.h"
#include "stdio.h"
#ifndef UNIT_TEST
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#endif

void dht22_init(dht22_t* const self) {
    self->data.temperature = 0;
    self->data.humidity = 0;
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++){
        self->subscribers[pos] = NULL;
    }
    self->mtx = xSemaphoreCreateMutex();
}
void dht22_cleanup(dht22_t* const self) {
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++) {
        if (self->subscribers[pos])
            dht22_unsubscribe(self, self->subscribers[pos]->subscriber_update_func);
    }
    free(self);
}

void dht22_dump_list(dht22_t* const self) {
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++) {
        if(self->subscribers[pos])
            if(self->subscribers[pos]->subscriber_update_func)
                printf("Client: %d, subscriber: %p, callback: %p", pos, 
                                                                   self->subscribers[pos]->subscriber_instance,  
                                                                   self->subscribers[pos]->subscriber_update_func);
    }
}

dht22_error_t dht22_get_data(dht22_t* const self) {
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

void dht22_update_data(dht22_t* const self, dht22_data_t* data){
    if(xSemaphoreTake(self->mtx, portMAX_DELAY) == pdTRUE){
        data->temperature = self->data.temperature;
        data->humidity = self->data.humidity;
    }
    xSemaphoreGive(self->mtx);
}

void dht22_notify(dht22_t* const self) {
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++){
        if(self->subscribers[pos])
            if(self->subscribers[pos]->subscriber_update_func){
                self->subscribers[pos]->subscriber_update_func(self->subscribers[pos]->subscriber_instance);
            }
    }
}

void dht22_subscribe(dht22_t* const self, void* subscriber_instance, dht22_notify_callback subcriber_ud_func){
    notification_handle_t* nh = (notification_handle_t*)malloc(sizeof(notification_handle_t));
    nh->subscriber_instance = subscriber_instance;
    nh->subscriber_update_func= subcriber_ud_func;
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++){
        if(!self->subscribers[pos]) {
            self->subscribers[pos] = nh;
            printf("DHT22: Subscribe success\n");
            return; 
        }
    }
    printf("DHT22: subscribe failed, max subscribers reached\n");
    free(nh);
}

void dht22_unsubscribe(dht22_t* const self, dht22_notify_callback subcriber_ud_func){
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++){
        if(self->subscribers[pos])
            if(self->subscribers[pos]->subscriber_update_func== subcriber_ud_func) {
                free(self->subscribers[pos]);
                self->subscribers[pos] = NULL;
            }
    }
}

dht22_t* dht22_create(void){
    dht22_t* self = (dht22_t*)malloc(sizeof(dht22_t));
    if (self != NULL) {
        dht22_init(self);
    }
    return self;
}