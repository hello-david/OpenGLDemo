//
//  OpenGLMartixRender.m
//  OpenGLDemo
//
//  Created by David.Dai on 2019/4/3.
//  Copyright © 2019 David.Dai. All rights reserved.
//

#import "OpenGLMartixRender.h"
#import "OpenGLTexture.h"
#import "OpenGLRender_Private.h"
#import <GLKit/GLKit.h>

NSString *const kMartixVertexShaderString = SHADER_STRING
(
 attribute vec3 position;
 void main() {
     gl_Position = vec4(position.xyz, 1.0);
 }
 );

NSString *const kMartixFragmentShaderString = SHADER_STRING
(
 void main() {
     gl_FragColor = vec4(1, 0, 1, 1);
 }
 );

@interface OpenGLMartixRender()

@end

@implementation OpenGLMartixRender
- (instancetype)init {
    if (self = [super initWithVertex:kMartixVertexShaderString fragment:kMartixFragmentShaderString]) {
        self.positionAttribute = [self.program attributeIndex:@"position"];
    }
    return self;
}

- (void)render:(CGSize)size {
    glClearColor(0, 1, 1, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // 首先我们默认所有的坐标系原点都是(0, 0, 0)
    glLoadIdentity();
    
    // 这里定义一个三角形
    GLKVector3 top = GLKVector3Make(0, 100, 0);
    GLKVector3 leftBottom = GLKVector3Make(-100, -100, 0);
    GLKVector3 rightBottom = GLKVector3Make(100, -100, 0);
    
    // 构建模型变换矩阵
    GLKMatrix4 translationMatrix = GLKMatrix4MakeTranslation(-200, 100, 0);
    GLKMatrix4 rotateMatrix = GLKMatrix4MakeRotation(45, 0, 0, 1); // 沿着z轴旋转45°
    GLKMatrix4 scaleMatrix = GLKMatrix4MakeScale(1.5, 1.5, 1); // x,y放大1.5倍数
    GLKMatrix4 modleMatrix = GLKMatrix4Multiply(translationMatrix, GLKMatrix4Multiply(rotateMatrix, scaleMatrix));
    
    // 构建视图变换矩阵
    GLKMatrix4 viewMatrix = GLKMatrix4Identity; // 在2D图形中没必要去改变相机角度
    
    // 构建投影变换矩阵
    // 这里定义一个视锥体空间(这里用世界坐标系的尺度模型也用这个尺度，但需要和屏幕空间比例一致)
    GLKMatrix4 orthoMatrix = GLKMatrix4MakeOrtho(-size.width, size.width, -size.height, size.height, -10, 10);
    GLKMatrix4 mvpMatrix = GLKMatrix4Multiply(orthoMatrix, GLKMatrix4Multiply(viewMatrix, modleMatrix));
    
    glViewport(0, 0, size.width, size.height);
    
    GLKVector3 ndcTop = GLKMatrix4MultiplyVector3WithTranslation(mvpMatrix, top);
    GLKVector3 ndcLeftBottom = GLKMatrix4MultiplyVector3WithTranslation(mvpMatrix, leftBottom);
    GLKVector3 ndcRightBottom = GLKMatrix4MultiplyVector3WithTranslation(mvpMatrix, rightBottom);
    
    float vertex[9] = { ndcTop.x, ndcTop.y, ndcTop.z, ndcLeftBottom.x, ndcLeftBottom.y, ndcLeftBottom.z, ndcRightBottom.x, ndcRightBottom.y, ndcRightBottom.z };
    
    // 当前上下文使用该渲染管线
    [self.program use];
    glEnableVertexAttribArray(self.positionAttribute);
    glVertexAttribPointer(self.positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, vertex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    glDisableVertexAttribArray(self.positionAttribute);
}

@end
