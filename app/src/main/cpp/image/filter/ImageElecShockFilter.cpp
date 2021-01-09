//
// Created by cw on 2020/10/26.
//

#include "ImageElecShockFilter.h"

const char *ImageElecShockFilter::getFragmentShaderString() {
    return GET_STR(
            precision highp float;
            varying vec2 vTextureCoord;
            uniform sampler2D uTexture;
            void main() {
                vec4 color = texture2D(uTexture, vTextureCoord);
                gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, 1.0);
            }
    );
}

ImageElecShockFilter::ImageElecShockFilter(int type) : ImageBaseFilter(type) {

}

ImageElecShockFilter::~ImageElecShockFilter() {

}
