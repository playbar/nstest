package com.jqbar;

import java.io.UnsupportedEncodingException;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.net.URLDecoder;
import java.util.List;

import com.unionpay.UPPayAssistEx;
import com.unionpay.uppay.PayActivity;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.StateListDrawable;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.webkit.JsResult;
import android.webkit.WebSettings.LayoutAlgorithm;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.ZoomButtonsController;

public class ChargeWebView extends Activity {
	WebView webview;
	Button refresh_btn;
	Button close_btn;
	LinearLayout mMainLayoutView;
	FrameLayout mTitleFrame2;
	ScrollView mScrolView;
	private BwPayZFB zfb = null;
	Display display = null;
	private String weburl = "";
	private int SCALE_VAL = 75;

	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		final ProgressDialog mProgressDialog;
		

		this.requestWindowFeature(Window.FEATURE_NO_TITLE);
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		
		GlobalFun.setOrientation( this );
		
		display = getWindowManager().getDefaultDisplay();
		zfb = MobileMain.zfb;

		refresh_btn = new Button(this);
		FrameLayout.LayoutParams paramRefresh = new FrameLayout.LayoutParams( 50, 50);
		paramRefresh.gravity = Gravity.LEFT;// 此处相当于布局文件中的Android:layout_gravity属性
		paramRefresh.setMargins(15, 0, 0, 0);
		refresh_btn.setLayoutParams(paramRefresh);
		refresh_btn.setGravity(Gravity.CENTER);
		Drawable normal_refresh_bg = this.getResources().getDrawable( R.drawable.bar_float_refresh);
		Drawable select_refresh_bg = this.getResources().getDrawable( R.drawable.bar_float_refresh_untouch);

		StateListDrawable refresh_btn_bg = new StateListDrawable();
		refresh_btn_bg.addState(new int[] { android.R.attr.state_active },
				normal_refresh_bg);
		refresh_btn_bg.addState(new int[] { android.R.attr.state_pressed,
				android.R.attr.state_enabled }, select_refresh_bg);
		refresh_btn_bg.addState(new int[] { android.R.attr.state_focused,
				android.R.attr.state_enabled }, normal_refresh_bg);
		refresh_btn_bg.addState(new int[] { android.R.attr.state_enabled },
				normal_refresh_bg);
		refresh_btn.setBackgroundDrawable(refresh_btn_bg);

		close_btn = new Button(this);
		FrameLayout.LayoutParams paramClose = new FrameLayout.LayoutParams(50, 50);
		paramClose.gravity = Gravity.RIGHT;// 此处相当于布局文件中的Android:layout_gravity属性
		//paramClose.setMargins(0, 0, 15, 0);
		
		close_btn.setLayoutParams(paramClose);
		close_btn.setGravity(Gravity.CENTER);
		Drawable normal_close_bg = this.getResources().getDrawable( R.drawable.bar_exit);
		Drawable select_close_bg = this.getResources().getDrawable( R.drawable.bar_exit_touch);
		StateListDrawable close_btn_bg = new StateListDrawable();
		close_btn_bg.addState(new int[] { android.R.attr.state_active },
				normal_close_bg);
		close_btn_bg.addState(new int[] { android.R.attr.state_pressed,
				android.R.attr.state_enabled }, select_close_bg);
		close_btn_bg.addState(new int[] { android.R.attr.state_focused,
				android.R.attr.state_enabled }, normal_close_bg);
		close_btn_bg.addState(new int[] { android.R.attr.state_enabled },
				normal_close_bg);
		close_btn.setBackgroundDrawable(close_btn_bg);
		
		mTitleFrame2 = new FrameLayout(this);
		FrameLayout.LayoutParams paramTitle = new FrameLayout.LayoutParams( LayoutParams.FILL_PARENT, 50);
		mTitleFrame2.setLayoutParams(paramTitle);
		mTitleFrame2.setBackgroundResource(R.drawable.zsht_keyboard_title);
		mTitleFrame2.addView(refresh_btn);
		mTitleFrame2.addView(close_btn);
		//setContentView( mTitleFrame2 );
		
		webview = new WebView(this);
		LinearLayout.LayoutParams paramWebView = new LinearLayout.LayoutParams(
				//LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.FILL_PARENT );
				display.getWidth(), display.getHeight()) ;
		webview.setLayoutParams(paramWebView);
		
		webview.getSettings().setJavaScriptEnabled(true);
		webview.getSettings().setJavaScriptCanOpenWindowsAutomatically( true );

		webview.getSettings().setSupportZoom( true );
		webview.getSettings().setUseWideViewPort( true );
		webview.getSettings().setLoadWithOverviewMode(true);
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.HONEYCOMB)
        {
        	webview.getSettings().setDisplayZoomControls( false );
        }
		webview.getSettings().setLayoutAlgorithm( LayoutAlgorithm.NORMAL );
		webview.getSettings().setDefaultZoom( WebSettings.ZoomDensity.FAR );
		webview.setInitialScale(SCALE_VAL);
		webview.getSettings().setBuiltInZoomControls(true);
		HideZoomControls();
		float scale = webview.getScale();	
		
