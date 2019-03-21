//
//  ViewController.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/9/10.
//  Copyright © 2018年 David.Dai. All rights reserved.
//

#import "ViewController.h"
#import "OpenGLView.h"
#import "OpenGLKView.h"

@interface ViewController ()
@property (nonatomic, strong) OpenGLView *glView;
@property (nonatomic, strong) OpenGLKView *glkView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    self.glkView = [[OpenGLKView alloc] initWithFrame:CGRectMake(0,  self.view.frame.size.height / 2.0 , self.view.frame.size.width, self.view.frame.size.height / 2.0)
                                              context:context];
    [self.view addSubview:self.glkView];
    
    self.glView = [[OpenGLView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height / 2.0)];
    [self.view addSubview:self.glView];
}

@end
