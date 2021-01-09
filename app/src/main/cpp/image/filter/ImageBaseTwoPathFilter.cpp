//
// Created by cw on 2021/1/9.
//

#include <base/gl_utils.h>
#include "ImageBaseTwoPathFilter.h"

ImageBaseTwoPathFilter::ImageBaseTwoPathFilter(int type) : ImageBaseFilter(type) {

}

ImageBaseTwoPathFilter::~ImageBaseTwoPathFilter() {

}

void ImageBaseTwoPathFilter::init() {
    initTwoFilters(type);
    horizontalFilter->init();
    verticalFilter->init();
}

void ImageBaseTwoPathFilter::setImageViewSize(int width, int height) {
    genFrameBuffer(width, height);
    horizontalFilter->setImageViewSize(width, height);
    verticalFilter->setImageViewSize(width, height);
}

void ImageBaseTwoPathFilter::doFrame(void *pixels, GLenum format) {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           frameBufferTextureId, 0);
    horizontalFilter->doFrame(pixels, format);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    verticalFilter->doFrame2(frameBufferTextureId);
}

void ImageBaseTwoPathFilter::genFrameBuffer(int width, int height) {
    glGenFramebuffers(1, &frameBuffer);
    frameBufferTextureId = createTexture(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, frameBufferTextureId);
    glViewport(0, 0, width, height);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        ALOGE("fbo glFramebufferTexture2D error")
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ALOGD("fbo genFrameBuffer %d %d", frameBuffer, frameBufferTextureId)
}
