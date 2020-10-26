//
// Created by cw on 2020/10/23.
//

#ifndef SHADERIMAGE_IMAGEDATA_H
#define SHADERIMAGE_IMAGEDATA_H

#include <jni.h>

class ImageData {

public:

    ImageData(int shaderType, int width, int height);

    ~ImageData() {};

    void setShaderImageView(JNIEnv *env, jobject javaShaderImageView);

    jobject getShaderImageView() const;

    void releaseShaderImageView(JNIEnv *env);

    bool isSameShaderType(ImageData *imageData) const;

    bool isSameSize(int w, int h) const;

    jobject shaderImageView;

    int shaderType;

    int width;

    int height;

};


#endif //SHADERIMAGE_IMAGEDATA_H
