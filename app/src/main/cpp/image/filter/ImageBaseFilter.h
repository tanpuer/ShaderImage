//
// Created by templechen on 2020/10/19.
//

#ifndef SHADERIMAGE_IMAGEBASEFILTER_H
#define SHADERIMAGE_IMAGEBASEFILTER_H


#include <GLES3/gl3.h>
#include "../../base/matrix_util.h"
#include "memory"

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

    ImageBaseFilter(int type) { this->type = type; };

    ~ImageBaseFilter();

    virtual void init();

    virtual void setImageViewSize(int width, int height);

    bool isSameType(int newType) const;

    virtual void doFrame(void *pixels, GLenum format);

    virtual void doFrame2(GLuint frameBufferTextureId) {}

    virtual const char *getVertexShaderString();

    virtual const char *getFragmentShaderString();

protected:

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

    std::unique_ptr<ESMatrix> textureMatrix;

    int bitmapWidth;
    int bitmapHeight;

    int type;
};


#endif //SHADERIMAGE_IMAGEBASEFILTER_H
