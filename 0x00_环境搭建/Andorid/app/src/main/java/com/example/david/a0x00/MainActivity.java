package com.example.david.a0x00;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity implements OpenGLSurfaceView.GLViewListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        OpenGLSurfaceView glView = (OpenGLSurfaceView)findViewById(R.id.OpenGLSurfaceView);
        glView.listener = this;
    }

    @Override
    public void onViewDidCreateGlResource(OpenGLSurfaceView view) {

    }
}
