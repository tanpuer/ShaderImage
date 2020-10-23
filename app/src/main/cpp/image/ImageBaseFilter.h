//
// Created by templechen on 2020/10/19.
//

#ifndef SHADERIMAGE_IMAGEBASEFILTER_H
#define SHADERIMAGE_IMAGEBASEFILTER_H


#include <GLES3/gl3.h>
#include "../base/matrix_util.h"

#define GET_STR(x) #x

static GLfloat imageVertex[] = {
        1.0f, 1.0f,
        -1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
        1.0f, -1.0f,
};

static GLfloat imageTexture[] = {
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

class ImageBaseFilter {

public:

    ImageBaseFilter() {};

    ~ImageBaseFilter();

    virtual void init();

    void setImageViewSize(int width, int height);

    virtual void doFrame(void *pixels, GLenum format);

    virtual const char *getVertexShaderString();

    virtual const char *getFragmentShaderString();

private:

    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint program = 0;
    GLuint texture = 0;

    const char *aPosition = "aPosition";
    const char *uTextureMatrix = "uTextureMatrix";
    const char *aTextureCoordinate = "aTextureCoordinate";
    const char *uTexture = "uTexture";

    GLint aPositionLocation = -1;
    GLint uTextureMatrixLocation = -1;
    GLint aTextureCoordinateLocation = -1;
    GLint uTextureLocation = -1;

    ESMatrix *textureMatrix;

    int bitmapWidth;
    int bitmapHeight;
};


#endif //SHADERIMAGE_IMAGEBASEFILTER_H
