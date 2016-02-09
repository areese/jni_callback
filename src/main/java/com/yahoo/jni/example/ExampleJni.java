// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

public class ExampleJni implements ExampleInterface {
    public ExampleJni() {
        JniLibraryLoader.load();
    }

    @Override
    public void execute(CallbackInterface cb) {
        ExampleJniAccess.executeCWithCallBack(cb);
    }
}
