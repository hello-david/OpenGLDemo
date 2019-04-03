//
// Created by David.Dai on 2018/10/8.
//
#ifndef GLPROGRAM_HPP
#define GLPROGRAM_HPP

#include <stdio.h>
#include <string>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>

namespace GLDemo {
    class GLProgram {

    private:
        bool mInitialized = false;
        GLuint mProgram, mVertShader, mFragShader;
        std::string mProgramLog, mVertexShaderLog,mFragmentShaderLog;

        bool compileShader(const std::string shaderStr, GLenum shaderType, GLuint *shader);

    public:
        GLProgram(const std::string &vertexShaderStr, const std::string &fragmentShaderStr);
        ~GLProgram();

        const std::string &vertexShaderLog(){return mVertexShaderLog;};
        const std::string &fragmentShaderLog(){return mFragmentShaderLog;};
        const std::string &programLog(){return mProgramLog;};

        GLint attributeIndex(const std::string &attributeName);
        GLint uniformIndex(const std::string &uniformName);

        bool link();
        void use() const;
        void validate();

    };
}

#endif
