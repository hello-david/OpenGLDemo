//
//  OpenGLTriangleMoveRender.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/26.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import "OpenGLTriangleMoveRender.h"
#import "OpenGLRender_Private.h"
#import <GLKit/GLKMath.h>

typedef struct {
    GLKVector2 positionCoords;
} TriangleVertex;

// 定义三个顶点的位置
static TriangleVertex vertices[3] = {
    {{-0.5f, -0.5f}},
    {{ 0.5f, -0.5f}},
    {{-0.5f,  0.5f}}
};

// 三个顶点偏移向量
static GLKVector3 movementVectors[3] = {
    {-0.02f, -0.010f},
    { 0.01f, -0.005f},
    {-0.01f,  0.010f},
};

// 定义三个顶点的位置
static TriangleVertex vertices1[4] = {
    {{-0.5f, -1.0f}},
    {{ 0.5f, -0.5f}},
    {{-0.5f,  0.5f}},
    {{ 1.0f,  1.0f}}
};

unsigned int indices[] = { // 注意索引从0开始!
    0, 1, 3
};

@interface OpenGLTriangleMoveRender()
@property (nonatomic, assign) GLuint vbo;
@property (nonatomic, assign) GLuint ebo;
@property (nonatomic, assign) GLuint vao;
@property (nonatomic, assign) GLuint vboInVao;
@end

@implementation OpenGLTriangleMoveRender

- (instancetype)init {
    if (self = [super init]) {
        glGenBuffers(1, &_vbo);
        glGenBuffers(1, &_ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        
        // 之后生成的vbo/ebo都属于这个vao
        glGenVertexArrays(1, &_vao);
        glGenBuffers(1, &_vboInVao);
        glBindBuffer(GL_ARRAY_BUFFER, _vboInVao);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    }
    return self;
}

- (void)dealloc {
    if (_vbo != 0) {
        glDeleteBuffers(1,&_vbo);
        _vbo = 0;
    }
    
    if (_vao != 0) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
    
    if (_ebo != 0) {
        glDeleteBuffers(1, &_ebo);
        _ebo = 0;
    }
    
    if (_vboInVao != 0) {
        glDeleteBuffers(1, &_vboInVao);
        _vboInVao = 0;
    }
}

- (void)render:(CGSize)size {
    // 定义裁剪空间转换到屏幕上的空间大小
    glViewport(0, 0, size.width, size.height);
    
    // 当前上下文使用该渲染管线
    [self.program use];
    
//    // VBO数据传输示例
//    {
//        // 更新VBO的数值
//        [self updateAnimatedVertexPositions];
//        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//        /**
//         *  glVertexAttribPointer最后一个参数ptr指针的含义：
//         *  在不使用VBO的情况下：ptr就是一个指针，指向的是需要上传到顶点数据指针。通常是数组名的偏移量。
//         *  在使用VBO的情况下：首先要glBindBuffer，以后ptr指向的就不是具体的数据了。这里的ptr指向的是缓冲区数据的偏移量。
//         **/
//        glEnableVertexAttribArray(self.positionAttribute);
//        glVertexAttribPointer(self.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//
//        // 使用三角形图元绘制
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
//        glDisableVertexAttribArray(self.positionAttribute);
//    }
    
//    // VAO数据传输示例
//    {
//        glBindVertexArray(_vao);
//
//        glEnableVertexAttribArray(_vao);
//        glEnableVertexAttribArray(self.positionAttribute);
//
//        glVertexAttribPointer(self.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
//        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
//
//        glDisableVertexAttribArray(_vao);
//        glDisableVertexAttribArray(self.positionAttribute);
//    }
    
    
    // EBO数据传输示例
    {
        // 这时候GL_ARRAY_BUFFER单元中的数据是没有变的所以不需要再传一次了
        glBindBuffer(GL_ARRAY_BUFFER, _vboInVao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
        
        glEnableVertexAttribArray(self.positionAttribute);
        
        glVertexAttribPointer(self.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        
        glDisableVertexAttribArray(self.positionAttribute);
    }
}

- (void)updateAnimatedVertexPositions {
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
@end
