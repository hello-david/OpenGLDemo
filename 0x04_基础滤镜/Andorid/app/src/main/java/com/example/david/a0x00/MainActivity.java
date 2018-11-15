package com.example.david.a0x00;

import android.content.Context;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.SeekBar;

public class MainActivity extends AppCompatActivity implements OpenGLSurfaceView.GLViewListener {
    static {
        //通过静态代码块加载so库
        System.loadLibrary("native-lib");
    }

    private static Context cxt;

    private OpenGLSurfaceView glView;
    private SeekBar seekBar;

    public static Context getContext(){
        return cxt;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        cxt = getApplicationContext();

        setContentView(R.layout.activity_main);

        // GLView
        glView = (OpenGLSurfaceView) findViewById(R.id.OpenGLSurfaceView);
        glView.listener = this;

        // SeekBar
        seekBar = (SeekBar) findViewById(R.id.seekBar);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            /**
             * 拖动条停止拖动的时候调用
             */
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

            /**
             * 拖动条开始拖动的时候调用
             */
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            /**
             * 拖动条进度改变的时候调用
             */
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                glView.renderler.setPercent((float)(progress / 100.0));
            }
        });
    }

    @Override
    public void onViewDidCreateGlResource(OpenGLSurfaceView view) {
        // 需要在非主线程中处理
        view.renderler = new OpenGLRender();
    }
}
