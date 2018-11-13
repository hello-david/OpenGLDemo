//
//  OpenGLSimpleRender.m
//  OpenGLDemo
//
//  Created by David.Dai on 2018/11/5.
//  Copyright © 2018 David.Dai. All rights reserved.
//

#import "OpenGLSimpleRender.h"
#import "OpenGLTexture.h"
#import "OpenGLRender_Private.h"

#pragma mark - 对比度
NSString *const kContrastFragmentShaderString = SHADER_STRING
(
 varying highp vec2 textureCoordinate;
 uniform sampler2D inputTexture;
 uniform lowp float contrast;
 
 void main() {
     lowp vec4 textureColor = texture2D(inputTexture, textureCoordinate);
     gl_FragColor = vec4(((textureColor.rgb - vec3(0.5)) * contrast + vec3(0.5)), textureColor.w);
 }
 );

#pragma mark - 亮度
NSString *const kLuminanceFragmentShaderString = SHADER_STRING
(
 varying highp vec2 textureCoordinate;
 uniform sampler2D inputTexture;
 uniform lowp float rangeReduction;
 
 // Values from "Graphics Shaders: Theory and Practice" by Bailey and Cunningham
 const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);
 
 void main() {
     lowp vec4 textureColor = texture2D(inputTexture, textureCoordinate);
     mediump float luminance = dot(textureColor.rgb, luminanceWeighting);
     mediump float luminanceRatio = ((0.5 - luminance) * rangeReduction);
     
     gl_FragColor = vec4((textureColor.rgb) + (luminanceRatio), textureColor.w);
 }
 );

#pragma mark - 饱和度
NSString *const kSaturationFragmentShaderString = SHADER_STRING
(
 varying highp vec2 textureCoordinate;
 uniform sampler2D inputTexture;
 uniform lowp float saturation;
 
 // Values from "Graphics Shaders: Theory and Practice" by Bailey and Cunningham
 const mediump vec3 luminanceWeighting = vec3(0.2125, 0.7154, 0.0721);
 
 void main() {
     lowp vec4 textureColor = texture2D(inputTexture, textureCoordinate);
     lowp float luminance = dot(textureColor.rgb, luminanceWeighting);
     lowp vec3 greyScaleColor = vec3(luminance);
     
     gl_FragColor = vec4(mix(greyScaleColor, textureColor.rgb, saturation), textureColor.w);
 }
);

#pragma mark - 锐化
NSString *const kSharpenVertexShaderString = SHADER_STRING
(
 attribute vec4 position;
 attribute vec4 inputTextureCoordinate;
 
 uniform float imageWidthFactor;
 uniform float imageHeightFactor;
 uniform float sharpness;
 
 varying vec2 textureCoordinate;
 varying vec2 leftTextureCoordinate;
 varying vec2 rightTextureCoordinate;
 varying vec2 topTextureCoordinate;
 varying vec2 bottomTextureCoordinate;
 
 varying float centerMultiplier;
 varying float edgeMultiplier;
 
 void main() {
     gl_Position = position;
     
     vec2 widthStep = vec2(imageWidthFactor, 0.0);
     vec2 heightStep = vec2(0.0, imageHeightFactor);
     
     textureCoordinate = inputTextureCoordinate.xy;
     leftTextureCoordinate = inputTextureCoordinate.xy - widthStep;
     rightTextureCoordinate = inputTextureCoordinate.xy + widthStep;
     topTextureCoordinate = inputTextureCoordinate.xy + heightStep;
     bottomTextureCoordinate = inputTextureCoordinate.xy - heightStep;
     
     centerMultiplier = 1.0 + 4.0 * sharpness;
     edgeMultiplier = sharpness;
 }
 );

NSString *const kSharpenFragmentShaderString = SHADER_STRING
(
 precision highp float;
 
 varying highp vec2 textureCoordinate;
 varying highp vec2 leftTextureCoordinate;
 varying highp vec2 rightTextureCoordinate;
 varying highp vec2 topTextureCoordinate;
 varying highp vec2 bottomTextureCoordinate;
 
 varying highp float centerMultiplier;
 varying highp float edgeMultiplier;
 
 uniform sampler2D inputTexture;
 
 void main() {
     mediump vec3 textureColor = texture2D(inputTexture, textureCoordinate).rgb;
     mediump vec3 leftTextureColor = texture2D(inputTexture, leftTextureCoordinate).rgb;
     mediump vec3 rightTextureColor = texture2D(inputTexture, rightTextureCoordinate).rgb;
     mediump vec3 topTextureColor = texture2D(inputTexture, topTextureCoordinate).rgb;
     mediump vec3 bottomTextureColor = texture2D(inputTexture, bottomTextureCoordinate).rgb;
     
     gl_FragColor = vec4((textureColor * centerMultiplier - (leftTextureColor * edgeMultiplier + rightTextureColor * edgeMultiplier + topTextureColor * edgeMultiplier + bottomTextureColor * edgeMultiplier)), texture2D(inputTexture, bottomTextureCoordinate).w);
 }
 );

#pragma mark -
@interface OpenGLSimpleRender()
@property (nonatomic, strong) OpenGLTexture *texture;
@property (nonatomic, assign) float percent;

