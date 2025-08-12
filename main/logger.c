#include "logger.h"
#include <stdio.h>
#include "stdlib.h"
#include "time.h"

void logger_init(logger_t* const log) {
    
}

void logger_on_notify(void* instance) {
    logger_t* ctx = (logger_t*)instance;
    if (ctx && ctx->logger_ctx.task_handle != NULL) {
        xTaskNotifyGive(ctx->logger_ctx.task_handle);
    } else {
        printf("Logger notify: task handle NULL!\n");
    }
}

void logger_write_to_file(logger_t* const log){
    time_t seconds;
    time(&seconds);
    struct tm *timeinfo = localtime(&seconds);
    char buffer[80];
    strftime(buffer, 80, "%d-%m-%Y %H:%M:%S", timeinfo);
    printf("Time: %s\n", buffer);
    printf("Current Temperature: %.2f\n", log->data.temperature);
    printf("Current Humidity: %.2f\n", log->data.humidity);
}

void logger_subscribe(logger_t* const log) {
    if(log->logger_ctx.sensor)
        dht22_subscribe(log->logger_ctx.sensor, log, &logger_on_notify);
}
