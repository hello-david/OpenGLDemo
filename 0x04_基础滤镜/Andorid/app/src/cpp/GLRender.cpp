//
// Created by David.Dai on 2018/10/8.
//

#include "GLRender.hpp"

using namespace GLDemo;

const char *kVertexShader = R"(
 attribute vec2 position;
 varying vec2 textureCoordinate;

 void main() {
     gl_Position = vec4(position.xy, 0, 1);
 }
)";

const char *kFragmentShader = R"(
 precision mediump float;
 varying vec2 textureCoordinate;
 uniform sampler2D inputTexture;

 void main() {
     gl_FragColor = vec4(0,0,0,1);
 }
)";


GLRender::GLRender() : GLRender(kVertexShader, kFragmentShader) {

}

GLRender::GLRender(const std::string &vertexStr, const std::string &fragmentStr) {
    mProgram = new GLProgram(vertexStr, fragmentStr);
    if (!mProgram->link()) {
        std::string progLog = mProgram->programLog();
        printf("Program link log: %s", progLog.c_str());
        std::string fragLog = mProgram->fragmentShaderLog();
        printf("Fragment shader compile log: %s", fragLog.c_str());
        std::string vertLog = mProgram->vertexShaderLog();
        printf("Vertex shader compile log: %s", vertLog.c_str());
        mProgram = NULL;
        assert(false);
    }

    // 给Program添加属性(shader脚本中定义好的变量)，默认脚本中使用三个变量
    mPositionAttribute = (GLuint)mProgram->attributeIndex("position");
    mInputTextureCoorAttribute = (GLuint)mProgram->attributeIndex("inputTextureCoordinate");
    mTextureUniform = (GLuint)mProgram->uniformIndex("inputTexture");
}

GLRender::~GLRender() {
    delete mProgram;
}

void GLRender::render(const _Size &size) {
    // 当前上下文使用该渲染管线
    mProgram->use();

    // 定义裁剪空间转换到屏幕上的空间大小
    glViewport(0, 0, size.width, size.height);

    glClearColor(0,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    // 输入顶点坐标
    // 安卓中申请的每个attr必须要填入值，不然效果会失效
    glEnableVertexAttribArray(mPositionAttribute);

    const float vertex[6] = {
            -0.5, -0.5,
            -0.5, 0.5,
            0.5, 0.5 };

    // 将顶点坐标和program中position这个变量联系到一起
    glVertexAttribPointer(mPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, vertex);

    // 使用三角形图元绘制
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

    glDisableVertexAttribArray(mPositionAttribute);

}