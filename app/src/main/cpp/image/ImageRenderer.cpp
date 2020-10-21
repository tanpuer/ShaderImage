//
// Created by templechen on 2020/10/19.
//

#include "ImageRenderer.h"
#include "../base/native_log.h"
#include "../base/gl_utils.h"
#include <GLES3/gl3.h>
#include <android/bitmap.h>
#include "assert.h"

ImageRenderer::ImageRenderer(JavaVM *javaVM, jobject shaderImageView) {
    ALOGD("ImageRenderer init")
    this->javaVM = javaVM;
    this->shaderImageView = shaderImageView;
}

ImageRenderer::~ImageRenderer() {
    delete baseFilter;
    javaVM->DetachCurrentThread();
    offSurface->release();
    offSurface = nullptr;
    eglCore->release();
    eglCore = nullptr;
}

void ImageRenderer::ImageCreated(int width, int height) {
    ALOGD("ImageCreated")
    javaVM->AttachCurrentThread(&env, nullptr);
    bitmapWidth = width;
    bitmapHeight = height;
    eglCore = new egl_core(nullptr, FLAG_TRY_GLES3);
    ALOGD("ImageCreated %d %d", width, height)
    offSurface = new offscreen_surface(eglCore, width, height);
    offSurface->makeCurrent();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    baseFilter = new ImageBaseFilter();
    baseFilter->setImageViewSize(width, height);
    glViewport(0, 0, bitmapWidth, bitmapHeight);

    GLint imp_fmt, imp_type;
    glGetIntegerv (GL_IMPLEMENTATION_COLOR_READ_FORMAT, &imp_fmt);
    glGetIntegerv (GL_IMPLEMENTATION_COLOR_READ_TYPE,   &imp_type);
    ALOGD("Supported Color Format/Type: %x/%x\n", imp_fmt, imp_type)
    //0x1907 0x1401表示glReadPixels只支持rgba和rgb格式
}

void ImageRenderer::ImageDoFrame() {
    jclass clazz = env->GetObjectClass(shaderImageView);
    assert(clazz != nullptr);
    // 读取 bitmap 的像素内容到 native 内存
    int ret;
    void *bitmapPixels;
    jmethodID currentBitmap = env->GetMethodID(clazz, "currentBitmap",
                                               "(I)Landroid/graphics/Bitmap;");
    assert(currentBitmap != nullptr);
    jobject bitmap = env->CallObjectMethod(shaderImageView, currentBitmap, 0);
    if (bitmap == nullptr) {
        ALOGE("current bitmap is nullptr, pls check")
        return;
    }
    AndroidBitmapInfo bitmapInfo;
    if ((ret = AndroidBitmap_getInfo(env, bitmap, &bitmapInfo)) < 0) {
        ALOGD("AndroidBitmap_getInfo() failed ! error=%d", ret)
        return;
    }
    bitmapFormat = bitmapInfo.format;
    GLenum format = GL_RGBA;
    jint size = 0;
    switch (bitmapInfo.format) {
        case ANDROID_BITMAP_FORMAT_RGBA_8888: {
            ALOGD("bitmapFormat is RGBA_8888")
            format = GL_RGBA;
            size = bitmapWidth * bitmapHeight * 4;
            break;
        }
        //todo 暂不支持 需要引入libyuv
//        case ANDROID_BITMAP_FORMAT_RGB_565: {
//            ALOGD("bitmapFormat is RGB_565")
//            format = GL_RGB;
//            size = bitmapWidth * bitmapHeight * 3;
//            break;
//        }
        case ANDROID_BITMAP_FORMAT_RGBA_4444: {
            ALOGD("bitmapFormat is RGBA_4444")
            format = GL_RGBA4;
            size = bitmapWidth * bitmapHeight * 2;
            break;
        }
        //todo 暂不支持 需要引入libyuv
//        case ANDROID_BITMAP_FORMAT_A_8: {
//            ALOGD("bitmapFormat is ALPHA_8")
//            format = GL_ALPHA;
//            size = bitmapWidth * bitmapHeight;
//            break;
//        }
    }
    if (size == 0 || bitmapFormat == ANDROID_BITMAP_FORMAT_NONE) {
        ALOGE("bitmap has no format, pls check")
    }
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels)) < 0) {
        ALOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret)
        return;
    } else {
        ALOGD("AndroidBitmap_lockPixels() success")
    }

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    if (baseFilter != nullptr) {
        baseFilter->doFrame(bitmapPixels, format);
    }
    glFlush();
    checkGLError("glFlush");

    auto *buffer = static_cast<unsigned char *>(malloc((size_t) size));
    glReadPixels(0, 0, bitmapWidth, bitmapHeight, format, GL_UNSIGNED_BYTE, buffer);
    checkGLError("glReadPixels");
    memcpy(bitmapPixels, buffer, size);
    AndroidBitmap_unlockPixels(env, bitmap);
}

void ImageRenderer::ImageDestroyed() {
    env->DeleteGlobalRef(shaderImageView);
    javaVM->DetachCurrentThread();
}
