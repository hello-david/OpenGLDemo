//
//  OpenGLTexture.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/30.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
@interface OpenGLTexture()
@property (nonatomic, assign) NSUInteger width;
@property (nonatomic, assign) NSUInteger height;
@end

@implementation OpenGLTexture
- (instancetype)init {
    if (self = [super init]) {
        [self loadTexutreFromPath:[[NSBundle mainBundle] pathForResource:@"1" ofType:@"jpg"]];
//        [self loadTexutreFromPath:[[NSBundle mainBundle] pathForResource:@"2" ofType:@"jpg"]];
    }
    return self;
}

- (void)loadTexutreFromPath:(NSString *)path {
    if (!path) {
        return ;
    }
    
    if (self.texture) {
        glDeleteTextures(1, &_texture);
        self.texture = 0;
    }
    
    int width = 0;
    int height = 0;
    int imageFormat = 0;
    
    // 通过stb这个库加载图片
    void *imageData = stbi_load([path cStringUsingEncoding:NSUTF8StringEncoding], &width, &height, &imageFormat, STBI_default);
    
    // 获取图片格式和像素对齐数
    int alignment = 1;
    GLuint glImageFormat = GL_RGBA;
    switch (imageFormat) {
        case STBI_grey:
            glImageFormat = GL_ALPHA;
            break;
        case STBI_grey_alpha:
            glImageFormat = GL_RG;
            break;
        case STBI_rgb:
            glImageFormat = GL_RGB;
            break;
        case STBI_rgb_alpha:
        default:
            alignment = 4;
            break;
    }
    
    // 设置像素字节对齐数，默认4字节
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    [self initTexture:imageData width:width height:height imageFormat:glImageFormat texureFormat:glImageFormat];
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
 
    free(imageData);
}

- (void)initTexture:(void *)data width:(int)width height:(int)height imageFormat:(GLuint)imageFormat texureFormat:(GLuint)texureFormat {
    // 生成Texure
    glGenTextures(1, &_texture);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, texureFormat, GL_UNSIGNED_BYTE, data);
    
    // 设置Texutre的滤波方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // 设置Texutre的环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    self.width = width;
    self.height = height;
    self.size = {(float)self.width, (float)self.height};
}

@end
