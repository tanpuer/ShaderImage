//
// Created by cw on 2021/1/8.
//

#ifndef SHADERIMAGE_IMAGEGAUSSIONBLURFILTER_H
#define SHADERIMAGE_IMAGEGAUSSIONBLURFILTER_H


#include "ImageBaseFilter.h"

class ImageGaussionBlurFilter : public ImageBaseFilter {

public:

    ImageGaussionBlurFilter(int type);

    ~ImageGaussionBlurFilter();

    const char *getFragmentShaderString() override;

    const char *getVertexShaderString() override;
};


#endif //SHADERIMAGE_IMAGEGAUSSIONBLURFILTER_H
