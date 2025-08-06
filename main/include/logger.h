#ifndef LOGGER_H
#define LOGGER_H

#include "dht22_types.h"
#include "dht22.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct logger_t {
    dht22_data_t* data;
    dht22_t* dht22;
} logger_t;
void logger_init(logger_t* const log);
void logger_write_to_file(logger_t* const log);
void logger_subscribe(logger_t* const log);
logger_t* logger_create();
#ifdef __cplusplus
}
#endif

#endif // LOGGER_H