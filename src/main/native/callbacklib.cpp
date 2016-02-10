// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "callbacklib.h"
#include "callback_support.h"

/**
 * This function simply calls reportBack from the same thread.
 */
int magicCFunction(void *ctx, report_fn reportBack) {
    const char *c1 = "Hello There";
    const char *c2 = "I'm bored";
    const char *c3 = "Foo";
    (*reportBack)(ctx, 1, c1, strlen(c1), c2, strlen(c2), c3, strlen(c3));

    return 0;
}

/**
 * This function calls reportBack from a new thread.
 */
struct NewThreadData {
    void *ctx;
    report_fn reportBack;
};

int newThreadCallback(void *data) {
    fprintf(stderr, "in new thread\n");
    NewThreadData *ptr = (NewThreadData *) data;

    // make sure we detach when done.
    ScopedDetach detach;

    magicCFunction(ptr->ctx, ptr->reportBack);
    free(ptr->ctx);
    free(ptr);
    return 0;
}

int callbackFromANewThread(void *ctx, report_fn reportBack) {
    NewThreadData *data = (NewThreadData *) calloc(1, sizeof(NewThreadData));
    data->ctx = ctx;
    data->reportBack = reportBack;

    pthread_attr_t attr;
    pthread_t thread;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_create(&thread, &attr,
            (void *(*)(void*))newThreadCallback, data);
    fprintf(stderr, "Created thread %p ret: %d\n", thread, ret);
}
