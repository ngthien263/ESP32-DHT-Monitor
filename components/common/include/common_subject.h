/**
 * @file common_subject.h
 * @brief Generic Subject interface for implementing the Observer design pattern.
 *
 * This module defines a generic "Subject" abstraction t to register, unregister, 
 * and receive notifications when data changes. It is intended to be used for a
 * driver or service objects (e.g., a sensor driver) to support publish-subscribe.
 */

#ifndef COMMON_SUBJECT_H
#define COMMON_SUBJECT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @def MAX_SUBSCRIBERS
 * @brief Maximum number of observers that can subscribe to a subject.
 */
#define MAX_SUBSCRIBERS 100

/**
 * @typedef subject_notify_callback_t
 * @brief Callback for observer notifications.
 *
 * @param instance Pointer to the observer instance receiving the notification.
 */
typedef void (*subject_notify_callback_t)(void* instance);

/**
 * @struct notification_handle_t
 * @brief Represents a subscriber entry.
 *
 * Stores both a pointer to the client instance and the callback
 * function that should be invoked when the subject notifies.
 */
typedef struct {
    void* instance;                                                                    /**< Pointer to the observer/client instance. */
    subject_notify_callback_t callback;                                                /**< Notification callback. */
} notification_handle_t;

/**
 * @struct subject_ops_t
 * @brief Operations vtable for Subject implementations.
 *
 * These functions must be implemented by a Subject (e.g. DHT22 driver).
 */
typedef struct {
    void (*init)(void* self);                                                           /**< Initialize the subject instance. */
    void (*cleanup)(void* self);                                                        /**< Clean up and release resources. */
    void (*subscribe)(void* self, void* client, subject_notify_callback_t cb);          /**< Add a subscriber. */
    void (*unsubscribe)(void* self, subject_notify_callback_t cb);                      /**< Remove a subscriber. */
    void (*notify)(void* self);                                                         /**< Notify all subscribers. */
    void (*update_data)(void* self, void* out);                                         /**< Copy current data snapshot to @p out. */
} subject_ops_t;

/**
 * @struct subject_t
 * @brief Generic Subject object.
 *
 * This structure acts as the base class for any concrete Subject implementation.
 * It stores the operation table pointer, self-reference, and subscriber list.
 */
typedef struct {
    const subject_ops_t* ops;                                                   /**< Operations table. */
    void* self;                                                                 /**< Pointer to concrete instance (e.g., driver struct). */
    notification_handle_t* subscribers[MAX_SUBSCRIBERS];                        /**< Subscriber list */
} subject_t;

/**
 * @brief Subscribe an observer to a subject.
 *
 * @param[in,out] subject  Subject instance.
 * @param[in]     instance Observer instance pointer.
 * @param[in]     cb       Callback to notify function.
 */
static inline void subject_subscribe(subject_t* subject, void* instance, subject_notify_callback_t cb) {
    subject->ops->subscribe(subject->self, instance, cb);
}

/**
 * @brief Unsubscribe an observer from a subject.
 *
 * @param[in,out] subject Subject instance.
 * @param[in]     cb      Callback to notify function.
 */
static inline void subject_unsubscribe(subject_t* subject, subject_notify_callback_t cb) {
    subject->ops->unsubscribe(subject->self, cb);
}

/**
 * @brief Notify all observers subscribed to the subject.
 *
 * Calls all registered callbacks in the subscriber list.
 *
 * @param[in,out] subject Subject instance.
 */
static inline void subject_notify(subject_t* subject){
    subject->ops->notify(subject);
}

/**
 * @brief Copy the subject's current data snapshot to @p out.
 *
 * @param[in]  subject Subject instance.
 * @param[out] out     Destination buffer for subject-specific data.
 */
static inline void subject_update_data(subject_t* subject, void* out) {
    subject->ops->update_data(subject->self, out);
}

#ifdef __cplusplus
}
#endif

#endif // COMMON_SUBJECT_H
