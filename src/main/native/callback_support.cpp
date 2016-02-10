// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
#include <jni.h>
#include <string.h>
#include <stdio.h>

// can't live without all my macros.
#include "jni_helper_defines.h"
#include "callback_support.h"

// You can't share JNIEnv, esp across threads.
// instead you need to use GetEnv to get it out.
JavaVM *global_jvm;

// http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/invocation.html
/**
 * JNI_OnLoad is called when the library is loaded.
 * We use this to init our global_vm pointer which we'll need later in life.
 */
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    // rather than constantly looking for the jvm get it when we load.
    global_jvm = vm;

    return JNI_VERSION_1_2;
}

/**
 * Depending on where this thread came from depends on if you need to attach and or detach the thread.
 */

/**
 * Given a JavaVM, get the JNIEnv out of it.
 */
JNIEnv *getJNIEnvFromJavaVM(JavaVM *jvm, AttachType attachType) {
    JNIEnv *jniEnv = NULL;
    int ret = jvm->GetEnv((void**) &jniEnv, JNI_VERSION_1_2);

    if (JNI_OK == ret) {
        return jniEnv;
    }

    JavaVMAttachArgs attach;
    memset(&attach, 0, sizeof(attach));
    attach.version=JNI_VERSION_1_2;

    if (JNI_EDETACHED == ret) {
        // the jvm is not attached to this thread.
        switch (attachType) {

        /**
         * NEVER_ATTACH is assuming that the callback stays in the same thread which is a java thread
         * java -> jni -> clib -> callback -> jni -> java
         * You shouldn't get EDETACHED in this state, and I even had code that cached JNIEnv in that situation and
         * ran under high load for many years.  :)
         */
        case NEVER_ATTACH:
            return NULL;

        /**
         * Attach, assuming the callback will detach when done.
         */
        case ATTACH_AND_DETACH_AFTER_CALLBACK:
            ret = jvm->AttachCurrentThread((void**)&jniEnv, &attach);
            if (JNI_OK == ret){
                return jniEnv;
            }

        }
    }

    return NULL;

}
