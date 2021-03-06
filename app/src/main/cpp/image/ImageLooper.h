//
// Created by templechen on 2020/10/19.
//

#ifndef SHADERIMAGE_IMAGELOOPER_H
#define SHADERIMAGE_IMAGELOOPER_H


#include <jni.h>
#include "../base/Looper.h"
#include "ImageRenderer.h"

class ImageLooper : public Looper{

public:

    enum {
        kMsgImageOnDestroy,
        kMsgRenderImageData
    };

    ImageLooper(JavaVM* javaVM);

    virtual ~ImageLooper();

    virtual void handleMessage(LooperMessage *msg);

    virtual void pthreadExit();

private:

    ImageRenderer *renderer;
};


#endif //SHADERIMAGE_IMAGELOOPER_H
