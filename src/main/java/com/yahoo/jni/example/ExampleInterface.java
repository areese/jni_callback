// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

/**
 * This interface exists to provide a nice way to interact with the JNI. I've been burned many times by exposing native
 * functions or wrappers instead of an interface.
 */
public interface ExampleInterface {
    /**
     * Call libcallback, and have it execute {@link CallbackInterface\#report(int, String, String, String)} as part of
     * the flow.
     * 
     * @param cb an implementation of {@link CallbackInterface} that gets called from the jni.
     */
    public void execute(CallbackInterface cb);
}
