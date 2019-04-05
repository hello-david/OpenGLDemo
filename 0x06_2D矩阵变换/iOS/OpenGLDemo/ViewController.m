//
//  ViewController.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/9/10.
//  Copyright © 2018年 David.Dai. All rights reserved.
//

#import "ViewController.h"
#import "OpenGLView.h"
#import <CoreMedia/CoreMedia.h>

@interface ViewController ()
@property (nonatomic, strong) OpenGLView *glView;
@property (weak, nonatomic) IBOutlet UISlider *slider;
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.glView = [[OpenGLView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    [self.view addSubview:self.glView];
    [self.view sendSubviewToBack:self.glView];
    self.slider.alpha = 0;
}

@end
