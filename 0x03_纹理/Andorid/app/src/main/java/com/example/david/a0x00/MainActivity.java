package com.example.david.a0x00;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity implements OpenGLSurfaceView.GLViewListener {
    static {
        //通过静态代码块加载so库
        System.loadLibrary("native-lib");
    }
    private static Context cxt;

    public static Context getContext(){
        return cxt;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        cxt = getApplicationContext();

        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        OpenGLSurfaceView glView = (OpenGLSurfaceView)findViewById(R.id.OpenGLSurfaceView);
        glView.listener = this;
    }

    @Override
    public void onViewDidCreateGlResource(OpenGLSurfaceView view) {
        // 需要在非主线程中处理
        view.renderler = new OpenGLRender();
    }
}
