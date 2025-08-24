/**
 * @file common_observer.h
 * @brief Generic Observer helper types and inline APIs for the Observer pattern.
 *
 * This module defines a lightweight observer wrapper that holds:
 *  - A pointer to a Subject (publisher),
 *  - A RTOS task handle (for notify-to-task patterns),
 *  - A notification callback invoked by the Subject.
 *
 * It provides inline helpers to set the subject and (un)subscribe.
 * The concrete observer object should set @ref observer_t.self and @ref observer_t.on_notify
 * before subscribing.
 */

#ifndef COMMON_OBSERVER_H
#define COMMON_OBSERVER_H

#include "common_subject.h"
#ifndef UNIT_TEST
#include "freertos/task.h"
#else 
#include "mock_freertos.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct observer_ctx_t
 * @brief Runtime context for an observer.
 *
 */
typedef struct {
    subject_t* subject;            
    TaskHandle_t task_handle;      
} observer_ctx_t;


/**
 * @struct observer_t
 * @brief Generic Observer “base class”.
 * 
 */
typedef struct {
    void* self;
    observer_ctx_t ctx;
    void (*on_notify)(void*);
} observer_t;


/**
 * @brief Wrapper to bind an observer to a subject (no subscription yet).
 *
 * @param[in,out] observer Observer to bind.
 * @param[in]     subject  Subject to observe.
 *
 * @warning This does not perform subscription. Call @ref observer_subscribe next.
 */
static inline void observer_set_subject(observer_t* observer, subject_t* subject){
    observer->ctx.subject = subject;
}

/**
 * @brief Subscribe the observer to its bound subject.
 *
 * Invokes Subject’s subscribe with @ref observer_t.self and @ref observer_t.on_notify.
 * @note Ensure both fields are set before calling.
 *
 * @param[in,out] observer Observer to subscribe.
 *
 */
static inline void observer_subscribe(observer_t* observer) {
    subject_subscribe(observer->ctx.subject, observer->self, observer->on_notify);
} 

/**
 * @brief Unsubscribe the observer from its bound subject.
 *
 * Removes the callback from the Subject’s subscriber list.
 *
 * @param[in,out] observer Observer to unsubscribe.
 *
 */
static inline void observer_unsubscribe(observer_t* observer) {
    subject_unsubscribe(observer->ctx.subject, observer->on_notify);
} 

#ifdef __cplusplus
}
#endif
#endif // COMMON_OBSERVER_H