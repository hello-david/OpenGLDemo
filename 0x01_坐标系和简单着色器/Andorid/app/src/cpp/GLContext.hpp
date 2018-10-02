//
// Created by David.Dai on 2018/9/21.
//

#ifndef ANDORID_GLCONTEXT_HPP
#define ANDORID_GLCONTEXT_HPP

#include <stdio.h>
#include <string>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

namespace GLDemo {

    enum GLRenderAPI{
        GLRenderAPIES1 = 1,
        GLRenderAPIES2 = 2,
        GLRenderAPIES3 = 3,
    };

    class GLContext {

    protected:

        EGLContext mContext = 0;
        EGLDisplay mDisplay = 0;
        EGLSurface mSurface = 0;
        EGLConfig  mConfig  = 0;
        ANativeWindow *mWindow = 0;

        // 默认使用OpenGLES2
        GLRenderAPI mRenderAPI = GLRenderAPIES2;

        void checkEglError(std::string msg);

    public:
        GLContext(GLRenderAPI apiLevel, ANativeWindow *window = nullptr);
        ~GLContext();

        bool isCurrentContext();
        void use();
        void swapToScreen();
    };

}


#endif //ANDORID_GLCONTEXT_HPP
