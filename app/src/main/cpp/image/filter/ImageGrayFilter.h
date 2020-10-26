//
// Created by cw on 2020/10/26.
//

#ifndef SHADERIMAGE_IMAGEGRAYFILTER_H
#define SHADERIMAGE_IMAGEGRAYFILTER_H


#include "ImageBaseFilter.h"

class ImageGrayFilter : public ImageBaseFilter {

public:

    ImageGrayFilter(int type);

    ~ImageGrayFilter();

    const char *getFragmentShaderString() override;

};


#endif //SHADERIMAGE_IMAGEGRAYFILTER_H
