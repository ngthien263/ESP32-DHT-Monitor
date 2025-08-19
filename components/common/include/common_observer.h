#ifndef OBSERVER_H
#define OBSERVER_H

#include "common_subject.h"
#include "freertos/task.h"
#ifdef __cplusplus
extern "C" {
#endif



typedef struct {
    subject_t* subject;            
    TaskHandle_t task_handle;      
} observer_ctx_t;

typedef struct {
    void* self;
    observer_ctx_t ctx;
    void (*on_notify)(void*);
} observer_t;

static inline void observer_subscribe(observer_t* observer) {
    subject_subscribe(observer->ctx.subject, observer->self, observer->on_notify);
} 

static inline void observer_unsubscribe(observer_t* observer) {
    subject_unsubscribe(observer->ctx.subject, observer->on_notify);
} 

#ifdef __cplusplus
}
#endif
#endif // OBSERVER_H