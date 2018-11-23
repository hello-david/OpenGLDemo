//
//  OpenGLTextureBlendingRender.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/11/23.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import "OpenGLTextureBlendingRender.h"
#import "OpenGLRender_Private.h"
#import "OpenGLTexture.h"

@interface OpenGLTextureBlendingRender()
@property (nonatomic, strong) OpenGLTexture *backTexture;
@property (nonatomic, strong) OpenGLTexture *frontTexture;
@end

@implementation OpenGLTextureBlendingRender

- (instancetype)init {
    if (self = [super initWithVertex:kVertexShaderString fragment:kFragmentShaderString]) {
        self.backTexture = [[OpenGLTexture alloc] initWithPath:[[NSBundle mainBundle] pathForResource:@"1" ofType:@"jpg"]];
        self.frontTexture = [[OpenGLTexture alloc] initWithPath:[[NSBundle mainBundle] pathForResource:@"3" ofType:@"png"]];
    }
    return self;
}

- (void)render:(CGSize)size {
    [self.program use];
    glViewport(0, 0, size.width, size.height);
    
    const float inputTextureCoor[8] = {
        0,     0,
        1.0,   0,
        0,   1.0,
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
    
    glEnable(GL_BLEND);
//    glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    
    // 渲染背景图片
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self.backTexture.texture);
    glUniform1i(self.textureUniform, 0);
    glVertexAttribPointer(self.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, [self textureVertexForViewSize:size textureSize:self.backTexture.size]);
    glVertexAttribPointer(self.inputTextureCoorAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateInputTextureCoor);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    const float frontPosition[8] = {
        0.1, -0.92,
        0.7, -0.92,
        0.1, -0.79,
        0.7, -0.79
    };
    
    // 渲染前景图片
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, self.frontTexture.texture);
    glUniform1i(self.textureUniform, 1);
    glVertexAttribPointer(self.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, frontPosition);
    glVertexAttribPointer(self.inputTextureCoorAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateInputTextureCoor);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisableVertexAttribArray(self.positionAttribute);
    glDisableVertexAttribArray(self.inputTextureCoorAttribute);
}

@end
