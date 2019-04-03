//
// Created by David.Dai on 2018/11/23.
//

#include "GLTextureBlendingRender.hpp"

using namespace GLDemo;

GLTextureBlendingRender::GLTextureBlendingRender(const std::string &backImagePath, const std::string &frontImagePath) :
        GLRender(kVertexForTextureShader, kFragmentForTextureShader) {
    mBackTexture = new GLTexture(backImagePath);
    mFrontTexture = new GLTexture(frontImagePath);
}

GLTextureBlendingRender::~GLTextureBlendingRender() {
    if (mBackTexture) {
        delete mBackTexture;
        mBackTexture = nullptr;
    }

    if (mFrontTexture) {
        delete mFrontTexture;
        mFrontTexture = nullptr;
    }
}

void GLTextureBlendingRender::render(const _Size &size) {
    mProgram->use();
    glViewport(0, 0, size.width, size.height);

    const float inputTextureCoor[8] = {
            0,     0,
            1.0,   0,
            0,   1.0,
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

    glEnable(GL_BLEND);
//    glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    // 渲染背景图片
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mBackTexture->texture());
    glUniform1i(mTextureUniform, 0);
    glVertexAttribPointer(mPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, textureVertexForViewSize(size, mBackTexture->size()));
    glVertexAttribPointer(mInputTextureCoorAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateInputTextureCoor);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    const float frontPosition[8] = {
            0.1, -0.92,
            0.7, -0.92,
            0.1, -0.79,
            0.7, -0.79
    };

    // 渲染前景图片
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mFrontTexture->texture());
    glUniform1i(mTextureUniform, 1);
    glVertexAttribPointer(mPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, frontPosition);
    glVertexAttribPointer(mInputTextureCoorAttribute, 2, GL_FLOAT, GL_FALSE, 0, rotateInputTextureCoor);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mPositionAttribute);
    glDisableVertexAttribArray(mInputTextureCoorAttribute);

}
