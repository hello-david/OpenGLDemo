//
// Created by David.Dai on 2018/10/8.
//

#include "GLRender.hpp"

using namespace GLDemo;

const char *kVertexShader = R"(
 attribute vec2 position;

 void main() {
     gl_Position = vec4(position.xy, 0, 1);
 }
)";

const char *kVertexForTextureShader = R"(
 attribute vec2 position;
 attribute vec2 inputTextureCoordinate;
 varying vec2 textureCoordinate;

 void main() {
     gl_Position = vec4(position.xy, 0, 1);
     textureCoordinate = inputTextureCoordinate;
 }
)";

const char *kFragmentShader = R"(
 precision mediump float;

 void main() {
     gl_FragColor = vec4(0,0,0,1);
 }
)";

const char *kFragmentForTextureShader = R"(
 varying highp vec2 textureCoordinate;
 uniform sampler2D inputTexture;

 void main() {
     gl_FragColor = texture2D(inputTexture, textureCoordinate);
 }
)";


GLRender::GLRender() : GLRender(kVertexShader, kFragmentShader) {
    // 给Program添加属性(shader脚本中定义好的变量)，默认脚本中使用三个变量
    mPositionAttribute = (GLuint)mProgram->attributeIndex("position");
    mInputTextureCoorAttribute = (GLuint)mProgram->attributeIndex("inputTextureCoordinate");
    mTextureUniform = (GLuint)mProgram->uniformIndex("inputTexture");
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


/**
 *  调整纹理顶点坐标让它和ViewPort比例一致
 */
GLfloat *GLRender::textureVertexForViewSize(_Size viewSize, _Size textureSize) {
    static GLfloat position[8];

    GLfloat viewAspectRatio = viewSize.width / viewSize.height;
    GLfloat textureAspectRatio = textureSize.width / textureSize.height;

    GLfloat widthScaling = 1;
    GLfloat heightScaling = 1;
    if (viewAspectRatio < textureAspectRatio) {
        GLfloat height = (viewSize.width / textureSize.width) * textureSize.height;
        heightScaling = height / viewSize.height;
    }else{
        GLfloat width = (viewSize.height / textureSize.height) * textureSize.width;
        widthScaling = width / viewSize.width;
    }

    position[0] = -widthScaling;
    position[1] = -heightScaling;
    position[2] = widthScaling;
    position[3] = -heightScaling;
    position[4] = -widthScaling;
    position[5] = heightScaling;
    position[6] = widthScaling;
    position[7] = heightScaling;

    return position;
}
