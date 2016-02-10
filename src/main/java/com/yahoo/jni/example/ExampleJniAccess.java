// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

/**
 * The whole purpose in life of this class is just a binding to the native functions. All java code that needs to be
 * dealt with before calling this lives in {@link ExampleJni}
 * 
 * @author areese
 *
 */
class ExampleJniAccess {
    static final native void executeCWithCallBackInSameThread(CallbackInterface cb);

    static final native void executeCWithCallBackInNewThread(CallbackInterface cb);

    static final native void executeCWithCallBackInSharedThread(CallbackInterface cb);
}
