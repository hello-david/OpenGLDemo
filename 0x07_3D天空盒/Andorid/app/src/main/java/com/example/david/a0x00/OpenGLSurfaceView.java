package com.example.david.a0x00;

import android.content.Context;
import android.opengl.GLES20;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Size;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

// 继承于SurfaceView并实现SurfaceHolder
public class OpenGLSurfaceView extends SurfaceView implements SurfaceHolder.Callback {

    private final int kOnSurfaceViewRender = -9999;
    private final int kOnSurfaceCreated    = -9998;
    private final int kOnSurfaceDestroyed  = -9997;
    private final int kOnSurfaceLayout     = -9996;

    private OpenGLContext mGLContext = null;
    private HandlerThread mGLViewHandlerThread = null;
    private android.os.Handler mRenderHandler = null;
    public OpenGLRender renderler = null;

    // Android中的代理对象
    public GLViewListener listener = null;

    public interface GLViewListener {
        void onViewDidCreateGlResource(OpenGLSurfaceView view);
    }

    // 构造器
    public OpenGLSurfaceView(Context context) {
        super(context);
        commonInit();
    }

    public OpenGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        commonInit();
    }

    public OpenGLSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        commonInit();
    }

    public OpenGLSurfaceView(Context context, AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        commonInit();
    }

    public OpenGLContext glContext(){
        return mGLContext;
    }

    // 控制渲染流程
    private void commonInit() {
        // SurfaceView 回调
        getHolder().addCallback(this);

        mGLViewHandlerThread = new HandlerThread("com.OpenGLView");
        mGLViewHandlerThread.start();

        mRenderHandler = new Handler(mGLViewHandlerThread.getLooper(), new Handler.Callback() {
            @Override
            public boolean handleMessage(Message msg) {
                switch (msg.what) {
                    case kOnSurfaceViewRender:
                        if (mGLContext != null) {
                            // 切换GL到当前上下文
                            mGLContext.useAsCurrentContex();
                            GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);

                            // GL绘制
                            GLES20.glClearColor(1,1,1,1);
                            GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);

                            if (renderler != null) {
                                renderler.render(new Size(OpenGLSurfaceView.this.getWidth(), OpenGLSurfaceView.this.getHeight()));
                            }

                            // 交换Renderbuffer前后帧
                            mGLContext.swapToScreen();
                        }

                        // 刷新画面
                        Message reMsg = new Message();
                        reMsg.what = kOnSurfaceViewRender;
                        mRenderHandler.sendMessageDelayed(reMsg,16);
                        break;

                    case kOnSurfaceCreated:
                        // 创建一个GLContex
                        mGLContext = new OpenGLContext(2, OpenGLSurfaceView.this.getHolder().getSurface());

                        if (listener != null) {
                            mGLContext.useAsCurrentContex();
                            listener.onViewDidCreateGlResource(OpenGLSurfaceView.this);
                        }
                        break;

                    case kOnSurfaceLayout:

                        break;

                    case kOnSurfaceDestroyed:
                        mGLContext.dispose();
                        mGLContext = null;
                        break;
                    default:
                        break;
                }

                return false;
            }
        });

        // 刷新画面
        Message reMsg = new Message();
        reMsg.what = kOnSurfaceViewRender;
        mRenderHandler.sendMessageDelayed(reMsg,16);
    }

    // SurfaceView Life Cycle
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        Message msg = Message.obtain();
        msg.what = kOnSurfaceCreated;
        mRenderHandler.sendMessage(msg);
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Message msg = Message.obtain();
        msg.what = kOnSurfaceLayout;
        mRenderHandler.sendMessage(msg);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Message msg = Message.obtain();
        msg.what = kOnSurfaceDestroyed;
        mRenderHandler.sendMessage(msg);
    }
}
