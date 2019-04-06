//
// Created by David.Dai on 2019/4/6.
//

#ifndef ANDORID_GLMATRIXRENDER_HPP
#define ANDORID_GLMATRIXRENDER_HPP

#include "GLRender.hpp"

namespace GLDemo {
    class GLMatrixRender : public GLRender {

    public:
        GLMatrixRender();

        void render(const _Size &size);
    };
}

#endif //ANDORID_GLMATRIXRENDER_HPP
