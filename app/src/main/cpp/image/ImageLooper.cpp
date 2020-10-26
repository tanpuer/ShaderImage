//
// Created by templechen on 2020/10/19.
//

#include "ImageLooper.h"
#include "../base/native_log.h"

ImageLooper::ImageLooper(JavaVM* javaVM) {
    renderer = new ImageRenderer(javaVM);
}

ImageLooper::~ImageLooper() {

}

void ImageLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMsgImageOnDestroy: {
            if (renderer != nullptr) {
                renderer->ImageDestroyed();
            }
            break;
        }
        case kMsgRenderImageData: {
            if (renderer != nullptr) {
                renderer->renderImageData(static_cast<ImageData *>(msg->obj));
            }
            break;
        }
        default: {
            ALOGE("unsupportedType: %d", msg->what)
        }
    }
}

void ImageLooper::pthreadExit() {
    delete renderer;
    delete this;
}
