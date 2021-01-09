//
// Created by cw on 2021/1/9.
//

#ifndef SHADERIMAGE_IMAGEGAUSSIONTWOPATHFILTER_H
#define SHADERIMAGE_IMAGEGAUSSIONTWOPATHFILTER_H


#include "ImageBaseTwoPathFilter.h"

class ImageGaussionTwoPathFilter : public ImageBaseTwoPathFilter {

public:

    ImageGaussionTwoPathFilter(int type);

    ~ImageGaussionTwoPathFilter();

    void initTwoFilters(int type) override;

};


#endif //SHADERIMAGE_IMAGEGAUSSIONTWOPATHFILTER_H
