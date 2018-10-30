//
//  OpenGLTextureRender.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/29.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import "OpenGLTextureRender.h"
#import "OpenGLRender_Private.h"
#import "OpenGLTexture.h"

@interface OpenGLTextureRender()
@property (nonatomic, strong) OpenGLTexture *texture;
@end

@implementation OpenGLTextureRender

- (instancetype)init {
    if (self = [super init]) {
        self.texture = [[OpenGLTexture alloc] init];
    }
    return self;
}

- (void)render:(CGSize)size {
    [self.program use];
    glViewport(0, 0, size.width, size.height);
    
    const float inputTextureCoor[8] = {
          0,   0,
        1.0,   0,
          0, 1.0,
        1.0, 1.0
    };
    
    float rotateInputTextureCoor[8];
    rotateInputTextureCoor[0] = inputTextureCoor[4];
    rotateInputTextureCoor[1] = inputTextureCoor[5];
    rotateInputTextureCoor[2] = inputTextureCoor[6];
    rotateInputTextureCoor[3] = inputTextureCoor[7];
    rotateInputTextureCoor[4] = inputTextureCoor[0];
    rotateInputTextureCoor[5] = inputTextureCoor[1];
    rotateInputTextureCoor[6] = inputTextureCoor[2];
    rotateInputTextureCoor[7] = inputTextureCoor[3];
    
    glEnableVertexAttribArray(self.positionAttribute);
    glEnableVertexAttribArray(self.inputTextureCoorAttribute);
    
    // 将纹理绑定到处理单元上并将纹理句柄传到着色器中
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self.texture.texture);
    glUniform1i(self.textureUniform, 0);
    
    // 传递数组数值到着色器中
    glVertexAttribPointer(self.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, [self textureVertexForViewSize:size textureSize:self.texture.size]);
    glVertexAttribPointer(self.inputTextureCoorAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateInputTextureCoor);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisableVertexAttribArray(self.positionAttribute);
    glDisableVertexAttribArray(self.inputTextureCoorAttribute);
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
