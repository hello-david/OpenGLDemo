//
// Created by David.Dai on 2018/10/30.
//

#ifndef ANDORID_GLTEXTURERENDER_HPP
#define ANDORID_GLTEXTURERENDER_HPP

#include <stdio.h>
#include "GLMath.hpp"
#include "GLRender.hpp"
#include "GLTexture.hpp"

namespace GLDemo {
    class GLTextureRender : public GLRender {

    protected:
        GLfloat *textureVertexForViewSize(_Size viewSize , _Size textureSize);
        GLTexture *mTexture = nullptr;

    public:
        GLTextureRender(const std::string &imagePath);
        ~GLTextureRender();

        virtual void render(const _Size &size);
    };
}


#endif //ANDORID_GLTEXTURERENDER_HPP
