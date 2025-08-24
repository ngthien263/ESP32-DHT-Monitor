#pragma once

#define pdTRUE 1
#define pdFALSE 0
#define portMAX_DELAY 0xFFFFFFFF

#include <stdio.h>

typedef void* SemaphoreHandle_t;
typedef void* TaskHandle_t;

static inline SemaphoreHandle_t xSemaphoreCreateMutex() {
    return (SemaphoreHandle_t)1;  
}

static inline int xSemaphoreTake(SemaphoreHandle_t mtx, int ticks) {
    return pdTRUE;  
}

static inline void xSemaphoreGive(SemaphoreHandle_t mtx) {}

static inline void xTaskNotifyGive(TaskHandle_t task) {
    
}

static inline unsigned long ulTaskNotifyTake(int clear, unsigned long ticks) {
    return 1;  
}

static inline TaskHandle_t xTaskGetCurrentTaskHandle() {
    return (TaskHandle_t)1;
}

static inline void vSemaphoreDelete(SemaphoreHandle_t mtx){

}