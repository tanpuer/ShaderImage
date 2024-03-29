//
// Created by templechen on 2020/10/19.
//

#include "ImageBaseFilter.h"
#include "../../base/gl_utils.h"

ImageBaseFilter::~ImageBaseFilter() {
    glDeleteProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteTextures(1, &texture);
    ALOGD("ImageBaseFilter release")
}

void ImageBaseFilter::init() {
    vertexShader = loadShader(GL_VERTEX_SHADER, getVertexShaderString());
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, getFragmentShaderString());
    program = createShaderProgram(vertexShader, fragmentShader);
    glUseProgram(program);
    textureMatrix = std::make_unique<ESMatrix>();
    setIdentityM(textureMatrix.get());
    texture = createTexture(GL_TEXTURE_2D);
//    ALOGD("program %d %d %d %d", program, fragmentShader, vertexShader, texture)
    checkGLError("ImageBaseFilter");
}

void ImageBaseFilter::setImageViewSize(int width, int height) {
    this->bitmapWidth = width;
    this->bitmapHeight = height;
}

void ImageBaseFilter::doFrame(void *pixels, GLenum format) {
    glUseProgram(program);

    //attribute
    GLint vertexCount = sizeof(imageVertex) / (sizeof(imageVertex[0]) * 2);
    aPositionLocation = glGetAttribLocation(program, aPosition);
    glEnableVertexAttribArray(aPositionLocation);
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, imageVertex);

    aTextureCoordinateLocation = glGetAttribLocation(program, aTextureCoordinate);
    checkGLError("glGetAttribLocation");
    glEnableVertexAttribArray(aTextureCoordinateLocation);
    glVertexAttribPointer(aTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 8, imageTexture);

    //uniform
    uTextureMatrixLocation = glGetUniformLocation(program, uTextureMatrix);
    glUniformMatrix4fv(uTextureMatrixLocation, 1, GL_FALSE, textureMatrix->m);

    //texture
    uTextureLocation = glGetUniformLocation(program, uTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    checkGLError("glBindTexture");

    glTexImage2D(GL_TEXTURE_2D, 0, format, this->bitmapWidth, this->bitmapHeight, 0, format,
                 GL_UNSIGNED_BYTE, pixels);
    checkGLError("glTexImage2D");
    glUniform1i(uTextureLocation, 0);

    //draw triangles
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);

    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLError("doFrameOver");
}

const char *ImageBaseFilter::getVertexShaderString() {
    return GET_STR(
            attribute vec4 aPosition;
            uniform mat4 uTextureMatrix;
            attribute vec4 aTextureCoordinate;
            varying vec2 vTextureCoord;
            void main() {
                vTextureCoord = (uTextureMatrix * aTextureCoordinate).xy;
                gl_Position = aPosition;
            }
    );
}

const char *ImageBaseFilter::getFragmentShaderString() {
    return GET_STR(
            precision highp float;
            varying vec2 vTextureCoord;
            uniform sampler2D uTexture;
            void main() {
                gl_FragColor = texture2D(uTexture, vTextureCoord);
            }
    );
}

bool ImageBaseFilter::isSameType(int newType) const {
    return this->type == newType;
}
