package com.jqbar;
 
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.net.URLDecoder;

import com.jqbar.R;
import com.unionpay.UPPayAssistEx;
import com.unionpay.uppay.PayActivity;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.app.AlertDialog.Builder;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.webkit.JsResult;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.LinearLayout;
import android.widget.ZoomButtonsController;
 
public class PageWebView extends WebView 
{
	
	private WebSettings mWebsetting;
	private String mWeburl = "";
	private MobileMain mContext;
	public PageWebView( Context context )
	{
		super( context );
		mContext = (MobileMain)context;
		mWebsetting = getSettings();
		
	}
	
	public void initView( int width, int height )
	{
		LinearLayout.LayoutParams paramWebView = new LinearLayout.LayoutParams( width, height ) ;
		setLayoutParams(paramWebView);
	}
	
    public void setViewZoom()
    {
    		mWebsetting.setDefaultZoom( WebSettings.ZoomDensity.FAR );
    		mWebsetting.setJavaScriptCanOpenWindowsAutomatically( true );
		mWebsetting.setBuiltInZoomControls(true);
		mWebsetting.setSupportZoom( true );
		mWebsetting.setUseWideViewPort( true );
		mWebsetting.setLoadWithOverviewMode(true);
		HideZoomControls();
		setInitialScale(100);
		
		
		setOnFocusChangeListener(new View.OnFocusChangeListener() 
		{  
            @Override  
            public void onFocusChange(View v, boolean hasFocus) 
            {  
                if(hasFocus)  
                {  
                		//webview.setInitialScale(100);
                    try 
                    {  
                        Field defaultScale = WebView.class.getDeclaredField("mDefaultScale");  
                        defaultScale.setAccessible(true);  
                        //WebViewSettingUtil.getInitScaleValue(VideoNavigationActivity.this, false )/100.0f 是我的程序的一个方法，可以用float 的scale替代  
                        defaultScale.setFloat(this, 1);  
                    } catch (SecurityException e) {  
                        e.printStackTrace();  
                    } catch (IllegalArgumentException e) {  
                        e.printStackTrace();  
                    } catch (IllegalAccessException e) {  
                        e.printStackTrace();  
                    } catch (NoSuchFieldException e) {  
                        e.printStackTrace();  
                        try{
                        		Field zoomManager = WebView.class.getDeclaredField("mZoomManager");
                        		zoomManager.setAccessible( true );
                        		Object zoomValue = zoomManager.get( this );
                        		Field defaultScale = zoomManager.getType().getDeclaredField("mDefaultScale");
                        		defaultScale.setAccessible( true );
                        		float sv = defaultScale.getFloat( zoomValue );
                        		defaultScale.setFloat( zoomValue, 1 );           
                        }catch( SecurityException e1 )
                        {
                        		e.printStackTrace();
                        } catch (IllegalArgumentException e1) {  
                            e.printStackTrace();  
                        } catch (IllegalAccessException e1) {  
                            e.printStackTrace();  
                        } catch (NoSuchFieldException e1) {  
                            e.printStackTrace(); 
                            try{
                            		Field mProviderField = WebView.class.getDeclaredField("mProvider");
                            		mProviderField.setAccessible( true );
                            		Object webviewclassic = mProviderField.get( this);
                            		Field zoomManager = webviewclassic.getClass().getDeclaredField("mZoomManager");
                            		zoomManager.setAccessible( true );
                            		Object zoomValue = zoomManager.get( webviewclassic );
                            		Field defaultScale = zoomManager.getType().getDeclaredField( "mDefaultScale" );
                            		defaultScale.setAccessible( true );
                            		float sv = defaultScale.getFloat( zoomValue );
                            		defaultScale.setFloat(zoomValue, 1 );
                            }
                            catch( Exception e2 )
                            {
                            	e2.printStackTrace();
                            }
                        }
                    }   
                }  
            }  
        });
		
		return;
    }
   
