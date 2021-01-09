//
// Created by cw on 2021/1/8.
//

#ifndef SHADERIMAGE_IMAGEGAUSSIONBLURFILTER_H
#define SHADERIMAGE_IMAGEGAUSSIONBLURFILTER_H


#include "ImageBaseFilter.h"

class ImageGaussionBlurFilter : public ImageBaseFilter {

public:

    enum {
        kGuassionBlurHorizontal,
        kGaussionBlurVertical
    };

    ImageGaussionBlurFilter(int type, int orientation);

    ~ImageGaussionBlurFilter();

    const char *getFragmentShaderString() override;

    const char *getVertexShaderString() override;

    void doFrame2(GLuint frameBufferTextureId) override;

private:

    int orientation = kGuassionBlurHorizontal;
};


#endif //SHADERIMAGE_IMAGEGAUSSIONBLURFILTER_H
