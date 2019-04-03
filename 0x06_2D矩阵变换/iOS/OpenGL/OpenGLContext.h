//
//  OpenGLContext.h
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/2.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import <Foundation/Foundation.h>
@import OpenGLES;

@interface OpenGLContext : NSObject
@property (nonatomic, strong, readonly) EAGLContext *eaglContext;

- (void)swapToScreen;
- (void)useAsCurrent;
- (BOOL)isCurrentContext;

@end
