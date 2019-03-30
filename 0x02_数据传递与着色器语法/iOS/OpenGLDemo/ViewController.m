//
//  ViewController.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/9/10.
//  Copyright © 2018年 David.Dai. All rights reserved.
//

#import "ViewController.h"
#import "OpenGLView.h"
#import <GLKit/GLKit.h>

@interface ViewController () <GLKViewDelegate>
@property (nonatomic, strong) OpenGLView *glView;
@property (nonatomic, strong) GLKView *glkView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
//    self.glView = [[OpenGLView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
//    [self.view addSubview:self.glView];
    
    self.glkView = [[GLKView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height) context:[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1]];
    self.glkView.delegate = self;
    [self.view addSubview:self.glkView];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    [EAGLContext setCurrentContext:view.context];
    
    glClearColor(1, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // VA传输示例
    const GLfloat triangleVertices[] = {
        0,      0,     // top center
        -1.0, -1.0,     // bottom left
        1.0,  -1.0,     // bottom right
    };

    const GLfloat triangleColors[] = {
        1.0, 0.0, 0.0, 1.0,  // red
        0.0, 1.0, 0.0, 1.0,  // green
        0.0, 0.0, 1.0, 1.0,  // blue
    };

    glVertexPointer(2, GL_FLOAT, 0, triangleVertices);
    glEnableClientState(GL_VERTEX_ARRAY);

    glColorPointer(4, GL_FLOAT, 0, triangleColors);
    glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
@end
