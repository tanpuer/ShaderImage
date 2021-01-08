//
// Created by cw on 2021/1/8.
//

#include "ImageGaussionBlurFilter.h"

ImageGaussionBlurFilter::ImageGaussionBlurFilter(int type) : ImageBaseFilter(type) {

}

ImageGaussionBlurFilter::~ImageGaussionBlurFilter() {

}

const char *ImageGaussionBlurFilter::getFragmentShaderString() {
    return GET_STR(
            precision mediump float;
            uniform sampler2D uTexture;
            varying vec2 vTextureCoord;
            const int GAUSSIAN_SAMPLES = 9;
            varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];
            void main() {
                vec3 sum = vec3(0.0);
                vec4 fragColor = texture2D(uTexture, vTextureCoord);
                sum += texture2D(uTexture, blurCoordinates[0]).rgb * 0.05;
                sum += texture2D(uTexture, blurCoordinates[1]).rgb * 0.09;
                sum += texture2D(uTexture, blurCoordinates[2]).rgb * 0.12;
                sum += texture2D(uTexture, blurCoordinates[3]).rgb * 0.15;
                sum += texture2D(uTexture, blurCoordinates[4]).rgb * 0.18;
                sum += texture2D(uTexture, blurCoordinates[5]).rgb * 0.15;
                sum += texture2D(uTexture, blurCoordinates[6]).rgb * 0.12;
                sum += texture2D(uTexture, blurCoordinates[7]).rgb * 0.09;
                sum += texture2D(uTexture, blurCoordinates[8]).rgb * 0.05;
                gl_FragColor = vec4(sum, fragColor.a);
            }
    );
}

const char *ImageGaussionBlurFilter::getVertexShaderString() {
    return GET_STR(
            attribute vec4 aPosition;
            uniform mat4 uTextureMatrix;
            attribute vec4 aTextureCoordinate;
            varying vec2 vTextureCoord;
            const int GAUSSIAN_SAMPLES = 9;
            varying vec2 blurCoordinates[GAUSSIAN_SAMPLES];
            void main() {
                vTextureCoord = (uTextureMatrix * aTextureCoordinate).xy;
                gl_Position = aPosition;
                // Calculate the positions for the blur +
                int multiplier = 0;
                vec2 blurStep;
                for (int i = 0; i < GAUSSIAN_SAMPLES; i++) {
                    multiplier = (i - ((GAUSSIAN_SAMPLES - 1) / 2));
                    // Blur in x (horizontal) +
                    blurStep = float(multiplier) * vec2(0.001, 0.001);
                    blurCoordinates[i] = vTextureCoord.xy + blurStep;
                }
            }
    );
}
