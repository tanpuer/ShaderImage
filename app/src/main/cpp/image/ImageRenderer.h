//
// Created by templechen on 2020/10/19.
//

#ifndef SHADERIMAGE_IMAGERENDERER_H
#define SHADERIMAGE_IMAGERENDERER_H


#include <jni.h>
#include "../egl/egl_core.h"
#include "../egl/offscreen_surface.h"
#include "ImageBaseFilter.h"

class ImageRenderer {

public:

    ImageRenderer(JavaVM* javaVM, jobject shaderImageView);

    ~ImageRenderer();

    void ImageCreated(int width, int height);

    void ImageDoFrame();

    void ImageDestroyed();

private:

    int bitmapWidth;
    int bitmapHeight;

    egl_core *eglCore;

    offscreen_surface *offSurface = nullptr;

    ImageBaseFilter *baseFilter;

    //
    JavaVM* javaVM;

    jobject shaderImageView;

    JNIEnv *env;

    int bitmapFormat = 0;

};

#endif //SHADERIMAGE_IMAGERENDERER_H
