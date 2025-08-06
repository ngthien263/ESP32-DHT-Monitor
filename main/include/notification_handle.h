#ifndef DHT22_NOTIFICATION_HANDLE_H
#define DHT22_NOTIFICATION_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct notification_handle_t {
    void (*subscriber_update_func)(void* self, struct dht22_data_t* data);
    void *subscriber_instance;
} notification_handle_t;

#ifdef __cplusplus
}
#endif

#endif