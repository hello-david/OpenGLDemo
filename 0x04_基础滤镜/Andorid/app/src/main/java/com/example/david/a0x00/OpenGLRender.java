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

    public void setPercent(float percent) {
        setPercent(mNativeRender, percent);
    }

    /**
     * 读出一张在Assets里面的图片并生成到缓存文件中，返回缓存文件的绝对地址
     * @param context JAVA上下文环境
     * @param fileName Assets里的文件名
     * @return  缓存文件的绝对路径
     */
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
    private native void setPercent(long nativeRender, float percent);
    private native void render(long nativeRender, int width, int height);

}
