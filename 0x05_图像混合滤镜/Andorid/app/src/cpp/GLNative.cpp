//
// Created by David.Dai on 2018/10/9.
//

#include <EGL/egl.h>
#include <jni.h>
#include <android/native_window_jni.h>
#include "GLNative.hpp"

#ifdef __cplusplus
extern "C"{
#endif

// ------------- Context
bool currentContextIsExist() {
    return eglGetCurrentContext() != NULL;
}

jlong Java_com_example_david_a0x00_OpenGLContext_createNativeGLContext(JNIEnv *env, jobject instance,
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
    return reinterpret_cast<long>(new GLDemo::GLContext(api, nullptr, ANativeWindow_fromSurface(env, surface)));
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

// ------------- Program
jlong Java_com_example_david_a0x00_OpenGLProgram_createProgram(JNIEnv *env, jobject instance,
                                                         jstring vertexString_,
                                                         jstring fragmentString_) {
    const char *vertexString = env->GetStringUTFChars(vertexString_, 0);
    const char *fragmentString = env->GetStringUTFChars(fragmentString_, 0);

    GLDemo::GLProgram *program = new GLDemo::GLProgram(vertexString, fragmentString);

    env->ReleaseStringUTFChars(vertexString_, vertexString);
    env->ReleaseStringUTFChars(fragmentString_, fragmentString);

    return reinterpret_cast<long>(program);
}

void Java_com_example_david_a0x00_OpenGLProgram_dispose__J(JNIEnv *env, jobject instance,
                                                      jlong nativeProgram) {
    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    if (glProgram) {
        delete glProgram;
    }
}

jstring Java_com_example_david_a0x00_OpenGLProgram_programLog__J(JNIEnv *env, jobject instance,
                                                         jlong nativeProgram) {


    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    auto returnValue = glProgram->programLog();

    return env->NewStringUTF(returnValue.c_str());
}

jstring Java_com_example_david_a0x00_OpenGLProgram_vertexShaderLog__J(JNIEnv *env, jobject instance,
                                                              jlong nativeProgram) {


    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    auto returnValue = glProgram->vertexShaderLog();

    return env->NewStringUTF(returnValue.c_str());
}

jstring Java_com_example_david_a0x00_OpenGLProgram_fragmentShaderLog__J(JNIEnv *env, jobject instance,
                                                                jlong nativeProgram) {


    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    auto returnValue = glProgram->fragmentShaderLog();

    return env->NewStringUTF(returnValue.c_str());
}

jint Java_com_example_david_a0x00_OpenGLProgram_attributeIndex__JLjava_lang_String_2(JNIEnv *env,
                                                                                jobject instance,
                                                                                jlong nativeProgram,
                                                                                jstring attrName_) {
    const char *attrName = env->GetStringUTFChars(attrName_, 0);

    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    int attributeIndex = glProgram->attributeIndex(attrName);

    env->ReleaseStringUTFChars(attrName_, attrName);
    return attributeIndex;
}

jint Java_com_example_david_a0x00_OpenGLProgram_uniformIndex__JLjava_lang_String_2(JNIEnv *env,
                                                                              jobject instance,
                                                                              jlong nativeProgram,
                                                                              jstring uniformName_) {
    const char *uniformName = env->GetStringUTFChars(uniformName_, 0);

    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    int uniformIndex = glProgram->uniformIndex(uniformName);

    env->ReleaseStringUTFChars(uniformName_, uniformName);
    return uniformIndex;
}

jboolean Java_com_example_david_a0x00_OpenGLProgram_linkShader__J(JNIEnv *env, jobject instance,
                                                         jlong nativeProgram) {

    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    return glProgram->link();
}

void Java_com_example_david_a0x00_OpenGLProgram_use__J(JNIEnv *env, jobject instance,
                                                  jlong nativeProgram) {

    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    glProgram->use();

}

void Java_com_example_david_a0x00_OpenGLProgram_validate__J(JNIEnv *env, jobject instance,
                                                       jlong nativeProgram) {

    GLDemo::GLProgram *glProgram = reinterpret_cast<GLDemo::GLProgram *>(nativeProgram);
    glProgram->validate();
}

// ------------- Render
jlong Java_com_example_david_a0x00_OpenGLRender_createRender(JNIEnv *env, jobject instance) {
    GLDemo::GLTriangleRender *glRender = new GLDemo::GLTriangleRender();
    return reinterpret_cast<long>(glRender);
}

jlong Java_com_example_david_a0x00_OpenGLRender_createTextureRender(JNIEnv *env, jobject instance, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    GLDemo::GLSimpleRender *glRender = new GLDemo::GLSimpleRender(path);
    env->ReleaseStringUTFChars(path_, path);
    return reinterpret_cast<long>(glRender);
}

jlong Java_com_example_david_a0x00_OpenGLRender_createRenderWithTwoTexture(JNIEnv *env, jobject instance,
        jstring first_,
jstring secondPath_) {
    const char *first = env->GetStringUTFChars(first_, 0);
    const char *secondPath = env->GetStringUTFChars(secondPath_, 0);
    GLDemo::GLTextureBlendingRender *glRender = new GLDemo::GLTextureBlendingRender(first, secondPath);

    env->ReleaseStringUTFChars(first_, first);
    env->ReleaseStringUTFChars(secondPath_, secondPath);
    return reinterpret_cast<long>(glRender);
}

void Java_com_example_david_a0x00_OpenGLRender_render__JII(JNIEnv *env, jobject instance,
                                                      jlong nativeRender, jint width, jint height) {

    GLDemo::GLTriangleRender *glRender = reinterpret_cast<GLDemo::GLTriangleRender *>(nativeRender);
    glRender->render({(float)(width), (float)(height)});
}

void Java_com_example_david_a0x00_OpenGLRender_setPercent__JF(JNIEnv *env, jobject instance,
                                                              jlong nativeRender, jfloat percent) {

    // TODO
    GLDemo::GLSimpleRender *glRender = reinterpret_cast<GLDemo::GLSimpleRender *>(nativeRender);
    glRender->setEffectPercent(percent);
}

#ifdef __cplusplus
}
#endif
