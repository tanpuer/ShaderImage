#include <jni.h>
#include <string>
#include "image/ImageLooper.h"
#include "android/native_window_jni.h"

ImageLooper *imageLooper = nullptr;
JavaVM *javaVM;
extern "C" JNIEXPORT void JNICALL
Java_com_temple_shaderimage_ShaderImageView_onCreate(
        JNIEnv *env,
        jobject shaderImageView) {
    if (imageLooper != nullptr) {
        delete imageLooper;
    }
    imageLooper = new ImageLooper(javaVM, env->NewGlobalRef(shaderImageView));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_temple_shaderimage_ShaderImageView_performShader(
        JNIEnv *env,
        jobject thiz,
        jint width,
        jint height) {
    if (imageLooper != nullptr) {
        imageLooper->sendMessage(imageLooper->kMSGImageOnCreate, width, height);
        imageLooper->sendMessage(imageLooper->kMsgImageDoFrame);
        imageLooper->quit();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_temple_shaderimage_ShaderImageView_onDestroy(
        JNIEnv *env,
        jobject thiz) {

}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    javaVM = vm;
    return JNI_VERSION_1_6;
}