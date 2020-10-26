//
// Created by cw on 2020/10/23.
//

#include "ImageData.h"
#include "string"

ImageData::ImageData(int shaderType, int width, int height) {
    shaderImageView = nullptr;
    this->shaderType = shaderType;
    this->width = width;
    this->height = height;
}

void ImageData::setShaderImageView(JNIEnv *env, jobject javaShaderImageView) {
    this->shaderImageView = env->NewGlobalRef(javaShaderImageView);
}

jobject ImageData::getShaderImageView() const {
    return shaderImageView;
}

void ImageData::releaseShaderImageView(JNIEnv *env) {
    if (shaderImageView != nullptr) {
        env->DeleteGlobalRef(shaderImageView);
        shaderImageView = nullptr;
    }
}

bool ImageData::isSameShaderType(ImageData *imageData) const {
    return this->shaderType == imageData->shaderType;
}

bool ImageData::isSameSize(int w, int h) const {
    return this->width == w && this->height == h;
}
