package com.example.david.a0x00;


import android.util.Size;

public class OpenGLRender {
    private long mNativeRender;

    OpenGLRender() { mNativeRender = createRender(); };
    public void render(Size size) { render(mNativeRender, size.getWidth(), size.getHeight()); };

    private native long createRender();
    private native void render(long nativeRender, int width, int height);

}
