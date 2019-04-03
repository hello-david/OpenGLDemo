//
// Created by David.Dai on 2018/10/26.
//

#include "GLTriangleRender.hpp"
using namespace GLDemo;

typedef struct {
    GLVector2 positionCoords;
} TriangleVertex;

static TriangleVertex vertices[3] = {
        {{-0.5f, -0.5f}},
        {{ 0.5f, -0.5f}},
        {{-0.5f,  0.5f}}
};

static GLVector2 movementVectors[3] = {
        {-0.02f,  -0.010f},
        { 0.01f,  -0.005f},
        {-0.01f,   0.010f},
};

GLTriangleRender::GLTriangleRender() : GLRender() {
    glGenBuffers(1, &mVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

GLTriangleRender::~GLTriangleRender() {
    if (mVBO) {
        glDeleteBuffers(1, &mVBO);
        mVBO = 0;
    }
}

void GLTriangleRender::render(const _Size &size) {
    // 当前上下文使用该渲染管线
    mProgram->use();

    // 定义裁剪空间转换到屏幕上的空间大小
    glViewport(0, 0, size.width, size.height);

    glClearColor(0,1,0,1);
    glClear(GL_COLOR_BUFFER_BIT);

    // 更新VBO
    updateAnimation();
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 输入顶点坐标
    // 安卓中申请的每个attr必须要填入值，不然效果会失效
    glEnableVertexAttribArray(mPositionAttribute);

    // 将顶点坐标和program中position这个变量联系到一起
    glVertexAttribPointer(mPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);

    // 使用三角形图元绘制
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

    glDisableVertexAttribArray(mPositionAttribute);
}

void GLTriangleRender::updateAnimation() {
    for(int i = 0; i < 3; i++) {
        vertices[i].positionCoords.x += movementVectors[i].x;
        if(vertices[i].positionCoords.x >= 1.0f || vertices[i].positionCoords.x <= -1.0f) {
            movementVectors[i].x = -movementVectors[i].x;
        }

        vertices[i].positionCoords.y += movementVectors[i].y;
        if(vertices[i].positionCoords.y >= 1.0f || vertices[i].positionCoords.y <= -1.0f) {
            movementVectors[i].y = -movementVectors[i].y;
        }
    }
}
