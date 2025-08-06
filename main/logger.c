#include "logger.h"
#include <stdio.h>
#include "stdlib.h"
#include "time.h"

void logger_init(logger_t* const log) {
    log->data = NULL;
    log->dht22 = NULL;
}

void logger_on_notify(void* context, dht22_data_t* data){
    logger_t* log = (logger_t*)context;
    log->data = data;
    logger_write_to_file(log);
}

void logger_write_to_file(logger_t* const log){
    time_t seconds;
    time(&seconds);
    struct tm *timeinfo = localtime(&seconds);
    char buffer[80];
    strftime(buffer, 80, "%d-%m-%Y %H:%M:%S", timeinfo);
    printf("Time: %s\n", buffer);
    printf("Current Temperature: %.2f\n", log->data->temperature);
    printf("Current Humidity: %.2f\n", log->data->humidity);
}

void logger_subscribe(logger_t* const log) {
    if(log->dht22)
        dht22_subscribe(log->dht22, log, &logger_on_notify);
}
