// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.

#ifndef _CALLBACK_SUPPORT_H_
#define _CALLBACK_SUPPORT_H_

#include <jni.h>
#include <string.h>
#include <stdio.h>

// can't live without all my macros.
#include "jni_helper_defines.h"

/**
 * This is how we should attach the thread to the JVM.
 *
 * http://stackoverflow.com/questions/20325792/java-thread-leaks-when-calling-back-from-native-thread-via-jni has some super interesting caveats about this.
 * Specifically the openjdk bug https://bugs.openjdk.java.net/browse/JDK-8033696.
 *
 * Docs cover this here:
 * http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/invocation.html
 *
 * They say you always need to attach, and detach, but can only detach if there are no java threads in the stack.
 * The android docs mention this is expensive.
 *
 * There is also AttachCurrentThreadAsDaemon, which will cleanup on exit?
 * http://docs.oracle.com/javase/7/docs/technotes/guides/jni/jni-14.html#AttachCurrentThreadAsDaemon
 *
 */
enum AttachType {
    /**
     * Assume that the thread is attached because this is a same thread callback
     */
    NEVER_ATTACH, //

    /**
     * Attach if not attached, and treat it as a daemon, meaning the JVM will exit even if this thread is running.
     */
    ATTACH_AS_DAEMON, //

    /**
     * Attach and use pthread destructors to detach.  Apparently requires 7u80, or 8u20 or newer.
     * See http://stackoverflow.com/questions/20325792/java-thread-leaks-when-calling-back-from-native-thread-via-jni and https://bugs.openjdk.java.net/browse/JDK-8033696
     * for an explanation of what can go wrong here.
     */
    ATTACH_WITH_TLS_DETACH
};

/**
 * This is the struct we pass into the c callback as a void*
 * the object within is an implementation of com.yahoo.jni.example.CallbackInterface
 */
typedef struct {
    jobject object;
    AttachType attach;
} report_callback_context_t;

/**
 * Exported here in case others need it.
 */
extern JavaVM *global_jvm;

JNIEnv *getJNIEnvFromJavaVM(JavaVM *jvm, AttachType attachType);

#endif //_CALLBACK_SUPPORT_H_