    public void setJSInterface( )
    {
    		mWebsetting.setJavaScriptEnabled(true);
    		setWebViewClient(new WebViewClient(){
			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) 
			{
				try {
					mWeburl = URLDecoder.decode(url, "UTF_8");
				} catch (UnsupportedEncodingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				if (mWeburl.contains("BaiWanIOSopenAlipay"))
				{
					//zfb.pay(mWeburl);
				}
				else if (mWeburl.contains("BaiWanIOSUPPay"))
				{
					String wares[] = mWeburl.split("\\|");
					payYinLian(wares[1]);
				} 
				else if (mWeburl.contains("BaiWanIOScloseWebView")) 
				{
					mContext.finish();
				} 
				else if( mWeburl.contains("BWWEBGAME|"))
				{
					int istart = mWeburl.indexOf("BWWEBGAME|");
					String gameurl = mWeburl.substring(istart + 10 );
					//Log.e("---->", gameurl );
					mContext.OpenGamePage( gameurl );
				}
				else 
				{
					view.loadUrl(url);
				}
				return true;
			}

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				super.onPageStarted(view, url, favicon);
				//mProgressDialog.show();
			}

			@Override
			public void onPageFinished(WebView view, String url) {
				super.onPageFinished(view, url);
				//mProgressDialog.hide();
				setInitialScale(100);
			}

			@Override
			public void onReceivedError(WebView view, int errorCode,
					String description, String failingUrl) 
			{
				super.onReceivedError(view, errorCode, description, failingUrl);
			}

		});
		
		setWebChromeClient(new WebChromeClient() {
		    @Override
		    public boolean onJsAlert(WebView view, String url, String message, final JsResult result) {
		        Builder b2 = new Builder( mContext );
		        b2.setTitle("友情提示").setMessage(message)
		                .setPositiveButton("ok",
		                        new AlertDialog.OnClickListener() {
		                            @Override
		                            public void onClick(DialogInterface dialog,
		                                    int which) {
		                                result.confirm();
		                                // MyWebView.this.finish();
		                            }
		                        });

		        b2.setCancelable(false);
		        b2.create();
		        b2.show();
		        return true;
		    }
		});

		setScrollBarStyle(View.SCROLLBARS_INSIDE_OVERLAY);
    }
    
    public void connectUrl( String strUrl )
    {
    		loadUrl( strUrl );
    }
    
	public void payYinLian(String orderNum) 
	{
		String serverMode="01";//测试环境01，生产环境00
		UPPayAssistEx.startPayByJAR( mContext, PayActivity.class, null, null,orderNum, serverMode);
	}
	
	/**
     * Disable the controls
     */
    @TargetApi(Build.VERSION_CODES.HONEYCOMB)
	private void HideZoomControls() 
    {
//        WebSettings settings = this.getSettings();
//         //基本的设置
//        settings.setJavaScriptEnabled(true);
//        settings.setBuiltInZoomControls(true);//support zoom
//        settings.setUseWideViewPort(true);
//        settings.setLoadWithOverviewMode(true);
//        if (Build.VERSION.SDK_INT >= 8) {
//            settings.setPluginState(WebSettings.PluginState.ON);
//        } 
//        else
//        {
//            settings.setPluginsEnabled(true);
//        }
//        this.setScrollBarStyle(View.SCROLLBARS_OUTSIDE_OVERLAY);
//         //去掉滚动条
//        this.setVerticalScrollBarEnabled(false);
//        this.setHorizontalScrollBarEnabled(false);

         //去掉缩放按钮
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.HONEYCOMB)
        {
            // Use the API 11+ calls to disable the controls
        		mWebsetting.setBuiltInZoomControls(true);
        		mWebsetting.setDisplayZoomControls(false);
        } 
        else 
        {
            // Use the reflection magic to make it work on earlier APIs
            getControlls();
        }
    }

    /**
     * This is where the magic happens :D
     */
    private void getControlls()
    {
        try 
        {
            Class webview = Class.forName("android.webkit.WebView");
            Method method = webview.getMethod("getZoomButtonsController");
            ZoomButtonsController zoom_controll = (ZoomButtonsController) method.invoke(this, true);
        } 
        catch (Exception e) 
        {
            e.printStackTrace();
        }
    }

    
}