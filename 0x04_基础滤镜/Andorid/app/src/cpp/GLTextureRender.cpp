//
// Created by David.Dai on 2018/10/30.
//

#include "GLTextureRender.hpp"

using namespace GLDemo;
const char *kTextureVertexShader = R"(
 attribute vec2 position;
 attribute vec2 inputTextureCoordinate;
 varying vec2 textureCoordinate;

 void main() {
     gl_Position = vec4(position.xy, 0, 1);
     textureCoordinate = inputTextureCoordinate;
 }
)";

const char *kTextureFragmentShader = R"(
 varying highp vec2 textureCoordinate;
 uniform sampler2D inputTexture;

 void main() {
     gl_FragColor = texture2D(inputTexture, textureCoordinate);
 }
)";

GLTextureRender::GLTextureRender(const std::string &imagePath) : GLRender(kTextureVertexShader, kTextureFragmentShader) {
    mTexture = new GLTexture(imagePath);
}

GLTextureRender::~GLTextureRender() {
    if (mTexture) {
        delete mTexture;
    }
}

void GLTextureRender::render(const _Size &size) {
    mProgram->use();
    glViewport(0, 0, size.width, size.height);

    const float inputTextureCoor[8] = {
            0,   0,
            1.0,   0,
            0, 1.0,
            1.0, 1.0
    };

//    const float inputTextureCoor[8] = {
//        0,     0,
//        2.0,   0,
//        0,   2.0,
//        2.0, 2.0
//    };

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

    // 传递数组数值到着色器中
    glVertexAttribPointer(mPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, textureVertexForViewSize(size, mTexture->size()));
    glVertexAttribPointer(mInputTextureCoorAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateInputTextureCoor);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mPositionAttribute);
    glDisableVertexAttribArray(mInputTextureCoorAttribute);
}

/**
 *  调整纹理顶点坐标让它和ViewPort比例一致
 */
GLfloat *GLTextureRender::textureVertexForViewSize(_Size viewSize, _Size textureSize) {
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