package com.example.david.a0x00;

public class OpenGLProgram {
    private long mNativeProgram;

    OpenGLProgram(String vertexString, String fragmentString) {
        mNativeProgram = createProgram(vertexString, fragmentString);
    }

    public void dispose() {
        if (mNativeProgram != 0) {
            dispose(mNativeProgram);
            mNativeProgram = 0;
        }
    }

    public String programLog() { return  programLog(mNativeProgram); }
    public String vertexShaderLog() { return vertexShaderLog(mNativeProgram); }
    public String fragmentShaderLog() { return fragmentShaderLog(mNativeProgram); }
    public int attributeIndex(String attrName) { return attributeIndex(mNativeProgram, attrName); }
    public int uniformIndex(String unifromName) { return uniformIndex(mNativeProgram, unifromName); }
    public void linkShader() { linkShader(mNativeProgram); }
    public void use() { use(mNativeProgram); }
    public void validate() { validate(mNativeProgram); }

    private native long createProgram(String vertexString, String fragmentString);
    private native void dispose(long nativeProgram);
    private native String programLog(long nativeProgram);
    private native String vertexShaderLog(long nativeProgram);
    private native String fragmentShaderLog(long nativeProgram);
    private native int attributeIndex(long nativeProgram, String attrName);
    private native int uniformIndex(long nativeProgram, String uniformName);
    private native boolean linkShader(long nativeProgram);
    private native void use(long nativeProgram);
    private native void validate(long nativeProgram);
}
