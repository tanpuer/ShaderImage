//
// Created by templechen on 2020/10/19.
//

#include "ImageRenderer.h"
#include "../base/native_log.h"
#include "../base/gl_utils.h"
#include <GLES3/gl3.h>
#include <android/bitmap.h>
#include <image/filter/ImageGaussionTwoPathFilter.h>
#include "assert.h"
#include "../base/utils.h"
#include "filter/ImageGrayFilter.h"
#include "filter/ImageElecShockFilter.h"

ImageRenderer::ImageRenderer(JavaVM *javaVM) {
    ALOGD("ImageRenderer init")
    this->javaVM = javaVM;
    this->bitmapWidth = -1;
    this->bitmapHeight = -1;
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
//    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, bitmapWidth, bitmapHeight);

    GLint imp_fmt, imp_type;
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &imp_fmt);
    glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &imp_type);
    ALOGD("Supported Color Format/Type: %x/%x\n", imp_fmt, imp_type)
    //0x1907 0x1401表示glReadPixels只支持rgba和rgb格式
}

void ImageRenderer::ImageDoFrame(ImageData *imageData) {
    long startTime = javaTimeMillis();
    jclass clazz = env->GetObjectClass(imageData->shaderImageView);
    assert(clazz != nullptr);
    // 读取 bitmap 的像素内容到 native 内存
    int ret;
    void *bitmapPixels;
    jmethodID currentBitmap = env->GetMethodID(clazz, "currentBitmap",
                                               "()Landroid/graphics/Bitmap;");
    assert(currentBitmap != nullptr);
    jobject bitmap = env->CallObjectMethod(imageData->shaderImageView, currentBitmap);
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
    //读取bitmap像素数据到bitmapPixels中
    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &bitmapPixels)) < 0) {
        ALOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret)
        return;
    } else {
        ALOGD("AndroidBitmap_lockPixels() success")
    }
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    if (baseFilter != nullptr) {
        //使用bitmapPixels进行贴图
        baseFilter->doFrame(bitmapPixels, format);
    }
    auto *buffer = static_cast<unsigned char *>(malloc((size_t) size));
    glReadPixels(0, 0, bitmapWidth, bitmapHeight, format, GL_UNSIGNED_BYTE, buffer);
    checkGLError("glReadPixels");

    memcpy(bitmapPixels, buffer, size);
    AndroidBitmap_unlockPixels(env, bitmap);
    long endTime = javaTimeMillis();
    ALOGD("process image cost time: %ld", endTime - startTime)

    //postInvalidate
    jmethodID postInvalidate = env->GetMethodID(clazz, "postInvalidate", "()V");
    if (postInvalidate != nullptr) {
        env->CallVoidMethod(imageData->shaderImageView, postInvalidate);
    }

    //清理imageData
    imageData->releaseShaderImageView(env);
}

void ImageRenderer::ImageDestroyed() {
    javaVM->DetachCurrentThread();
}

void ImageRenderer::renderImageData(ImageData *imageData) {
    if (!imageData->isSameSize(bitmapWidth, bitmapHeight)) {
        if (offSurface != nullptr) {
            delete offSurface;
            offSurface = nullptr;
        }
        this->ImageCreated(imageData->width, imageData->height);
    }
    if (baseFilter == nullptr || !baseFilter->isSameType(imageData->shaderType)) {
        delete baseFilter;
        baseFilter = nullptr;
        this->initFilter(imageData->shaderType);
    }
    this->ImageDoFrame(imageData);
}

void ImageRenderer::initFilter(int type) {
    switch (type) {
        case 1 : {
            baseFilter = new ImageGrayFilter(type);
            break;
        }
        case 2: {
            baseFilter = new ImageElecShockFilter(type);
            break;
        }
        case 3: {
            baseFilter = new ImageGaussionTwoPathFilter(type);
            break;
        }
        default: {
            baseFilter = new ImageBaseFilter(type);
        }
    }
    baseFilter->init();
    baseFilter->setImageViewSize(bitmapWidth, bitmapHeight);
}
