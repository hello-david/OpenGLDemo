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
    self.glView = [[OpenGLView alloc] initWithFrame:CGRectMake(50, 50, 100, 100)];
    [self.view addSubview:self.glView];
    
    self.glkView = [[OpenGLKView alloc] initWithFrame:CGRectMake(50, 170, 30, 30)];
    [self.view addSubview:self.glkView];
}

@end
