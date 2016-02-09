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

    if (JNI_EDETACHED == ret) {
        // the jvm is not attached to this thread.
        switch (attachType) {
        case NEVER_ATTACH:
            return NULL;

            //FIXME: don't fail
        }
    }

    return NULL;

}
