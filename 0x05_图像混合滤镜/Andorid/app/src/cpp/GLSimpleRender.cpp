//
// Created by David.Dai on 2018/11/14.
//

#include "GLSimpleRender.hpp"
using namespace GLDemo;

#pragma mark - 对比度
const char *kContrastFragmentShader = R"(
 varying highp vec2 textureCoordinate;
 uniform sampler2D inputTexture;
 uniform lowp float contrast;

 void main() {
    lowp vec4 textureColor = texture2D(inputTexture, textureCoordinate);
    gl_FragColor = vec4(((textureColor.rgb - vec3(0.5)) * contrast + vec3(0.5)), textureColor.w);
 }
)";

#pragma mark - 亮度
const char *kLuminanceFragmentShader = R"(
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
)";

#pragma mark - 饱和度
const char *kSaturationFragmentShader = R"(
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
)";

#pragma mark - 锐化
const char *kSharpenVertexShader = R"(
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
)";

const char *kSharpenFragmentShader = R"(
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
)";

#pragma mark - 影晕
const char *kVignetteFragmentShader = R"(
 uniform sampler2D inputTexture;
 varying highp vec2 textureCoordinate;

 uniform lowp vec2 vignetteCenter;
 uniform lowp vec3 vignetteColor;
 uniform highp float vignetteStart;
 uniform highp float vignetteEnd;

 void main() {
     lowp vec4 sourceImageColor = texture2D(inputTexture, textureCoordinate);
     lowp float d = distance(textureCoordinate, vec2(vignetteCenter.x, vignetteCenter.y));
     lowp float percent = smoothstep(vignetteStart, vignetteEnd, d);
     gl_FragColor = vec4(mix(sourceImageColor.rgb, vignetteColor, percent), sourceImageColor.a);
 }
)";

#pragma mark -

GLSimpleRender::GLSimpleRender(const std::string &imagePath) : GLRender(kVertexForTextureShader, kVignetteFragmentShader) {
    mTexture = new GLTexture(imagePath);

    mContrastUniform = mProgram->uniformIndex("contrast");
    mLuminanceUniform = mProgram->uniformIndex("rangeReduction");
    mSaturationUniform = mProgram->uniformIndex("saturation");

    mSharpnessUniform = mProgram->uniformIndex("sharpness");
    mImageWidthFactorUniform = mProgram->uniformIndex("imageWidthFactor");
    mImageHeightFactorUniform = mProgram->uniformIndex("imageHeightFactor");

    mVignetteCenterUniform = mProgram->uniformIndex("vignetteCenter");
    mVignetteColorUniform = mProgram->uniformIndex("vignetteColor");
    mVignetteStartUniform = mProgram->uniformIndex("vignetteStart");
    mVignetteEndUniform = mProgram->uniformIndex("vignetteEnd");
}

GLSimpleRender::~GLSimpleRender() {
    if (mTexture) {
        delete mTexture;
    }
}

void GLSimpleRender::render(const _Size &size) {
    mProgram->use();
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

    glEnableVertexAttribArray(mPositionAttribute);
    glEnableVertexAttribArray(mInputTextureCoorAttribute);

    // 将纹理绑定到处理单元上并将纹理句柄传到着色器中
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTexture->texture());
    glUniform1i(mTextureUniform, 0);

//    glUniform1f(mContrastUniform, mPercent);
//    glUniform1f(mLuminanceUniform, mPercent);
//    glUniform1f(mSaturationUniform, mPercent);
//
//    glUniform1f(mSharpnessUniform, mPercent);
//    glUniform1f(mImageWidthFactorUniform, 1.0 / size.height);
//    glUniform1f(mImageHeightFactorUniform, 1.0 / size.width);

    glUniform1f(mVignetteStartUniform, mVignetteStart);
    glUniform1f(mVignetteEndUniform, mPercent);
    glUniform2f(mVignetteCenterUniform, mVignetteCenter.x, mVignetteCenter.y);
    glUniform3f(mVignetteColorUniform, mVignetteColor.r, mVignetteColor.g, mVignetteColor.b);

    glVertexAttribPointer(mPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, textureVertexForViewSize(size, mTexture->size()));
    glVertexAttribPointer(mInputTextureCoorAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateInputTextureCoor);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mPositionAttribute);
    glDisableVertexAttribArray(mInputTextureCoorAttribute);
}

/**
 *  调整纹理顶点坐标让它和ViewPort比例一致
 */
GLfloat *GLSimpleRender::textureVertexForViewSize(_Size viewSize, _Size textureSize) {
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