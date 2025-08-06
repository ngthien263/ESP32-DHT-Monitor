#include "dht22.h"
#include "stdlib.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/timers.h"
#include "freertos/task.h"
void dht22_init(dht22_t* const self) {
    self->data = NULL;
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++){
        self->subscribers[pos] = NULL;
    }
}
void dht22_cleanup(dht22_t* const self) {
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++) {
        if (self->subscribers[pos])
            dht22_unsubscribe(self, self->subscribers[pos]->subscriber_update_func);
    }
    free(self->data);
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
    __start_signal();
	if (__wait_respond_signal() == DHT22_OK){
		__read_data(self->data);
		printf("Humidity: %.2f\n", self->data->humidity);
		printf("Temprature: %.2f\n", self->data->temperature);
	} else {
		return DHT22_ERROR_TIMEOUT;
	}
    dht22_notify(self);
	return DHT22_OK;
}

void dht22_notify(dht22_t* const self) {
    for(int pos = 0; pos < DHT22_MAX_SUBSCRIBERS; pos++){
        if(self->subscribers[pos])
            if(self->subscribers[pos]->subscriber_update_func){
                self->subscribers[pos]->subscriber_update_func(self->subscribers[pos]->subscriber_instance, self->data);
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
        self->data = malloc(sizeof(dht22_data_t));
    }
    return self;
}