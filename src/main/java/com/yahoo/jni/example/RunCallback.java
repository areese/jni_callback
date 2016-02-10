// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

/**
 * Run with java -cp target/classes -Djava.library.path=target/native/x86_64-linux-gcc com.yahoo.jni.example.RunCallback
 * or java -cp target/classes -Djava.library.path=target/native/x86_64-darwin-clang com.yahoo.jni.example.RunCallback
 * 
 * @author areese
 *
 */
public class RunCallback {
    public static void main(String[] args) {
        ExampleJni e = new ExampleJni();
        e.execute(new CallbackImplementation());
    }
}
