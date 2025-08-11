#ifndef LOGGER_H
#define LOGGER_H

#include "dht22_types.h"
#include "dht22.h"
#ifdef __cplusplus
extern "C" {
#endif


typedef struct logger_t {
    dht22_data_t data;
    observer_ctx_t logger_ctx;
} logger_t;

void logger_write_to_file(logger_t* const log);
void logger_subscribe(logger_t* const log);

#ifdef __cplusplus
}
#endif

#endif // LOGGER_H