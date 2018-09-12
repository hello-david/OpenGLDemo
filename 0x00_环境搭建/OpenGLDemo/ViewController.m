//
//  ViewController.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/9/10.
//  Copyright © 2018年 David.Dai. All rights reserved.
//

#import "ViewController.h"
#import "OpenGLView.h"

@interface ViewController ()
@property (nonatomic, strong) OpenGLView *glView;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.glView = [[OpenGLView alloc] initWithFrame:CGRectMake(50, 50, 300, 300)];
    [self.view addSubview:self.glView];
}

@end
