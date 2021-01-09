//
// Created by cw on 2021/1/9.
//

#include "ImageGaussionTwoPathFilter.h"
#include "ImageGaussionBlurFilter.h"

void ImageGaussionTwoPathFilter::initTwoFilters(int type) {
    horizontalFilter = new ImageGaussionBlurFilter(type,
                                                   ImageGaussionBlurFilter::kGuassionBlurHorizontal);
    verticalFilter = new ImageGaussionBlurFilter(type,
                                                 ImageGaussionBlurFilter::kGaussionBlurVertical);
}

ImageGaussionTwoPathFilter::ImageGaussionTwoPathFilter(int type) : ImageBaseTwoPathFilter(type) {

}

ImageGaussionTwoPathFilter::~ImageGaussionTwoPathFilter() {

}
