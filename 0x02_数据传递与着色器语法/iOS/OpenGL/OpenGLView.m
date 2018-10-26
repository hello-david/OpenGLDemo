//
//  OpenGLView.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/9/10.
//  Copyright © 2018年 David.Dai. All rights reserved.
//

#import "OpenGLView.h"
#import "OpenGLContext.h"
#import "OpenGLRender.h"
#import "OpenGLTriangleMoveRender.h"

@interface OpenGLView()
@property (nonatomic, assign) GLuint framebuffer;
@property (nonatomic, assign) GLuint renderbuffer;

@property (nonatomic, strong) OpenGLTriangleMoveRender *render;
@property (nonatomic, strong) OpenGLContext *context;
@property (nonatomic, strong) CAEAGLLayer *eaglLayer;

@property (nonatomic, assign) CGSize oldSize;
@property (nonatomic, strong) CADisplayLink *displayLink;
@end

@implementation OpenGLView

// 表示这个视图的Layer是CAEAGLLayer
+ (Class)layerClass{
    return [CAEAGLLayer class];
}

- (instancetype)init {
    if (self = [super init]) {
        [self commonInit];
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)coder {
    if (self = [super initWithCoder:coder]) {
        [self commonInit];
    }
    return self;
}

- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        [self commonInit];
    }
    return self;
}

- (void)commonInit {
    self.eaglLayer = (CAEAGLLayer *)self.layer;
    self.context = [[OpenGLContext alloc] init];
    self.render = [[OpenGLTriangleMoveRender alloc] init];
    
    self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(displayLinkUpdete)];
    [self.displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSDefaultRunLoopMode];
}

// Layout的时候重新适配一个Renderbuffer
- (void)layoutSubviews {
    [super layoutSubviews];
    
    CGSize size = self.frame.size;
    if (CGSizeEqualToSize(self.oldSize, CGSizeZero) || !CGSizeEqualToSize(_oldSize, size)) {
        [self linkOpenGLBuffer];
        self.oldSize = size;
    }
}

- (void)linkOpenGLBuffer {
    // 用于显示的layer
    self.eaglLayer = (CAEAGLLayer *)self.layer;
    
    // CALayer默认是透明的，而透明的层对性能负荷很大。所以将其关闭。
    self.eaglLayer.opaque = YES;
    
    // 释放旧的renderbuffer
    if (_renderbuffer) {
        glDeleteRenderbuffers(1, &_renderbuffer);
        _renderbuffer = 0;
    }
    
    // 释放旧的framebuffer
    if (_framebuffer) {
        glDeleteFramebuffers(1, &_framebuffer);
        _framebuffer = 0;
    }
    
    // 生成renderbuffer
    glGenRenderbuffers(1, &_renderbuffer);
    
    // 绑定_renderbuffer到当前OpenGL Context的GL_RENDERBUFFER中
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    
    // 绑定_renderbuffer到eaglLayer上
    [self.context.eaglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
    
    // 生成framebuffer(之后OpenGL的数据均传递到这个framebuffer中)
    glGenFramebuffers(1, &_framebuffer);
    
    // 绑定_framebuffer到当前OpenGL Context的GL_FRAMEBUFFER中
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    
    // 绑定当前OpenGL Context的GL_FRAMEBUFFER和GL_RENDERBUFFER
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _renderbuffer);
    
    // 在此完成_framebuffer<--->GL_FRAMEBUFFER<--->GL_RENDERBUFFER<--->_renderbuffer的绑定
    
    // 检查framebuffer是否创建成功
    NSError *error;
    NSAssert1([self checkFramebuffer:&error], @"%@",error.userInfo[@"ErrorMessage"]);
}

- (BOOL)checkFramebuffer:(NSError *__autoreleasing *)error {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    NSString *errorMessage = nil;
    BOOL result = NO;
    
    switch (status) {
        case GL_FRAMEBUFFER_UNSUPPORTED:
            errorMessage = @"framebuffer不支持该格式";
            result = NO;
            break;
        case GL_FRAMEBUFFER_COMPLETE:
            NSLog(@"framebuffer 创建成功");
            result = YES;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            errorMessage = @"Framebuffer不完整 缺失组件";
            result = NO;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            errorMessage = @"Framebuffer 不完整, 附加图片必须要指定大小";
            result = NO;
            break;
        default:
            // 一般是超出GL纹理的最大限制
            errorMessage = @"未知错误 error !!!!";
            result = NO;
            break;
    }
    
    NSLog(@"%@",errorMessage ? errorMessage : @"");
    *error = errorMessage ? [NSError errorWithDomain:@"error" code:status userInfo:@{@"ErrorMessage" : errorMessage}] : nil;
    
    return result;
}

- (void)displayLinkUpdete {
    [self drawSampleToFramebuffer];
}

- (void)drawSampleToFramebuffer {
    // 保证是在当前Context中
    [self.context useAsCurrent];
    
    // 保证GL_RENDERBUFFER和GL_FRAMEBUFFER绑定的对象是正确的
    glBindRenderbuffer(GL_RENDERBUFFER, _renderbuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
    
    // 给framebuffer画上背景色（最简单的fbo操作)
    glClearColor(1, 1, 1, 1);// 白色
    glClear(GL_COLOR_BUFFER_BIT);
    
    [self.render render:self.frame.size];
    
    // 告知eaglContext可以renderbuffer
    [self.context swapToScreen];
}
@end
