#ifndef DHT22_H
#define DHT22_H

#include "dht22_types.h"

#include "dht22_private.h"
#ifdef __cplusplus
extern "C" {
#endif
#ifndef DHT22_PORT_NUM
#define DHT22_PORT_NUM 4
#endif

typedef void (*dht22_notify_callback)(void*);


//
void dht22_init(dht22_t* const self);
void dht22_cleanup(dht22_t* const self);
dht22_error_t dht22_get_data(dht22_t* const self);
void dht22_notify(dht22_t* const self);
void dht22_update_data(dht22_t* const self, dht22_data_t* data);
void dht22_subscribe(dht22_t* const self, void* subscriber_instance, dht22_notify_callback subcriber_notify_func);
void dht22_unsubscribe(dht22_t* const self, dht22_notify_callback notify_ptr);
dht22_t* dht22_create(void);
void dht22_destroy(dht22_t* const self);
#ifdef __cplusplus
}
#endif
#endif //DHT22_H