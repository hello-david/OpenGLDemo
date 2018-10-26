//
// Created by David.Dai on 2018/10/8.
//
#ifndef GLRENDER_HPP
#define GLRENDER_HPP

#include <stdio.h>
#include <string>
#include "GLProgram.hpp"

extern const char* kVertexShader;
extern const char* kFragmentShader;

struct _Size {
    float width;
    float height;

    _Size(float w = 0, float h = 0){
        width  = w;
        height = h;
    }
};

namespace GLDemo {
    class GLRender {
    private:
        GLDemo::GLProgram *mProgram = nullptr;
        GLuint mPositionAttribute = 0;
        GLuint mInputTextureCoorAttribute = 0;
        GLuint mTextureUniform = 0;

    public:
        GLRender();
        ~GLRender();

        void render(const _Size &size);
    };
}

#endif