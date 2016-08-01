package com.jqbar;
 
import com.jqbar.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.KeyEvent;
import android.view.Window;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
 
public class BwWebView extends Activity 
{
    /** Called when the activity is first created. */
    private WebView mWebView;
    private ProgressDialog mProgressDlg;
    private final static int BW_MSG_SHOW = 0;
    private final static int BW_MSG_HIDE = 1;
    private  Handler mHandler;
    
    public void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        GlobalFun.setOrientation( this );
        setContentView(R.layout.bwwebview);
        init();//执行初始化函数
       
        mHandler = new Handler()
        {
            public void handleMessage(Message msg)
            {
                // 定义一个Handler，用于处理下载线程与UI间通讯
                if (!Thread.currentThread().isInterrupted())
                {
                    switch (msg.what)
                    {
                    case BW_MSG_SHOW:
                        mProgressDlg.show();// 显示进度对话框
                        break;
                    case BW_MSG_HIDE:
                        mProgressDlg.hide();// 隐藏进度对话框，不可使用dismiss()、cancel(),否则再次调用show()时，显示的对话框小圆圈不会动。
                        break;
                    }
                }
                super.handleMessage(msg);
            }
        };
        Intent intent = this.getIntent();
        Bundle bundle = intent.getExtras();
        String strUrl = bundle.getString("BW_URL");
        bwLoadurl(mWebView, strUrl );
    }
    public void init()
    {
        //初始化
        mWebView=(WebView)findViewById(R.id.bwwebview);
        mWebView.getSettings().setJavaScriptEnabled(true);//可用JS
        mWebView.setScrollBarStyle(0);//滚动条风格，为0就是不给滚动条留空间，滚动条覆盖在网页上
        mWebView.getSettings().setDefaultZoom( WebSettings.ZoomDensity.FAR );
        mWebView.setInitialScale( 100 );
        //mWebView.getSettings().setBuiltInZoomControls(true); 
        mWebView.getSettings().setBuiltInZoomControls(false);
        //mWebView.getSettings().setUseWideViewPort(true); 
        mWebView.getSettings().setSupportZoom(false);
        mWebView.setWebViewClient(new WebViewClient()
        {   
            public boolean shouldOverrideUrlLoading(final WebView view, final String url)
            {
                bwLoadurl(view,url);//载入网页
                return true;   
            }//重写点击动作,用webview载入
 
        });
        mWebView.setWebChromeClient(new WebChromeClient()
        {
            public void onProgressChanged(WebView view,int progress)
            {//载入进度改变而触发 
                if(progress==100)
                {
                    mHandler.sendEmptyMessage( BW_MSG_HIDE );//如果全部载入,隐藏进度对话框
                }   
                super.onProgressChanged(view, progress);   
            }   
        });
 
        mProgressDlg=new ProgressDialog(BwWebView.this);
        mProgressDlg.setProgressStyle(ProgressDialog.STYLE_SPINNER);
        mProgressDlg.setMessage("数据载入中，请稍候！");
    }
    
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
    		if(keyCode == KeyEvent.KEYCODE_BACK)
        {
            ConfirmExit();//按了返回键，但已经不能返回，则执行退出确认
            return true; 
        }   
        //捕捉返回键
        if ((keyCode == KeyEvent.KEYCODE_BACK) && mWebView.canGoBack())
        {   
            mWebView.goBack();   
            return true;   
        }
        else if(keyCode == KeyEvent.KEYCODE_BACK)
        {
            ConfirmExit();//按了返回键，但已经不能返回，则执行退出确认
            return true; 
        }   
        return super.onKeyDown(keyCode, event);   
    }
    public void ConfirmExit()
    {
        //退出确认
        AlertDialog.Builder dlgBuild=new AlertDialog.Builder(BwWebView.this);
        dlgBuild.setTitle("退出");
        dlgBuild.setMessage("是否退出?");
        dlgBuild.setPositiveButton("是", new DialogInterface.OnClickListener()
        {//退出按钮
            public void onClick(DialogInterface dialog, int i) 
            {
                // TODO Auto-generated method stub
                BwWebView.this.finish();//关闭activity
            }
        });
        dlgBuild.setNegativeButton("否",new DialogInterface.OnClickListener()
        {
            public void onClick(DialogInterface dialog, int i) 
            {
                //不退出不用执行任何操作
            }
        });
        dlgBuild.show();//显示对话框
    }
    public void bwLoadurl(final WebView view,final String url)
    {
        new Thread()
        {
            public void run()
            {
                mHandler.sendEmptyMessage( BW_MSG_SHOW);
                view.loadUrl(url);//载入网页
            }
        }.start();
    }
    
}