//
//  OpenGLProgram.h
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/2.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES3/gl.h>

@interface OpenGLProgram : NSObject
@property (nonatomic, strong, readonly) NSString *vertexShaderLog;
@property (nonatomic, strong, readonly) NSString *fragmentShaderLog;
@property (nonatomic, strong, readonly) NSString *programLog;
@property (nonatomic, assign, readonly) BOOL initialized;

- (instancetype)initWithVertexShader:(NSString *)vertexShader
                      fragmentShader:(NSString *)fragmentShader;

- (GLint)attributeIndex:(NSString *)attributeName;
- (GLint)uniformIndex:(NSString *)uniformName;

- (BOOL)linkShader;
- (void)use;
- (void)validate;

@end

