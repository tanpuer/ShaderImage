//
// Created by templechen on 2020/10/19.
//

#include "ImageLooper.h"
#include "../base/native_log.h"

ImageLooper::ImageLooper(JavaVM* javaVM, jobject shaderImageView) {
    renderer = new ImageRenderer(javaVM, shaderImageView);
}

ImageLooper::~ImageLooper() {

}

void ImageLooper::handleMessage(Looper::LooperMessage *msg) {
    switch (msg->what) {
        case kMSGImageOnCreate: {
            if (renderer != nullptr) {
                renderer->ImageCreated(msg->arg1, msg->arg2);
            }
            break;
        }
        case kMsgImageDoFrame: {
            if (renderer != nullptr) {
                renderer->ImageDoFrame();
            }
            break;
        }
        case kMsgImageOnDestroy: {
            if (renderer != nullptr) {
                renderer->ImageDestroyed();
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
