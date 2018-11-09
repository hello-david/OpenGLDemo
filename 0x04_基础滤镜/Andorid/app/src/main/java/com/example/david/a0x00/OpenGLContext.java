package com.example.david.a0x00;

import android.view.Surface;
import android.opengl.GLES20;

public class OpenGLContext {
    private long mNativeGLContext = 0;

    OpenGLContext(int glVersion, Surface surface) {
        mNativeGLContext = createNativeGLContext(glVersion, surface);
    }

    public void useAsCurrentContex() {
        use(mNativeGLContext);
    }

    public void swapToScreen() {
        swapToScreen(mNativeGLContext);
    }

    public void dispose() {
        if (mNativeGLContext != 0) {
            dispose(mNativeGLContext);
            mNativeGLContext = 0;
        }
    }

    // c++调用
    private native long createNativeGLContext(int glVersion, Surface surface);
    private native void use(long nativeGLContext);
    private native void dispose(long nativeGLContext);
    private native void swapToScreen(long nativeGLContext);
}
