//
//  OpenGLRender.h
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/2.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "OpenGLProgram.h"

#define STRINGIZE(x) #x
#define STRINGIZE2(x) STRINGIZE(x)
#define SHADER_STRING(name) @STRINGIZE(name)

// 默认脚本
extern NSString *const kVertexShaderString;
extern NSString *const kFragmentShaderString;

@interface OpenGLRender : NSObject
@property (nonatomic, strong) OpenGLProgram *program;

- (instancetype)initWithVertex:(NSString *)vertex fragment:(NSString *)fragment;
- (void)render:(CGSize)size;
- (GLfloat *)textureVertexForViewSize:(CGSize)viewSize textureSize:(CGSize)textureSize;
@end
