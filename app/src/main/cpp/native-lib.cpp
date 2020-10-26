#include <jni.h>
#include <string>
#include "image/ImageLooper.h"
#include "android/native_window_jni.h"
#include "base/native_log.h"

ImageLooper *imageLooper = nullptr;
JavaVM *javaVM;
int imageViewRefCount = 0;

extern "C" JNIEXPORT void JNICALL
Java_com_temple_shaderimage_ShaderImageView_onCreate(
        JNIEnv *env,
        jobject shaderImageView) {
    if (imageLooper == nullptr) {
        imageLooper = new ImageLooper(javaVM);
    }
    imageViewRefCount++;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_temple_shaderimage_ShaderImageView_performShader(
        JNIEnv *env,
        jobject thiz,
        jint width,
        jint height,
        jobject javaShaderImageView,
        jint shaderType) {
    if (imageLooper != nullptr) {
        auto *imageData = new ImageData(shaderType, width, height);
        imageData->setShaderImageView(env, javaShaderImageView);
        imageLooper->sendMessage(imageLooper->kMsgRenderImageData, imageData);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_temple_shaderimage_ShaderImageView_onDestroy(
        JNIEnv *env,
        jobject thiz) {
    imageViewRefCount--;
    ALOGD("ShaderImageView destroy %d", imageViewRefCount)
    if (imageViewRefCount == 0 && imageLooper != nullptr) {
        imageLooper->quit();
        imageLooper = nullptr;
    }
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    javaVM = vm;
    return JNI_VERSION_1_6;
}