// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

/**
 * This is an implementation of a callback called from the jni. Be careful what you do in these because the call stack
 * is:
 * 
 * Java -> Jni -> C -> Jni -> Java
 * 
 * @author areese
 *
 */
public class CallbackImplementation implements CallbackInterface {

    @Override
    public void report(int kind, String name, String value, String reason) {
        System.err.println("Got callback " + kind + " with name=" + name + " value=" + value + " because " + reason);
        // just because it's interesting to understand how we got here.
        new Exception().printStackTrace();
    }

}
