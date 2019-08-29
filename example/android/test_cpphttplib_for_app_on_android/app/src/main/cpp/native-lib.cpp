#include <jni.h>
#include <string>
#include "http/test_http.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_citylife_test_1cpphttplib_1for_1app_1on_1android_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    test_https();


    return env->NewStringUTF(hello.c_str());
}
