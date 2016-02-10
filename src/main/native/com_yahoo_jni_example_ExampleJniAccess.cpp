// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
#include <jni.h>
#include <string.h>
#include <stdio.h>

// can't live without all my macros.
#include "jni_helper_defines.h"
// this is some little support library for callbacks.
#include "callback_support.h"
// this contains the c function that invokes the callback
#include "callbacklib.h"
#include "com_yahoo_jni_example_ExampleJniAccess.h"

#define CONTEXT_OBJ context->object.get()->get()

// simply fully qualified java class name with . changed to /
DECLARE_CACHED_CLASS(reportCallbackClass,
        "com/yahoo/jni/example/CallbackInterface");

// use javap -s -cp target/classes/ com.yahoo.jni.example.CallbackInterface to get the signature.
// you'll need one of these for every method.
DECLARE_CACHED_METHOD_ID(reportCallbackClass, reportID, "report",
        "(ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

/**
 * This function is called from the c library (callbacklib.cpp/magicCFunction in this case) and used to call back into java.
 */
void report_callback_adaptor(void *voidContextPtr, int kind, const char *arg1,
        int len1, const char *arg2, int len2, const char *arg3, int len3) {
    //  fprintf(stderr, "JNI: report called: ctx: %p, kind: %d, arg1: %p, %d, arg2: %p,%d, arg3: %p,%d\n",ctx,kind,arg1,len1,arg2,len2,arg3,len3);

    // We could move this later but we wouldn't have the signal of how to conncet back to the vm.
    if (NULL == voidContextPtr) {
        fprintf(stderr, "Null ctx in report_callback_adaptor!\n");
        return;
    }

    report_callback_context_t *context =
            (report_callback_context_t *) voidContextPtr;

    // However, we need access to JNIEnv in there, and we can't use the one we have,
    // we have to get a java_vm and use that.
    JNIEnv *jenv = getJNIEnvFromJavaVM(global_jvm, context->attach);

    // This will return, back to the function in the c library that called our callback
    // When it finally unwinds itself, and someone calls jenv->ExceptionCheck(),
    // They should return, and then it goes back into java and the exception is thrown.
    if (NULL == jenv) {
        fprintf(stderr, "Unable to locate jvm in callback\n");
        return;
    }

    // get the method and cache it.
    GET_CACHED_METHOD_ID(jenv, reportID);
    RETURN_AND_THROW_IF_NULL(reportID, "getting reportID");

    RETURN_AND_THROW_IF_NULL(CONTEXT_OBJ, "Null object in context");

    jstring str1 = NULL;
    jstring str2 = NULL;
    jstring str3 = NULL;

    {
        int maxLength = len1 > len2 ? len1 : len2;
        maxLength = maxLength > len3 ? maxLength : len3;

        char *buffer = (char *) calloc(maxLength + 1, sizeof(char));

        if (arg1) {
            memset(buffer, 0, maxLength + 1);
            memcpy(buffer, arg1, len1);
            buffer[len1] = 0;

            str1 = jenv->NewStringUTF(buffer);

            // This really confuses the hell out of people.
            // Had a bug the other day where calloc was failing, and they were getting NPE's.
            // it's way better than cores.
            RETURN_AND_THROW_IF_NULL(str1, "Error allocating String1");
        }

        if (arg2) {
            memset(buffer, 0, maxLength + 1);
            memcpy(buffer, arg2, len2);
            buffer[len2] = 0;

            str2 = jenv->NewStringUTF(buffer);
            RETURN_AND_THROW_IF_NULL(str2, "Error allocating String2");
        }

        if (arg3) {
            memset(buffer, 0, maxLength + 1);
            memcpy(buffer, arg3, len3);
            buffer[len3] = 0;

            str3 = jenv->NewStringUTF(buffer);
            RETURN_AND_THROW_IF_NULL(str3, "Error allocating String3");
        }

        free(buffer);
    }

    jenv->CallVoidMethod(CONTEXT_OBJ, reportID, kind, str1, str2, str3);

    // release our refs now.
    jenv->DeleteLocalRef(str1);
    jenv->DeleteLocalRef(str2);
    jenv->DeleteLocalRef(str3);
}

/*
 * Class:     com_yahoo_jni_example_ExampleJniAccess
 * Method:    executeCWithCallBackInSameThread
 * Signature: (Lcom/yahoo/jni/example/CallbackInterface;)V
 */
JNIEXPORT void JNICALL Java_com_yahoo_jni_example_ExampleJniAccess_executeCWithCallBackInSameThread
(JNIEnv *jenv, jclass thisClass, jobject callbackObject)
{
    if (NULL==jenv || NULL==thisClass || NULL==callbackObject) {
        return;
    }

    // First we need to setup the context we are passing to the function.
    // If it's async, we should use a shared pointer, or we should be passing a free function in the context.
    report_callback_context_t *context = (report_callback_context_t *)calloc(1,sizeof(report_callback_context_t));
    // this should probably throw OOME instead of NPE
    //RETURN_AND_THROW_IF_NULL(context, "allocation failure");

    // set the attach type based on your intent.
    // in this case, we're not async, and we're not making the callback from another thread, so we can use NEVER_ATTACH
    // If we are in another thread, it depends on the lifetime.
    // For new threads, you can attach forever, but the jvm will wait for them to exit.
    // You can attach as a daemon, but the jvm could exit while that thread is busy.
    // You can attach and detach using a pthreads destructor to detach, but jdks older than 7u80, 8u20 will not detach.
    context->attach = NEVER_ATTACH;
    context->object = std::make_shared<JavaReference>(global_jvm, callbackObject, context->attach);

// To have a c function make a callback that calls java we need a callback adapter.

    magicCFunction(context, report_callback_adaptor);

    free(context);
}

/*
 * Class:     com_yahoo_jni_example_ExampleJniAccess
 * Method:    executeCWithCallBackInNewThread
 * Signature: (Lcom/yahoo/jni/example/CallbackInterface;)V
 */
JNIEXPORT void JNICALL Java_com_yahoo_jni_example_ExampleJniAccess_executeCWithCallBackInNewThread
(JNIEnv *jenv, jclass thisClass, jobject callbackObject)
{
    if (NULL==jenv || NULL==thisClass || NULL==callbackObject) {
        return;
    }

    // First we need to setup the context we are passing to the function.
    // If it's async, we should use a shared pointer, or we should be passing a free function in the context.
    report_callback_context_t *context = (report_callback_context_t *)calloc(1,sizeof(report_callback_context_t));
    // this should probably throw OOME instead of NPE
    RETURN_AND_THROW_IF_NULL(context, "allocation failure");

    // set the attach type based on your intent.
    // in this case, we're not async, and we're not making the callback from another thread, so we can use NEVER_ATTACH
    // If we are in another thread, it depends on the lifetime.
    // For new threads, you can attach forever, but the jvm will wait for them to exit.
    // You can attach as a daemon, but the jvm could exit while that thread is busy.
    // You can attach and detach using a pthreads destructor to detach, but jdks older than 7u80, 8u20 will not detach.
    context->attach = ATTACH_AND_DETACH_AFTER_CALLBACK;
    //FIXME need to release this later.
    context->object = std::make_shared<JavaReference>(global_jvm, callbackObject, context->attach);

    // To have a c function make a callback that calls java we need a callback adapter.
    // this function free's context.
    callbackFromANewThread(context, report_callback_adaptor);
}
