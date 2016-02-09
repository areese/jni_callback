// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

/**
 * Run with java -Djava.library.path=target/native
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
