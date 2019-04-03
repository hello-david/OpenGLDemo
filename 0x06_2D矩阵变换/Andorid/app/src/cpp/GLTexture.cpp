//
// Created by David.Dai on 2018/10/30.
//

#include "GLTexture.hpp"

// 需要加上这个宏不然编译器会编译失败
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace GLDemo;

GLTexture::GLTexture(const std::string &imagePath) {
    loadTexutreFromPath(imagePath);
}

GLTexture::~GLTexture() {
    if (mTexture) {
        glDeleteTextures(1, &mTexture);
        mTexture = 0;
    }
}

void GLTexture::loadTexutreFromPath(const std::string &path) {
    if (!path.length()) {
        return ;
    }

    if (mTexture) {
        glDeleteTextures(1, &mTexture);
        mTexture = 0;
    }

    int width = 0;
    int height = 0;
    int imageFormat = 0;

    // 通过stb这个库加载图片
    void *imageData = stbi_load(path.c_str(), &width, &height, &imageFormat, STBI_default);

    // 获取图片格式和像素对齐数
    int alignment = 1;
    GLuint glImageFormat = GL_RGBA;
    switch (imageFormat) {
        case STBI_grey:
            glImageFormat = GL_ALPHA;
            break;
        case STBI_grey_alpha:
            glImageFormat = GL_RG;
            break;
        case STBI_rgb:
            glImageFormat = GL_RGB;
            break;
        case STBI_rgb_alpha:
        default:
            alignment = 4;
            break;
    }

    // 设置像素字节对齐数，默认4字节
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    initTexture(imageData, width, height, glImageFormat, glImageFormat);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    STBI_FREE(imageData);
}

void GLTexture::initTexture(void *data, int width, int height, GLuint imageFormat, GLuint texureFormat) {
    // 生成并绑定Texure
    glGenTextures(1, &mTexture);
    glBindTexture(GL_TEXTURE_2D, mTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, texureFormat, GL_UNSIGNED_BYTE, data);

    // 设置纹理单元的滤波方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 设置纹理单元的环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    mWidth = width;
    mHeight = height;
}
