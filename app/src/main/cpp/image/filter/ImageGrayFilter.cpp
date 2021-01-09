//
// Created by cw on 2020/10/26.
//

#include "ImageGrayFilter.h"

const char *ImageGrayFilter::getFragmentShaderString() {
    return GET_STR(
            precision highp float;
            varying vec2 vTextureCoord;
            uniform sampler2D uTexture;
            void main() {
                vec4 color = texture2D(uTexture, vTextureCoord);
                float fGrayColor = (0.3*color.r + 0.59*color.g + 0.11*color.b);
                gl_FragColor = vec4(fGrayColor, fGrayColor, fGrayColor, color.a);
            }
    );
}

ImageGrayFilter::ImageGrayFilter(int type) : ImageBaseFilter(type) {

}

ImageGrayFilter::~ImageGrayFilter() {

}
