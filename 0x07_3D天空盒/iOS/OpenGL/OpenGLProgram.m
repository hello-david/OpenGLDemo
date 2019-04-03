//
//  OpenGLProgram.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/2.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import "OpenGLProgram.h"

@interface OpenGLProgram()
@property (nonatomic, strong) NSString *vertShaderStr;
@property (nonatomic, strong) NSString *fragShaderStr;

@property (nonatomic, strong) NSString *vertexShaderLog;
@property (nonatomic, strong) NSString *fragmentShaderLog;
@property (nonatomic, strong) NSString *programLog;
@property (nonatomic, assign) BOOL initialized;

@property (nonatomic, assign) GLuint program;
@property (nonatomic, assign) GLuint vertShader;
@property (nonatomic, assign) GLuint fragShader;

@end

@implementation OpenGLProgram

- (instancetype)initWithVertexShader:(NSString *)vertexShader
                      fragmentShader:(NSString *)fragmentShader {
    if (self = [super init]) {
        self.vertShaderStr = vertexShader;
        self.fragShaderStr = fragmentShader;
        [self commonInit];
    }
    return self;
}

- (void)commonInit {
    // 创建GL Program
    self.program = glCreateProgram();
    
    // 编译脚本
    if (![self compileShader:&_vertShader type:GL_VERTEX_SHADER shaderString:_vertShaderStr]) {
        NSLog(@"Failed to compile vertex shader");
    }
    
    if (![self compileShader:&_fragShader type:GL_FRAGMENT_SHADER shaderString:_fragShaderStr]) {
        NSLog(@"Failed to compile fragment shader");
    }
    
    // 装载Shader到GL Program
    glAttachShader(_program, _vertShader);
    glAttachShader(_program, _fragShader);
}

- (void)dealloc {
    if (self.vertShader)
        glDeleteShader(self.vertShader);
    
    if (self.fragShader)
        glDeleteShader(self.fragShader);
    
    if (self.program)
        glDeleteProgram(self.program);
}

- (GLint)attributeIndex:(NSString *)attributeName {
    return glGetAttribLocation(self.program, [attributeName cStringUsingEncoding:NSUTF8StringEncoding]);
}

- (GLint)uniformIndex:(NSString *)uniformName {
    return glGetUniformLocation(self.program, [uniformName cStringUsingEncoding:NSUTF8StringEncoding]);
}

- (BOOL)linkShader {
    GLint status;
    
    // 链接脚本到Program
    glLinkProgram(self.program);
    glGetProgramiv(self.program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        return NO;
    
    // 在链接后编译的脚本便可以删除
    if (self.vertShader) {
        glDeleteShader(self.vertShader);
        self.vertShader = 0;
    }
    
    if (self.fragShader) {
        glDeleteShader(self.fragShader);
        self.fragShader = 0;
    }
    
    self.initialized = YES;
    
    return YES;
}

- (void)use {
    glUseProgram(self.program);
}

- (void)validate {
    GLint logLength;
    
    // 失效Program
    glValidateProgram(self.program);
    glGetProgramiv(self.program, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar log[logLength];
        glGetProgramInfoLog(self.program, logLength, &logLength, log);
        char logBuffer[logLength];
        sprintf(logBuffer,"%s", log);
        self.programLog = [NSString stringWithCString:logBuffer encoding:NSUTF8StringEncoding];
    }
    
    self.initialized = NO;
}

#pragma mark - Internal
- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type shaderString:(NSString *)shaderString {
    GLint status;
    const GLchar *source = [shaderString cStringUsingEncoding:NSUTF8StringEncoding];
    if (!source) {
        NSLog(@"Failed to load shader");
        return NO;
    }
    
    // 创建及编译Shader
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    
    // 输出错误日志
    if (status != GL_TRUE) {
        GLint logLength = 0;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar log[logLength];
            glGetShaderInfoLog(*shader, logLength, &logLength, log);
            char logBuffer[logLength];
            sprintf(logBuffer,"%s", log);
            if (shader == &_vertShader) {
                self.vertexShaderLog = [NSString stringWithCString:logBuffer encoding:NSUTF8StringEncoding];
            }
            else {
                self.fragmentShaderLog = [NSString stringWithCString:logBuffer encoding:NSUTF8StringEncoding];
            }
        }
    }
    
    return status == GL_TRUE ? YES : NO;
}
@end