//		mScrolView = new ScrollView(this);
//		LayoutParams paramview = new LayoutParams( 
//				LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.FILL_PARENT );
//				display.getWidth(), display.getHeight() );
//		mScrolView.setLayoutParams(paramview);
//		mScrolView.addView( webview);
		
		
		mMainLayoutView = new LinearLayout(this);
		LinearLayout.LayoutParams param = new LinearLayout.LayoutParams(
				//LinearLayout.LayoutParams.FILL_PARENT, LinearLayout.LayoutParams.FILL_PARENT );
				display.getWidth(), display.getHeight());
		mMainLayoutView.setLayoutParams(param);
		mMainLayoutView.setOrientation(LinearLayout.VERTICAL);
		mMainLayoutView.addView(mTitleFrame2);
		
		//mMainLayoutView.addView(mScrolView);
		mMainLayoutView.addView(webview);
		setContentView(mMainLayoutView);
		
	
		mProgressDialog = new ProgressDialog(this);
		mProgressDialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
		mProgressDialog.setMessage("数据载入中，请稍候！");
		mProgressDialog.setOnCancelListener(new DialogInterface.OnCancelListener() {

					@Override
					public void onCancel(DialogInterface dialog) {
						mProgressDialog.hide();
						webview.stopLoading();

					}
				});

		webview.setWebViewClient(new WebViewClient(){
			@Override
			public boolean shouldOverrideUrlLoading(WebView view, String url) {
				try {
					weburl = URLDecoder.decode(url, "UTF_8");
				} catch (UnsupportedEncodingException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				if (weburl.contains("BaiWanIOSopenAlipay")) {
					zfb.pay(weburl);
				} else if (weburl.contains("BaiWanIOSUPPay")) {
					String wares[] = weburl.split("\\|");
					payYinLian(wares[1]);
				} else if (weburl.contains("BaiWanIOScloseWebView")) {
					finish();
					android.os.Process.killProcess(android.os.Process.myPid());
				} else {
					view.loadUrl(url);
				}
				return true;
			}

			@Override
			public void onPageStarted(WebView view, String url, Bitmap favicon) {
				super.onPageStarted(view, url, favicon);
				mProgressDialog.show();
			}

			@Override
			public void onPageFinished(WebView view, String url) {
				super.onPageFinished(view, url);
				mProgressDialog.hide();
				webview.setInitialScale(SCALE_VAL);
			}

			@Override
			public void onReceivedError(WebView view, int errorCode,
					String description, String failingUrl) {
				super.onReceivedError(view, errorCode, description, failingUrl);
			}

		});
		
		webview.setWebChromeClient(new WebChromeClient() {
		    @Override
		    public boolean onJsAlert(WebView view, String url, String message, final JsResult result) {
		        Builder b2 = new Builder( ChargeWebView.this);
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
		    @Override
		    public void onProgressChanged( WebView view, int progress )
		    {
		    		if( progress == 100 )
		    		{
		    			super.onProgressChanged( view , progress );
		    			webview.requestFocus();
		    		}
		    }
		});
		
		webview.setOnFocusChangeListener(new View.OnFocusChangeListener() 
		{  
            @Override  
            public void onFocusChange(View v, boolean hasFocus) 
            {  
                if(hasFocus)  
                {  
                		//webview.setInitialScale(SCALE_VAL);
                    try 
                    {  
                        Field defaultScale = WebView.class.getDeclaredField("mDefaultScale");  
                        defaultScale.setAccessible(true);  
                        //WebViewSettingUtil.getInitScaleValue(VideoNavigationActivity.this, false )/100.0f 是我的程序的一个方法，可以用float 的scale替代  
                        defaultScale.setFloat(webview, 1);  
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
                        		Object zoomValue = zoomManager.get( webview );
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
                            		Object webviewclassic = mProviderField.get( webview);
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

//		webview.setScrollBarStyle(View.SCROLLBARS_INSIDE_OVERLAY);
//		SharedPreferences settings = getSharedPreferences( GlobalFun.BW_CONFIGURE_FILE, 0);
//		String url = settings.getString("noticeurl", "");
		Intent intent = getIntent();
		Bundle bl = intent.getExtras();
		String url = bl.getString("BW_WEBV");
		webview.loadUrl(url);
		refresh_btn.setOnClickListener(new Button.OnClickListener() {

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				webview.reload();
			}
		});

		close_btn.setOnClickListener(new Button.OnClickListener() {

			@Override
			public void onClick(View v) {
				if (mProgressDialog != null) {
					mProgressDialog.dismiss();
				}
				//zfb.onTouchCpp();
				finish();
				//zfb.setChargeView_width(display.getWidth());
				//zfb.setChargeView_height(display.getHeight());
			}
		});
	}
	
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
    		if(keyCode == KeyEvent.KEYCODE_BACK)
        {
            ConfirmExit();//按了返回键，但已经不能返回，则执行退出确认
             return true; 
        }   
        return super.onKeyDown(keyCode, event);   
    }
    
    public void ConfirmExit()
    {
        //退出确认
        AlertDialog.Builder dlgBuild=new AlertDialog.Builder(ChargeWebView.this);
        dlgBuild.setTitle("退出充值");
        dlgBuild.setMessage("是否退出充值?");
        dlgBuild.setPositiveButton("是", new DialogInterface.OnClickListener()
        {//退出按钮
            public void onClick(DialogInterface dialog, int i) 
            {
                // TODO Auto-generated method stub
            	finish();
				android.os.Process.killProcess(android.os.Process.myPid());
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
    
    @TargetApi(Build.VERSION_CODES.HONEYCOMB)
 	private void HideZoomControls() 
     {
          //去掉缩放按钮
         if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.HONEYCOMB)
         {
             // Use the API 11+ calls to disable the controls
        	 	webview.getSettings().setBuiltInZoomControls(true);
        	 	webview.getSettings().setDisplayZoomControls(false);
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
    
	public void payYinLian(String orderNum) 
	{
		String serverMode="00";//测试环境01，生产环境00
		UPPayAssistEx.startPayByJAR( ChargeWebView.this,PayActivity.class, null, null,orderNum, serverMode);
	}
	/////
	static {
		System.loadLibrary("entryex");
	}
	/////
}
