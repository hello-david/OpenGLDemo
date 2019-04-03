//
// Created by David.Dai on 2018/11/23.
//

#ifndef ANDORID_GLTEXTUREBLENDINGRENDER_HPP
#define ANDORID_GLTEXTUREBLENDINGRENDER_HPP

#include <stdio.h>
#include <string>
#include "GLMath.hpp"
#include "GLRender.hpp"
#include "GLTexture.hpp"

namespace GLDemo {
    class GLTextureBlendingRender : public GLRender {

    private:
        GLTexture *mBackTexture = nullptr;
        GLTexture *mFrontTexture = nullptr;

    public:
        GLTextureBlendingRender(const std::string &backImagePath, const std::string &frontImagePath);
        ~GLTextureBlendingRender();

        void render(const _Size &size);
    };
}

#endif //ANDORID_GLTEXTUREBLENDINGRENDER_HPP
