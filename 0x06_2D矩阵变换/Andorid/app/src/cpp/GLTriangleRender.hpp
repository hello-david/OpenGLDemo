//
// Created by David.Dai on 2018/10/26.
//

#ifndef ANDORID_GLTRIANGLERENDER_HPP
#define ANDORID_GLTRIANGLERENDER_HPP

#include "GLRender.hpp"

namespace GLDemo {
    class GLTriangleRender : public GLRender {
    private:
        void updateAnimation();

    protected:
        GLuint mVBO;

    public:
        GLTriangleRender();
        ~GLTriangleRender();

        void render(const _Size &size);
    };
}

#endif //ANDORID_GLTRIANGLERENDER_HPP
