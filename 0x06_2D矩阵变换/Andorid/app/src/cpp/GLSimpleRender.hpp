//
// Created by David.Dai on 2018/11/14.
//

#ifndef ANDORID_GLSIMPLERENDER_HPP
#define ANDORID_GLSIMPLERENDER_HPP

#include <stdio.h>
#include <string>
#include "GLMath.hpp"
#include "GLRender.hpp"
#include "GLTexture.hpp"

extern const char *kContrastFragmentShader;
extern const char *kLuminanceFragmentShader;
extern const char *kSaturationFragmentShader;
extern const char *kSharpenVertexShader;
extern const char *kSharpenFragmentShader;
extern const char *kVignetteFragmentShader;

namespace GLDemo {
    class GLSimpleRender : public GLRender {

    private:
        float mPercent = 1.0f;
        GLint mContrastUniform, mLuminanceUniform, mSaturationUniform;
        GLint mSharpnessUniform, mImageWidthFactorUniform, mImageHeightFactorUniform;
        GLint mVignetteCenterUniform, mVignetteColorUniform, mVignetteStartUniform, mVignetteEndUniform;

        GLVector2 mVignetteCenter = { 0.5f, 0.5f };
        GLVector3 mVignetteColor = { 1.0f, 1.0f, 1.0f };
        float mVignetteStart= 0.3f, mVignetteEnd = 0.75f;

    protected:
        GLTexture *mTexture = nullptr;

    public:
        GLSimpleRender(const std::string &imagePath);
        ~GLSimpleRender();

        void setEffectPercent(float percent) { mPercent = 0.5 + percent; };
        virtual void render(const _Size &size);
    };
}


#endif //ANDORID_GLSIMPLERENDER_HPP
