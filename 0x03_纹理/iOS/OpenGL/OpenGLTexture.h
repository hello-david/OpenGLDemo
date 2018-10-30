//
//  OpenGLTexture.h
//  OpenGLDemo
//
//  Created by David.Dai on 2018/10/30.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES3/gl.h>

@interface OpenGLTexture : NSObject
@property (nonatomic, assign) GLuint texture;
@property (nonatomic, assign) CGSize size;

- (void)loadTexutreFromPath:(NSString *)path;
@end

