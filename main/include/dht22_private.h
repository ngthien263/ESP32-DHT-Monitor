#ifndef DHT22_PRIVATE_H
#define DHT22_PRIVATE_H
#include "dht22.h"
#ifdef __cplusplus
extern "C" {
#endif

dht22_error_t __start_signal();
dht22_error_t __wait_respond_signal();
dht22_error_t __read_data(dht22_data_t* data);

#ifdef __cplusplus
}
#endif

#endif // DHT22_PRIVATE_H