// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

/**
 * This is the jni wrapper implementation. I like to keep the jni wrapper itself {@link ExampleJniAccess} being package
 * private and have a class like this do the checking and java wrangling.
 * 
 * @author areese
 *
 */
public class ExampleJni implements ExampleInterface {
    public ExampleJni() {
        JniLibraryLoader.load();
    }

    @Override
    public void executeInSameThread(CallbackInterface cb) {
        ExampleJniAccess.executeCWithCallBackInSameThread(cb);
    }


    @Override
    public void executeInNewThread(CallbackInterface cb) {
        ExampleJniAccess.executeCWithCallBackInNewThread(cb);
    }
}
