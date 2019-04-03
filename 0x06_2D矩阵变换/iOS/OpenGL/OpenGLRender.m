//
//  OpenGLRender.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/2.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import "OpenGLRender.h"
#import "OpenGLRender_Private.h"

NSString *const kVertexShaderString = SHADER_STRING
(
 attribute vec2 position;
 attribute vec2 inputTextureCoordinate;
 varying vec2 textureCoordinate;
 
 void main() {
     gl_Position = vec4(position.xy, 0, 1);
     textureCoordinate = inputTextureCoordinate;
 }
 );

NSString *const kFragmentShaderString = SHADER_STRING
(
 varying highp vec2 textureCoordinate;
 uniform sampler2D inputTexture;
 
 void main() {
     gl_FragColor = texture2D(inputTexture, textureCoordinate);
 }
 );

@implementation OpenGLRender

- (instancetype)init {
    return [self initWithVertex:kVertexShaderString fragment:kFragmentShaderString];
}

- (instancetype)initWithVertex:(NSString *)vertex fragment:(NSString *)fragment {
    if (self = [super init]) {
        self.program = [[OpenGLProgram alloc] initWithVertexShader:vertex fragmentShader:fragment];
        
        if (![self.program linkShader]) {
            NSLog(@"Program link log:%@",self.program.programLog);
            NSLog(@"Vertex shader compile log:%@",self.program.vertexShaderLog);
            NSLog(@"Fragment shader compile log:%@",self.program.fragmentShaderLog);
            assert(false);
        }
        
        // 给Program添加属性(shader脚本中定义好的变量)，默认脚本中使用三个变量
        self.positionAttribute = [self.program attributeIndex:@"position"];
        self.inputTextureCoorAttribute = [self.program attributeIndex:@"inputTextureCoordinate"];
        self.textureUniform = [self.program uniformIndex:@"inputTexture"];
    }
    return self;
}

// 通过默认脚本绘制一个三角形
- (void)render:(CGSize)size {
    // 当前上下文使用该渲染管线
    [self.program use];
    
    // 定义裁剪空间转换到屏幕上的空间大小
    glViewport(0, 0, size.width, size.height);
    
    // 输入顶点坐标
    glEnableVertexAttribArray(self.positionAttribute);
    const float vertex[8] = {
        -0.5, -0.5,
        -0.5, 0.5,
        0.5, 0.5 };
    
    // 将顶点坐标和programz中position这个变量联系到一起
    glVertexAttribPointer(self.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, vertex);
    
    // 使用三角形图元绘制
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
    
    glDisableVertexAttribArray(self.positionAttribute);
}

/**
 *  调整纹理顶点坐标让它和ViewPort比例一致
 */
- (GLfloat *)textureVertexForViewSize:(CGSize)viewSize textureSize:(CGSize)textureSize {
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

@end
