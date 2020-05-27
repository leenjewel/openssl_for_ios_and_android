#include <jni.h>
#include <string>
//#include "test/test_http2.hpp"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_test_1curl_1with_1ssl_1and_1http2_1android_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
//    test_curl_http2();
    return env->NewStringUTF(hello.c_str());
}
