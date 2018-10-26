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

union _GLVector2  {
    struct { float x, y; };
    struct { float r, g; };
    struct { float s, t; };
    float v[2];
};
typedef union _GLVector2 GLVector2;

union _GLKVector3  {
    struct { float x, y, z; };
    struct { float r, g, b; };
    struct { float s, t, p; };
    float v[3];
};
typedef union _GLVector3 GLVector3;

namespace GLDemo {
    class GLRender {
    protected:
        GLDemo::GLProgram *mProgram = nullptr;
        GLuint mPositionAttribute = 0;
        GLuint mInputTextureCoorAttribute = 0;
        GLuint mTextureUniform = 0;

    public:
        GLRender();
        ~GLRender();

        virtual void render(const _Size &size);
    };
}

#endif