package com.example.david.a0x00;
import android.content.Context;
import android.util.Size;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class OpenGLRender {
    private long mNativeRender;

    OpenGLRender() {
        mNativeRender = createTextureRender(getResource());
    };

    private String getResource() {
        return getAssetsCacheFile(MainActivity.getContext(), "texture1.jpg");
    }

    public String getAssetsCacheFile(Context context,String fileName)   {
        File cacheFile = new File(context.getCacheDir(), fileName);
        try {
            InputStream inputStream = context.getAssets().open(fileName);
            try {
                FileOutputStream outputStream = new FileOutputStream(cacheFile);
                try {
                    byte[] buf = new byte[1024];
                    int len;
                    while ((len = inputStream.read(buf)) > 0) {
                        outputStream.write(buf, 0, len);
                    }
                } finally {
                    outputStream.close();
                }
            } finally {
                inputStream.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return cacheFile.getAbsolutePath();
    }

    public void render(Size size) { render(mNativeRender, size.getWidth(), size.getHeight()); };

    private native long createRender();
    private native long createTextureRender(String path);
    private native void render(long nativeRender, int width, int height);

}
