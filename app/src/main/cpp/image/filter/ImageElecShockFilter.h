//
// Created by cw on 2020/10/26.
//

#ifndef SHADERIMAGE_IMAGEELECSHOCKFILTER_H
#define SHADERIMAGE_IMAGEELECSHOCKFILTER_H


#include "ImageBaseFilter.h"

class ImageElecShockFilter : public ImageBaseFilter{

public:

    ImageElecShockFilter(int type);

    ~ImageElecShockFilter();

    virtual const char *getFragmentShaderString();

};


#endif //SHADERIMAGE_IMAGEELECSHOCKFILTER_H
