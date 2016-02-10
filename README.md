This is example code for having a c library execute a java method as a callback.


Currently it is 3 parts:

com.yahoo.jni.example.ExampleJni  This class calls the JNI and gives it a Java function to use as a callback

com.yahoo.jni.example.JniLibraryLoader  This class is because I'm sick of wondering why someone can't load the .so

com.yahoo.jni.example.RunCallback Is the main class.  java -cp target/class -Djava.library.path=target/native/x86_64-darwin-clang com.yahoo.jni.example.RunCallback

com.yahoo.jni.example.ExampleJniAccess  This is the class containing the native functions

com.yahoo.jni.example.CallbackImplementation This class contains the java function that the c library will call in the end.




Important reading:

Invocation api spec:
http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/invocation.html


Caveats about using pthread destructors to detach threads:
http://stackoverflow.com/questions/20325792/java-thread-leaks-when-calling-back-from-native-thread-via-jni
https://bugs.openjdk.java.net/browse/JDK-8033696

http://docs.oracle.com/javase/7/docs/technotes/guides/jni/jni-14.html#AttachCurrentThreadAsDaemon



