// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

/**
 * This interface represents the function that the C code will call as a callback.
 * 
 * @author areese
 *
 */
public interface CallbackInterface {
    /**
     * 
     * @param kind kind of callback being called
     * @param name The name we're calling back about
     * @param value The value we're reporting on
     * @param reason Why we called
     */
    public void report(int kind, String name, String value, String reason);

}
