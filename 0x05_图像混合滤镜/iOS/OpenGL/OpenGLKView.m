//
//  OpenGLKView.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/9/12.
//  Copyright © 2018年 David.Dai. All rights reserved.
//

#import "OpenGLKView.h"

@implementation OpenGLKView

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];
    
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

@end
