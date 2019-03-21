//
//  OpenGLKView.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/9/12.
//  Copyright © 2018年 David.Dai. All rights reserved.
//

#import "OpenGLKView.h"
#import "OpenGLTextureRender.h"

@interface OpenGLKView()
@property (nonatomic, strong) OpenGLTextureRender *textureRender;
@end

@implementation OpenGLKView

- (OpenGLTextureRender *)textureRender {
    if (!_textureRender) {
        _textureRender = [[OpenGLTextureRender alloc] init];
    }
    return _textureRender;
}

- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];
    if (!self.context) {
        return;
    }
    
    [EAGLContext setCurrentContext:self.context];
    [self.textureRender render:self.frame.size setupViewPort:NO];
    [EAGLContext setCurrentContext:nil];
    
}

@end
