// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.

#ifndef _CALLBACK_SUPPORT_H_
#define _CALLBACK_SUPPORT_H_

#include <jni.h>
#include <string.h>
#include <stdio.h>

#include <memory>

// can't live without all my macros.
#include "jni_helper_defines.h"

/**
 * Exported here in case others need it.
 */
extern JavaVM *global_jvm;

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
    ATTACH_WITH_TLS_DETACH,

    /**
     * This will attach for you when EDETACHED is returned, but assumes you will call DetachCurrentThread() at the end of the callback adaptor.
     * You have to use NewGlobalRef on the objects handed around this way.
     * You also have to remember to free them..
     */
    ATTACH_AND_DETACH_AFTER_CALLBACK,
};

JNIEnv *getJNIEnvFromJavaVM(JavaVM *jvm, AttachType attachType);

/**
 * When passing objects across threads, you need a GlobalRef of the object to ensure it doesn't get free'd before the other thread gets a chance to do the callback.
 * Prob better done with a combination shared pointer and NewGlobalRef, but for right now NewGlobalRef is done in thread one, and this is in thread 2.
 */
class GlobalRefDestructor {
private:
    JNIEnv *jenv;
    jobject ref;

public:
    GlobalRefDestructor(JNIEnv *jenv, jobject ref) :
            jenv(jenv), ref(ref) {
    }
    ;

    ~GlobalRefDestructor() {
        if (NULL != jenv && NULL != ref) {
            jenv->DeleteGlobalRef(ref);
            jenv = NULL;
            ref = NULL;
        }
    }
};

class JavaReference {
private:
    JavaVM *jvm;
    jobject ref;
    AttachType attachType;

public:
    JavaReference(JavaVM *jvm, jobject ref, AttachType attachType) :
            jvm(jvm), attachType(attachType) {
        JNIEnv *jenv = getJNIEnvFromJavaVM(jvm, attachType);
        this->ref = jenv->NewGlobalRef(ref);
    }

    ~JavaReference() {
        release();
    }

    jobject get() {
        return ref;
    }

    void release() {
        if (NULL != ref) {
            JNIEnv *jenv = getJNIEnvFromJavaVM(this->jvm, this->attachType);
            jenv->DeleteGlobalRef(this->ref);
            this->ref = NULL;
        }
    }
};

/**
 * This is the struct we pass into the c callback as a void*
 * the object within is an implementation of com.yahoo.jni.example.CallbackInterface
 */
typedef struct {
    std::shared_ptr<JavaReference> object;
    AttachType attach;
} report_callback_context_t;

/**
 * This class can be used in the callback adapter.
 */
class ScopedDetach {
public:
    JNIEnv *jenv;

    ScopedDetach() {
        jenv = getJNIEnvFromJavaVM(global_jvm,
                ATTACH_AND_DETACH_AFTER_CALLBACK);
    }

    ~ScopedDetach() {
        global_jvm->DetachCurrentThread();
    }
};

#endif //_CALLBACK_SUPPORT_H_