@property (nonatomic, assign) GLint contrastUniform;
@property (nonatomic, assign) GLint luminanceUniform;
@property (nonatomic, assign) GLint saturationUniform;

@property (nonatomic, assign) GLint sharpnessUniform;
@property (nonatomic, assign) GLint imageWidthFactorUniform;
@property (nonatomic, assign) GLint imageHeightFactorUniform;
@end

@implementation OpenGLSimpleRender

- (instancetype)init {
    if (self = [super init]) {
        self.program = [[OpenGLProgram alloc] initWithVertexShader:kSharpenVertexShaderString fragmentShader:kSharpenFragmentShaderString];
        self.texture = [[OpenGLTexture alloc] init];
        self.percent = 1.0;
        
        if (![self.program linkShader]) {
            NSLog(@"Program link log:%@",self.program.programLog);
            NSLog(@"Vertex shader compile log:%@",self.program.vertexShaderLog);
            NSLog(@"Fragment shader compile log:%@",self.program.fragmentShaderLog);
            assert(false);
        }
        
        // 给Program添加属性(shader脚本中定义好的变量)，默认脚本中使用三个变量
        self.positionAttribute = [self.program attributeIndex:@"position"];
        self.inputTextureCoorAttribute = [self.program attributeIndex:@"inputTextureCoordinate"];
        self.textureUniform = [self.program uniformIndex:@"inputTexture"];
        
        self.contrastUniform = [self.program uniformIndex:@"contrast"];
        self.luminanceUniform = [self.program uniformIndex:@"rangeReduction"];
        self.saturationUniform = [self.program uniformIndex:@"saturation"];
        
        self.sharpnessUniform = [self.program uniformIndex:@"sharpness"];
        self.imageWidthFactorUniform = [self.program uniformIndex:@"imageWidthFactor"];
        self.imageHeightFactorUniform = [self.program uniformIndex:@"imageHeightFactor"];
        
    }
    return self;
}

/**
 *  这里只是为了验证效果，所以取值区间简易设置
 */
- (void)setEffectPercent:(float)percent {
    self.percent = (0.5 + percent * 2);
}

- (void)render:(CGSize)size {
    [self.program use];
    glViewport(0, 0, size.width, size.height);
    
    const float inputTextureCoor[8] = {
        0,   0,
        1.0,   0,
        0, 1.0,
        1.0, 1.0
    };
    
    float rotateInputTextureCoor[8];
    rotateInputTextureCoor[0] = inputTextureCoor[4];
    rotateInputTextureCoor[1] = inputTextureCoor[5];
    rotateInputTextureCoor[2] = inputTextureCoor[6];
    rotateInputTextureCoor[3] = inputTextureCoor[7];
    rotateInputTextureCoor[4] = inputTextureCoor[0];
    rotateInputTextureCoor[5] = inputTextureCoor[1];
    rotateInputTextureCoor[6] = inputTextureCoor[2];
    rotateInputTextureCoor[7] = inputTextureCoor[3];
    
    glEnableVertexAttribArray(self.positionAttribute);
    glEnableVertexAttribArray(self.inputTextureCoorAttribute);
    
    // 将纹理绑定到处理单元上并将纹理句柄传到着色器中
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, self.texture.texture);
    glUniform1i(self.textureUniform, 0);
    
    // 传递数组数值到着色器中
    glVertexAttribPointer(self.positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, [self textureVertexForViewSize:size textureSize:self.texture.size]);
    glVertexAttribPointer(self.inputTextureCoorAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateInputTextureCoor);
    
    // 传递uniform到着色器中
//    glUniform1f(self.contrastUniform, self.percent);
//    glUniform1f(self.luminanceUniform, self.percent);
//    glUniform1f(self.saturationUniform, self.percent);
    
    glUniform1f(self.sharpnessUniform, self.percent);
    glUniform1f(self.imageWidthFactorUniform, 1.0 / size.height);
    glUniform1f(self.imageHeightFactorUniform, 1.0 / size.width);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    glDisableVertexAttribArray(self.positionAttribute);
    glDisableVertexAttribArray(self.inputTextureCoorAttribute);
}

/**
 *  调整纹理顶点坐标让它和ViewPort比例一致
 */
- (GLfloat *)textureVertexForViewSize:(CGSize)viewSize textureSize:(CGSize)textureSize {
    static GLfloat position[8];
    
    GLfloat viewAspectRatio = viewSize.width / viewSize.height;
    GLfloat textureAspectRatio = textureSize.width / textureSize.height;
    
    GLfloat widthScaling = 1;
    GLfloat heightScaling = 1;
    if (viewAspectRatio < textureAspectRatio) {
        GLfloat height = (viewSize.width / textureSize.width) * textureSize.height;
        heightScaling = height / viewSize.height;
    }else{
        GLfloat width = (viewSize.height / textureSize.height) * textureSize.width;
        widthScaling = width / viewSize.width;
    }
    
    position[0] = -widthScaling;
    position[1] = -heightScaling;
    position[2] = widthScaling;
    position[3] = -heightScaling;
    position[4] = -widthScaling;
    position[5] = heightScaling;
    position[6] = widthScaling;
    position[7] = heightScaling;
    return position;
}

@end
