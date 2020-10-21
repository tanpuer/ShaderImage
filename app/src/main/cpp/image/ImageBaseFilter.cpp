//
// Created by templechen on 2020/10/19.
//

#include "ImageBaseFilter.h"
#include "../base/gl_utils.h"

#define GET_STR(x) #x

static const char *VERTEX_SHADER_STR = GET_STR(
        attribute vec4 aPosition;
        uniform mat4 uTextureMatrix;
        attribute vec4 aTextureCoordinate;
        varying vec2 vTextureCoord;
        void main() {
            vTextureCoord = (uTextureMatrix * aTextureCoordinate).xy;
            gl_Position = aPosition;
        }
);

static const char *FRAGMENT_SHADER_STR = GET_STR(
        precision highp float;
        varying vec2 vTextureCoord;
        uniform sampler2D uTexture;
        void main() {
            gl_FragColor = vec4(texture2D(uTexture, vTextureCoord).xyz, 0.4);
//            gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
);

ImageBaseFilter::ImageBaseFilter() {
    vertexShader = loadShader(GL_VERTEX_SHADER, VERTEX_SHADER_STR);
    fragmentShader = loadShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER_STR);
    program = createShaderProgram(vertexShader, fragmentShader);
    glUseProgram(program);
    textureMatrix = new ESMatrix();
    setIdentityM(textureMatrix);
    texture = createTexture(GL_TEXTURE_2D);
    ALOGD("program %d %d %d %d", program, fragmentShader, vertexShader, texture)
    checkGLError("ImageBaseFilter");
}

ImageBaseFilter::~ImageBaseFilter() {
    glDeleteProgram(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteTextures(1, &texture);
    delete textureMatrix;
    ALOGD("ImageBaseFilter release")
}

void ImageBaseFilter::setImageViewSize(int width, int height) {
    this->bitmapWidth = width;
    this->bitmapHeight = height;
}

void ImageBaseFilter::doFrame(void *pixels, GLenum format) {
    ALOGD("doFrame %d", program)
    glUseProgram(program);

    //attribute
    GLint vertexCount = sizeof(imageVertex) / (sizeof(imageVertex[0]) * 2);
    aPositionLocation = glGetAttribLocation(program, aPosition);
    glEnableVertexAttribArray(aPositionLocation);
    glVertexAttribPointer(aPositionLocation, 2, GL_FLOAT, GL_FALSE, 8, imageVertex);
    ALOGD("aPositionLocation %d", aPositionLocation)

    aTextureCoordinateLocation = glGetAttribLocation(program, aTextureCoordinate);
    checkGLError("glGetAttribLocation");
    glEnableVertexAttribArray(aTextureCoordinateLocation);
    glVertexAttribPointer(aTextureCoordinateLocation, 2, GL_FLOAT, GL_FALSE, 8, imageTexture);
    ALOGD("aTextureCoordinateLocation %d", aTextureCoordinateLocation)

    //uniform
    uTextureMatrixLocation = glGetUniformLocation(program, uTextureMatrix);
    glUniformMatrix4fv(uTextureMatrixLocation, 1, GL_FALSE, textureMatrix->m);
    ALOGD("uTextureMatrixLocation %d", uTextureMatrixLocation)

    //texture
    uTextureLocation = glGetUniformLocation(program, uTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    checkGLError("glBindTexture");

    glTexImage2D(GL_TEXTURE_2D, 0, format, this->bitmapWidth, this->bitmapHeight, 0, format,
                 GL_UNSIGNED_BYTE, pixels);
    checkGLError("glTexImage2D");
    glUniform1i(uTextureLocation, 0);
    ALOGD("uTextureLocation %d", uTextureLocation)

    //draw triangles
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glDisableVertexAttribArray(aPositionLocation);
    glDisableVertexAttribArray(aTextureCoordinateLocation);

    glBindTexture(GL_TEXTURE_2D, 0);
    checkGLError("doFrameOver");
}
