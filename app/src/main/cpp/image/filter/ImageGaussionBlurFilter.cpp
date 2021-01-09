//
// Created by cw on 2021/1/8.
//

#include <base/gl_utils.h>
#include "ImageGaussionBlurFilter.h"

ImageGaussionBlurFilter::ImageGaussionBlurFilter(int type, int orientation) : ImageBaseFilter(
        type) {
    this->orientation = orientation;
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
    if (orientation == kGuassionBlurHorizontal) {
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
                        blurStep = float(multiplier) * vec2(0.03, 0.0);
                        blurCoordinates[i] = vTextureCoord.xy + blurStep;
                    }
                }
        );
    } else {
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
                        blurStep = float(multiplier) * vec2(0.0, 0.03);
                        blurCoordinates[i] = vTextureCoord.xy + blurStep;
                    }
                }
        );
    }
}

void ImageGaussionBlurFilter::doFrame2(GLuint frameBufferTextureId) {
    glUseProgram(program);

    //attribute
    GLint vertexCount = sizeof(imageVertex) / (sizeof(imageVertex[0]) * 2);
    aPositionLocation = glGetAttribLocation(program, aPosition);
    glEnableVertexAttribArray(aPositionLocation);
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, imageVertex);
//    ALOGD("aPositionLocation %d", aPositionLocation)

    aTextureCoordinateLocation = glGetAttribLocation(program, aTextureCoordinate);
    checkGLError("glGetAttribLocation");
    glEnableVertexAttribArray(aTextureCoordinateLocation);
    glVertexAttribPointer(aTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 8, imageTexture);
//    ALOGD("aTextureCoordinateLocation %d", aTextureCoordinateLocation)

    //uniform
    uTextureMatrixLocation = glGetUniformLocation(program, uTextureMatrix);
    glUniformMatrix4fv(uTextureMatrixLocation, 1, GL_FALSE, textureMatrix->m);
//    ALOGD("uTextureMatrixLocation %d", uTextureMatrixLocation)

    uTextureLocation = glGetUniformLocation(program, uTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frameBufferTextureId);
    checkGLError("glBindTexture");

    checkGLError("glTexImage2D");
    glUniform1i(uTextureLocation, 0);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);

    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLError("doFrameOver");
}
