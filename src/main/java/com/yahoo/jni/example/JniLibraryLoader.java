// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.
package com.yahoo.jni.example;

public class JniLibraryLoader {
    static final String LIBRARY = "jni_callback";
    static {
        try {
            System.err.println("Loading " + LIBRARY + " native library");
            System.loadLibrary(LIBRARY);
            System.err.println("Loaded " + LIBRARY + " native library");
        } catch (UnsatisfiedLinkError error) {
            String javaLibraryPath = System.getProperty("java.library.path");

            System.err.println("Cannot load " + LIBRARY + " native library, -Djava.library.path="
                            + ((null == javaLibraryPath) ? "unset" : javaLibraryPath));
            error.printStackTrace();
            System.err.println("**********************************************************************************************************************\n"
                            + "**********************************************************************************************************************\n"
                            + "***                                                                                                                ***\n"
                            + "***                                                                                                                ***\n"
                            + "***    Cannot load "
                            + LIBRARY
                            + " native library appears to already have been loaded.                                 ***\n"
                            + "***  JNI only allows a native library to be loaded once per JVM..                                                  ***\n"
                            + "***  This means that any jars that use JNI should be linked under java.library.path and not bundled into your      ***\n"
                            + "***  ear or war file.                                                                                              ***\n"
                            + "***  This error is almost always due to bundling a second copy of the jar that uses JNI into your deployment.      ***\n"
                            + "***                                                                                                                ***\n"
                            + "***                                                                                                                ***\n"
                            + "**********************************************************************************************************************\n"
                            + "**********************************************************************************************************************\n");
            throw error;
        }
    }

    public static void load() {
        // yes it's empty;
    }
}
