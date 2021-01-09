//
// Created by cw on 2021/1/9.
//

#ifndef SHADERIMAGE_IMAGEBASETWOPATHFILTER_H
#define SHADERIMAGE_IMAGEBASETWOPATHFILTER_H


#include "ImageBaseFilter.h"

class ImageBaseTwoPathFilter : public ImageBaseFilter {

public:

    ImageBaseTwoPathFilter(int type);

    ~ImageBaseTwoPathFilter();

    void init() override;

    void setImageViewSize(int width, int height) override;

    void doFrame(void *pixels, GLenum format) override;

    void genFrameBuffer(int width, int height);

    virtual void initTwoFilters(int type) = 0;

protected:

    ImageBaseFilter *horizontalFilter;

    ImageBaseFilter *verticalFilter;

    GLuint frameBufferTextureId = 0;

    GLuint frameBuffer = 0;

};


#endif //SHADERIMAGE_IMAGEBASETWOPATHFILTER_H
