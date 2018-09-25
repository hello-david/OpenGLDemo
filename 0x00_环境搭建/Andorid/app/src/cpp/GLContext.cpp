//
// Created by David.Dai on 2018/9/21.
//

#include "GLContext.hpp"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <jni.h>

using namespace GLDemo;

GLContext::GLContext(GLRenderAPI apiLevel, ANativeWindow *window) {

    // EGL配置
    EGLint confAttr[15] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,// very important!
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,       // we will create a pixelbuffer surface
            EGL_RED_SIZE,   8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE,  8,
            EGL_ALPHA_SIZE, 8, // if you need the alpha channel
            EGL_DEPTH_SIZE, 16,// if you need the depth buffer
            EGL_NONE
    };

    // EGL Context配置
    EGLint ctxAttr[3] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, // very important!
            EGL_NONE
    };
    switch (apiLevel) {
        case GLRenderAPIES2 :
            confAttr[1] = EGL_OPENGL_ES2_BIT;
            ctxAttr[1] = 2;
            break;
        case GLRenderAPIES3:
            confAttr[1] = EGL_OPENGL_ES3_BIT_KHR;
            ctxAttr[1] = 3;
            break;
        default:
            break;
    }

    // 获取默认Display（默认显示设备对应的Renderbuffer）
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    checkEglError("eglCreateWindowSurface");
    if (display == EGL_NO_DISPLAY) {
        throw "eglGetDisplay failed";
    }

    // 初始化EGL
    EGLint eglMajVers, eglMinVers;
    EGLBoolean initResult = eglInitialize(display, &eglMajVers, &eglMinVers);
    assert(initResult);

    // 配置EGL
    EGLConfig config  = NULL;
    EGLint numConfigs = 0;
    if (!eglChooseConfig(display, confAttr, &config, 1, &numConfigs) || numConfigs != 1) {
        if (apiLevel == GLRenderAPIES3) {
            confAttr[1] = EGL_OPENGL_ES2_BIT;
            ctxAttr[1] = 2;
            mRenderAPI = GLRenderAPIES2;
            if (!eglChooseConfig(display, confAttr, &config, 1, &numConfigs) || numConfigs != 1) {
                assert(false);
            }
        } else{
            assert(false);
        }
    }

    // 创建Surface（Framebuffer）
    int surfaceAttribs[] = { EGL_NONE };
    EGLSurface surface = eglCreateWindowSurface(display, config, window, surfaceAttribs);
    checkEglError("eglCreateWindowSurface");

    // 创建EGL Context
    EGLContext context = eglCreateContext(display, config, nullptr, ctxAttr);
    if (context == EGL_NO_CONTEXT) {
        assert(false);
    }

    mWindow  = window;
    mSurface = surface;
    mContext = context;
    mDisplay = display;
    mConfig  = config;
}

GLContext::~GLContext() {
    eglDestroySurface(mDisplay, mSurface);
    eglDestroyContext(mDisplay, mContext);
    eglTerminate(mDisplay);
    eglReleaseThread();

    if (mWindow) {
        ANativeWindow_release(mWindow);
    }
}

void GLContext::swapToScreen() {
    eglSwapBuffers(mDisplay, mSurface);
}

void GLContext::use() {
    eglMakeCurrent(mDisplay, mSurface, mSurface , mContext);
}

bool GLContext::isCurrentContext() {
    return eglGetCurrentContext() == mContext;
}

void GLContext::checkEglError(std::string msg) {
    int error;
    if ((error = eglGetError()) != EGL_SUCCESS) {
        throw msg + ": EGL error: 0x" +   std::to_string(error);
    }
}

// JNI 层
#ifdef __cplusplus
extern "C"{
#endif

bool currentContextIsExist() {
    return eglGetCurrentContext() != NULL;
}

JNIEXPORT jlong JNICALL
Java_com_example_david_a0x00_OpenGLContext_createNativeGLContext(JNIEnv *env, jobject instance,
                                                                 jint glVersion, jobject surface) {

    GLDemo::GLRenderAPI api = GLDemo::GLRenderAPIES2;
    switch (glVersion) {
        case 1:
            api = GLDemo::GLRenderAPIES1;
            break;
        case 2:
            api = GLDemo::GLRenderAPIES2;
            break;
        case 3:
            api = GLDemo::GLRenderAPIES3;
            break;
        default:break;
    }
    return reinterpret_cast<long>(new GLDemo::GLContext(api, ANativeWindow_fromSurface(env, surface)));
}

void Java_com_example_david_a0x00_OpenGLContext_use(JNIEnv * env, jobject obj, jlong nativeGLContext) {
    GLDemo::GLContext *glContext = reinterpret_cast<GLDemo::GLContext *>(nativeGLContext);

    if (glContext) {
        glContext->use();
    }
}

void Java_com_example_david_a0x00_OpenGLContext_dispose(JNIEnv * env, jobject obj, jlong nativeGLContext) {
    GLDemo::GLContext *glContext = reinterpret_cast<GLDemo::GLContext *>(nativeGLContext);
    if (glContext) {
        delete glContext;
    }
}

void Java_com_example_david_a0x00_OpenGLContext_swapToScreen(JNIEnv * env, jobject obj, jlong nativeGLContext) {
    GLDemo::GLContext *glContext = reinterpret_cast<GLDemo::GLContext *>(nativeGLContext);

    if (glContext) {
        glContext->swapToScreen();
    }
}

#ifdef __cplusplus
}
#endif
