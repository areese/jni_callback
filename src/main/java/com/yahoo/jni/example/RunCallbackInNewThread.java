// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

import java.util.concurrent.TimeUnit;

/**
 * Run with java -cp target/classes -Djava.library.path=target/native/x86_64-linux-gcc com.yahoo.jni.example.RunCallback
 * or java -cp target/classes -Djava.library.path=target/native/x86_64-darwin-clang com.yahoo.jni.example.RunCallback
 * 
 * @author areese
 *
 */
public class RunCallbackInNewThread {
    public static void main(String[] args) throws InterruptedException {
        ExampleJni e = new ExampleJni();
        // e.executeInSameThread(new CallbackImplementation());
        e.executeInNewThread(new CallbackImplementation());

        System.err.println("Sleeping 5 seconds to give the callback a chance");
        TimeUnit.SECONDS.sleep(5);
    }
}
