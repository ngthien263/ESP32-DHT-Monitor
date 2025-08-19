#ifndef COMMON_SUBJECT_H
#define COMMON_SUBJECT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SUBSCRIBERS 16


typedef void (*subject_notify_callback_t)(void* instance);

typedef struct {
    void* instance;
    subject_notify_callback_t callback;
} notification_handle_t;

typedef struct {
    void (*init)(void* self);
    void (*cleanup)(void*  self);
    void (*subscribe)(void*  self, void* client, subject_notify_callback_t cb);
    void (*unsubscribe)(void*  self, subject_notify_callback_t cb);
    void (*notify)(void*  self);
    void (*get_data)(void*  self, void* out); 
} subject_ops_t;

typedef struct {
    const subject_ops_t* ops;
    void* self; 
    notification_handle_t* subscribers[MAX_SUBSCRIBERS]; 
} subject_t;

static inline void subject_subscribe(subject_t* subject, void* instance, subject_notify_callback_t cb) {
    subject->ops->subscribe(subject->self, instance, cb);
}

static inline void subject_unsubscribe(subject_t* subject, subject_notify_callback_t cb) {
    subject->ops->unsubscribe(subject->self, cb);
}

static inline void subject_get_data(subject_t* subject, void* out) {
    subject->ops->get_data(subject->self, out);
}

#ifdef __cplusplus
}
#endif

#endif // COMMON_SUBJECT_H
