//
// Created by David.Dai on 2018/10/8.
//

#include "GLProgram.hpp"
using namespace GLDemo;

GLProgram::GLProgram(const std::string &vertexShaderStr, const std::string &fragmentShaderStr) {
    mProgram = glCreateProgram();

    if (!compileShader(vertexShaderStr, GL_VERTEX_SHADER, &mVertShader)) {
        printf("Failed to compile vertex shader \n");
    }

    if (!compileShader(fragmentShaderStr, GL_FRAGMENT_SHADER, &mFragShader)) {
        printf("Failed to compile fragment shader \n");
    }

    glAttachShader(mProgram, mVertShader);
    glAttachShader(mProgram, mFragShader);
}

GLProgram::~GLProgram(){
    if (mVertShader)
        glDeleteShader(mVertShader);

    if (mFragShader)
        glDeleteShader(mFragShader);

    if (mProgram)
        glDeleteProgram(mProgram);
}

GLint GLProgram::attributeIndex(const std::string &attributeName) {
    return glGetAttribLocation(mProgram, attributeName.c_str());
}

GLint GLProgram::uniformIndex(const std::string &uniformName) {
    return glGetUniformLocation(mProgram, uniformName.c_str());
}

bool GLProgram::link() {
    GLint status;
    glLinkProgram(mProgram);

    glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        return false;

    if (mVertShader) {
        glDeleteShader(mVertShader);
        (mVertShader) = 0;
    }
    if ((mFragShader)) {
        glDeleteShader((mFragShader));
        (mFragShader) = 0;
    }

    mInitialized = true;
    return true;
}

void GLProgram::use() const {
    glUseProgram(mProgram);
}

void GLProgram::validate() {
    GLint logLength;
    glValidateProgram(mProgram);

    // 获取Log
    glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log =  new GLchar[logLength];
        glGetProgramInfoLog(mProgram, logLength, &logLength, log);
        char logBuffer[logLength];
        sprintf(logBuffer,"%s", log);
        mProgramLog = logBuffer;
        delete [] log;
    }
}

bool GLProgram::compileShader(const std::string shaderStr, GLenum shaderType, GLuint *shader) {
    GLint status;
    const GLchar *source;

    source = shaderStr.c_str();
    if (!source) {
        printf("Failed to load vertex shader \n");
        return false;
    }

    // 编译Shader
    *shader = glCreateShader(shaderType);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);

    // 获取Log
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)  {
        GLint logLength = 0;
        glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0) {
            GLchar *log = new GLchar[logLength];
            glGetShaderInfoLog(*shader, logLength, &logLength, log);
            char logBuffer[logLength];
            sprintf(logBuffer,"%s", log);
            if (shader == &mVertShader)  {
                mVertexShaderLog = logBuffer;
            }
            else {
                mFragmentShaderLog = logBuffer;
            }
            delete [] log;
        }
    }

    return status == GL_TRUE;
}
