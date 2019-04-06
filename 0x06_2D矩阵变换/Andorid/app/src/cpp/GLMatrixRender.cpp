//
// Created by David.Dai on 2019/4/6.
//

#include "GLMatrixRender.hpp"

using namespace GLDemo;

const char *kMartixVertexShader = R"(
 attribute vec3 position;
 void main() {
     gl_Position = vec4(position.xyz, 1.0);
 }
)";

const char *kMatrixFragmentShader = R"(
 precision mediump float;
 void main() {
     gl_FragColor = vec4(1, 0, 1, 1);
 }
)";

GLMatrixRender::GLMatrixRender() : GLRender(kMartixVertexShader, kMatrixFragmentShader) {
    mPositionAttribute = (GLuint)mProgram->attributeIndex("position");
}

void GLMatrixRender::render(const _Size &size) {
    glClearColor(0, 1, 1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // 这里定义一个三角形
    GLVector3 top = GLVector3Make(0, 100, 0);
    GLVector3 leftBottom = GLVector3Make(-100, -100, 0);
    GLVector3 rightBottom = GLVector3Make(100, -100, 0);

    // 构建模型变换矩阵
    GLMatrix4 translationMatrix = GLMatrix4MakeTranslation(-200, 100, 0);
    GLMatrix4 rotateMatrix = GLMatrix4MakeRotation(45, 0, 0, 1); // 沿着z轴旋转45°
    GLMatrix4 scaleMatrix = GLMatrix4MakeScale(1.5, 1.5, 1); // x,y放大1.5倍数
    GLMatrix4 modleMatrix = GLMatrix4Multiply(translationMatrix, GLMatrix4Multiply(rotateMatrix, scaleMatrix));

    // 构建视图变换矩阵
    GLMatrix4 viewMatrix = GLMatrix4Identity; // 在2D图形中没必要去改变相机角度

    // 构建投影变换矩阵
    // 这里定义一个视锥体空间(这里用世界坐标系的尺度模型也用这个尺度，但需要和屏幕空间比例一致)
    GLMatrix4 orthoMatrix = GLMatrix4MakeOrtho(-size.width, size.width, -size.height, size.height, -10, 10);
    GLMatrix4 mvpMatrix = GLMatrix4Multiply(orthoMatrix, GLMatrix4Multiply(viewMatrix, modleMatrix));

    glViewport(0, 0, size.width, size.height);

    GLVector3 ndcTop = GLMatrix4MultiplyVector3WithTranslation(mvpMatrix, top);
    GLVector3 ndcLeftBottom = GLMatrix4MultiplyVector3WithTranslation(mvpMatrix, leftBottom);
    GLVector3 ndcRightBottom = GLMatrix4MultiplyVector3WithTranslation(mvpMatrix, rightBottom);

    float vertex[9] = { ndcTop.x, ndcTop.y, ndcTop.z, ndcLeftBottom.x, ndcLeftBottom.y, ndcLeftBottom.z, ndcRightBottom.x, ndcRightBottom.y, ndcRightBottom.z };

    // 当前上下文使用该渲染管线

    mProgram->use();
    glViewport(0, 0, size.width, size.height);

    glEnableVertexAttribArray(mPositionAttribute);
    glVertexAttribPointer(mPositionAttribute, 3, GL_FLOAT, GL_FALSE, 0, vertex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    glDisableVertexAttribArray(mPositionAttribute);
}
