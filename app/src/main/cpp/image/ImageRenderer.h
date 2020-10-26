//
// Created by templechen on 2020/10/19.
//

#ifndef SHADERIMAGE_IMAGERENDERER_H
#define SHADERIMAGE_IMAGERENDERER_H


#include <jni.h>
#include "../egl/egl_core.h"
#include "../egl/offscreen_surface.h"
#include "filter/ImageBaseFilter.h"
#include "ImageData.h"

class ImageRenderer {

public:

    ImageRenderer(JavaVM *javaVM);

    ~ImageRenderer();

    void ImageCreated(int width, int height);

    void ImageDoFrame(ImageData *imageData);

    void ImageDestroyed();

    void renderImageData(ImageData *imageData);

    void initFilter(int type);

private:

    int bitmapWidth;
    int bitmapHeight;

    egl_core *eglCore = nullptr;

    offscreen_surface *offSurface = nullptr;

    ImageBaseFilter *baseFilter = nullptr;

    //
    JavaVM *javaVM = nullptr;

    JNIEnv *env = nullptr;

    int bitmapFormat = 0;

};

#endif //SHADERIMAGE_IMAGERENDERER_H
