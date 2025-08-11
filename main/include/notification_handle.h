#ifndef DHT22_NOTIFICATION_HANDLE_H
#define DHT22_NOTIFICATION_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct notification_handle_t {
    void (*subscriber_update_func)(void* instance);
    void *subscriber_instance;
} notification_handle_t;

#ifdef __cplusplus
}
#endif

#endif