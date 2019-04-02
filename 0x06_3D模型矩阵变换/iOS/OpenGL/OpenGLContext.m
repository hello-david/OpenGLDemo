//
//  OpenGLContext.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/2.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import "OpenGLContext.h"

@interface OpenGLContext()
@property (nonatomic, strong) EAGLContext *eaglContext;
@end

@implementation OpenGLContext

- (instancetype)init {
    if (self = [super init]) {
        self.eaglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        [EAGLContext setCurrentContext:self.eaglContext];
    }
    return self;
}

- (void)swapToScreen {
    [self.eaglContext presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)useAsCurrent {
    if ([EAGLContext currentContext] != self.eaglContext) {
        [EAGLContext setCurrentContext:self.eaglContext];
    }
}

- (BOOL)isCurrentContext {
    return [EAGLContext currentContext] == self.eaglContext;
}

@end
