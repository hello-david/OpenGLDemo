//
// Created by David.Dai on 2018/10/30.
//

#ifndef ANDORID_GLTEXTURE_HPP
#define ANDORID_GLTEXTURE_HPP

#include <stdio.h>
#include <string>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>
#include "GLMath.hpp"

namespace GLDemo {
    class GLTexture {

    private:
        GLuint mTexture = 0;
        float mWidth = 0;
        float mHeight = 0;

        void initTexture(void *data, int width, int height, GLuint imageFormat, GLuint texureFormat);
    public:
        GLTexture(const std::string &imagePath);
        ~GLTexture();

        _Size size() const { return {mWidth, mHeight}; };
        GLuint texture() const { return mTexture; };

        void loadTexutreFromPath(const std::string &path);
    };
}


#endif //ANDORID_GLTEXTURE_HPP
