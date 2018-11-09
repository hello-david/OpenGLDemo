//
// Created by David.Dai on 2018/10/8.
//
#ifndef GLRENDER_HPP
#define GLRENDER_HPP

#include <stdio.h>
#include <string>
#include "GLMath.hpp"
#include "GLProgram.hpp"

extern const char* kVertexShader;
extern const char* kFragmentShader;

namespace GLDemo {
    class GLRender {
    protected:
        GLDemo::GLProgram *mProgram = nullptr;
        GLuint mPositionAttribute = 0;
        GLuint mInputTextureCoorAttribute = 0;
        GLuint mTextureUniform = 0;

    public:
        GLRender();
        GLRender(const std::string &vertexStr, const std::string &fragmentStr);
        ~GLRender();

        virtual void render(const _Size &size);
    };
}

#endif