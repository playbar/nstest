package com.jqbar;

import java.nio.Buffer;
import java.nio.ByteBuffer;










//import com.jqbar.wxapi.MobileMain;调用微信导这个包
import com.jqbar.MobileMain;

import android.R.integer;
import android.content.Context;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.graphics.Typeface;
import android.graphics.Paint.FontMetrics;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.opengl.GLSurfaceView;
import android.os.IBinder;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.ScaleAnimation;
import android.widget.LinearLayout;
import android.widget.RelativeLayout.LayoutParams;

/*
 * 实现类似MobileView的功能
 *
 */
public class MobileView extends SurfaceView implements IMobileView,Callback
{
	public boolean mbDraw1 = false;// 是否绘制鼠标位置
	public int mTouchX1 = 0;
	public int mTouchY1 = 0;

	private boolean mbFirst = true;
	
	private boolean mbDraw2 = false;// 是否绘制鼠标位置
	private int mTouchX2 = 0;
	private int mTouchY2 = 0;

	private int mMobileViewID; // accessed by native methods
	
	private MobileMain mMobileMain = null;
	
	private int swidth=0;//surfaceview初始化宽高
	private int sheight=0;
	private int dWH=0;//三星平板有工具条，进入转屏游戏，会出现残留工具条宽度
	public IBinder getWindowToken() {
		return super.getWindowToken();
	}

	public void bringToFront() {
		super.bringToFront();
	}

	public void onPause() {
		
	}

	public void onResume() {
		
	}

	public void RegisterCallBack() {
	}

	public void invalidateView1(int x, int y, boolean b )
    {
        mbDraw1 = b;
        mTouchX1 = x;
        mTouchY1 = y;
    }

    public void invalidateView2(int x, int y, boolean b )
    {
        mbDraw2 = b;
        mTouchX2 = x;	
        mTouchY2 = y;	
        invalidate();
    }

	public MobileView(Context context, int width, int height) {
		super(context);
		mMobileMain = (MobileMain) context;
		setFocusable(true);
		initCPP(width, height);
        getHolder().addCallback(this);
	}
	
	public void surfaceCreated(SurfaceHolder holder)
    {
		Log.e("MobileViewGLES", "surfaceCreated");
    		cppSurfaceCreate( getHolder().getSurface() );
    } 
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height)
    {   
    	Log.e("MobileViewGLES", "surfaceChanged");
    	swidth=width;
        sheight=height;
    	cppSurfaceChange( width,height);
    	if( mbFirst )
    	{
			 initCPP(width, height);
			 String strUrl = "http://m.jqbar.com/product/";
			 connectUrl( strUrl );
			 mbFirst = false;
    	}
    }
    public void surfaceDestroyed(SurfaceHolder holder) 
    { 
    	Log.e("MobileViewGLES", "surfaceDestroyed"); 
//	    termRenderGLES();
    } 

	public void postInvalidate() {
		super.postInvalidate();
		return;
	}
	
	public void refresh(){
		mMobileMain.refresh();
	}
	public void postInvalidate(int left, int top, int right, int bottom) {
		super.postInvalidate(left, top, right, bottom);
		return;
	}

	public void invalidate() {
		super.invalidate();
		return;
	}

	public void loadPageStart() {
		mMobileMain.loadPageStart();
	}

	public void loadPagePercent(int per) {
		mMobileMain.loadPagePercent(per);
	}

	public void loadPageEnd() {
		mMobileMain.loadPageEnd();
	}

	public void flashLoadingEnd() {
		mMobileMain.flashLoadingEnd();
	}

	public void closeSubView() {
		mMobileMain.closeSubView();
	}

	public void startFlashView(int sta) {
		mMobileMain.startFlashView(sta);
		return;
	}

	public void closeFlashView() {
		mMobileMain.closeFlashView();
		return;
	}

	public void flashSaveFinish() {
		mMobileMain.flashSaveFinish();
		return;
	}

	public void SetMode(int mode) {
		mMobileMain.SetMode(mode);

		return;
	}
	
	public void MakeErrorPage()
	{
		mMobileMain.MakeErrorPage();
	}

	public void CallTextEdit(String inText, int ilength, int style) {
		mMobileMain.CallTextEdit(inText, ilength, style);
		return;
	}

	public void changeScreenLandscape(int mode)
	{
		mMobileMain.SetMode(1);
		return;
	}

	public void changeScreenPortrait(int mode) {
		mMobileMain.SetMode(0);
		return;
	}

	public void changeScreenDir( int mode ) 
	{
		//设置屏幕方向
		mMobileMain.changeScreenDir( mode );
		//return mMobileMain.orientation;
		DisplayMetrics dm = new DisplayMetrics();
		//获取屏幕信息
		mMobileMain.getWindowManager().getDefaultDisplay().getMetrics(dm);
		int screenWidth = dm.widthPixels;
		int screenHeight = dm.heightPixels;
		//适配GT-800三星平板
		if ((dWH=Math.abs(screenHeight-swidth))<100&&mode==-1) {//当工具条（例如GT-800三星平板）小于100且进入游戏转屏时
			Configuration configuration = getResources().getConfiguration();
			if (configuration.orientation == Configuration.ORIENTATION_LANDSCAPE) {
				screenHeight=screenHeight+dWH;//横屏转竖屏游戏
			}
	  }
		nativesetWinWH(screenWidth,screenHeight,mode);
	}

	public void setScreenWHJava(int width, int height) {
		setScreenWH(width, height);
	}

	public void showImgProgress() {
		mMobileMain.showImgProgress();
		return;
	}


	public String getLocalMacAddress() {
		WifiManager wifi = (WifiManager) mMobileMain.getSystemService(Context.WIFI_SERVICE);
		WifiInfo info = wifi.getConnectionInfo();
		String strMac = info.getMacAddress();
		if (strMac == null)
			return null;
		strMac = strMac.replaceAll(":", "");
		return strMac;
	}

	public void initCPP(int width, int height) {
		nativeInit();
		String strMac = getLocalMacAddress();
		// 800,480,91300001,null,F46DE2E15C2E
		CreateView(width, height, mMobileMain.mChannelValue,
				mMobileMain.mUAValue, strMac);
		return;
	}

	public void finalize() {
		// nativeFinalize();
	}

	public void Release() {
		nativeFinalize();
	}

	public void onDrawFrame() {
	}

	public void onSurfaceChanged(int width, int height) {
	}

	public int onSurfaceCreated() {
		return 0;
	}

	// public native void getOutBuffer( ByteBuffer buffer );
	public native void setBmpData(Bitmap buffer);
	
	// public native void OnPaint(Canvas canva);

	public native void cppSurfaceChange( int width,int height);
	
	public native void cppSurfaceCreate( Surface surface );
	
	public native void termRenderGLES();
	
	public native void nativesetWinWH(int screenWidth,int screenHeight,int mode);
	
	public native void nativeInit();

	public native void CreateView(int width, int height, String strCid,
			String strUA, String strMac);

	public native void setScreenWH(int width, int height);

	public native void freeParameter();

	public native void nativeFinalize();

	public native void OnIdle();

	public native void timeTickCpp();

	public native void setTopPos(int pos);

	public native void setBottomPos(int pos);

	// public native boolean onTouchEvent(MotionEvent event);
	//
	public native boolean onKeyDownCpp(int keyCode, KeyEvent event);

	public native boolean onKeyUpCpp(int keyCode, KeyEvent event);

	public native void onTouchDownCpp(int x, int y, int pos);

	public native void onTouchMoveCpp(int x, int y, int pos);

	public native void onTouchUpCpp(int x, int y, int pos);

	public native boolean onDomQuit();

	public native boolean onDomStop();

	public native boolean onDomHome();

	public native boolean onDomBack();

	public native boolean onDomForward();

	public native boolean onDomAddbook();

	public native boolean onDomUpdate();

	public native void SetXEdit(String str);

	public native int clearCache();

	public native boolean isFlashGame();

	public native boolean canFlashSave();

	public native void flashSave();

	public native void flashSaveCancel();

	public native boolean exitFlash();
	
	public native void exit_setShader();

	public native void connectUrl(String strUrl);

	public native void setDeviceId(String strDevice);

	public native void upScore();

	public native boolean isUpScore();

	public native void flashPause();

	public native void flashResume();

	public native void flashRestart();

	public native boolean canBack();

	public native boolean canForward();

	public native boolean isCurrentHomePage();

	public native int flashContentMode();

	public void setFlashOriginalScreen() {
	};

	public native void flashZoomInOrOut(int inOut);
	
	public native void flashZoomInOut(int data);

	public native int flashLockMode();

	public native void setFlashLockMode(boolean bLock);

	public native void flashLockZoomInOut();

	public native void flashUnLockZoomInOut();

	public native void setFlashZoomMode(int mode);

	public native void flashPointZoom();

	public native int getZoomPecent();

	public native void switchHelp();

	public native void switchAcc();

	public native void switchButton();

	public native void senseAcc(float x, float y, float z);

	public native boolean hasFlashACC();

	public native boolean isFlashAccEnable();

	public native boolean hasFlashButton();

	public native void checkUpgrade();

	public native void closePopView();

	public native void openPayWindow();

	public native boolean canPayWindow();

	public native boolean isPause();

	public native void setSilence(boolean bsilence);

	public native void openFeedbackView();

	public native void setViewRegion(int left, int top, int right, int bottom);

	public native String encUIDStr(String str); // zhengjl 2012.03

	public native String getAppUA();// liupan 2012.05.16

	public native String getAppHeaders();

	// @Override
	// protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
	// // TODO Auto-generated method stub
	// setMeasuredDimension(GlobalFun.BWScreenWidth, GlobalFun.BWScreenHeight);
	// // super.onMeasure(widthMeasureSpec, heightMeasureSpec);
	// }

	@Override
	public void showBarandsale( int type ) {
		// TODO Auto-generated method stub
		mMobileMain.showBarandsale( type );
	}

	@Override
	public void WXSendText(String pdata, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendText(pdata, type);
	}

	@Override
	public void WXSendPhoto(byte[] pdata1, int pdata2, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendPhoto(pdata1, pdata2, type);
	}

	@Override
	public void WXSendLink(String title, String description, byte[] pdata1,
			String weburl, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendLink(title, description, pdata1, weburl, type);
	}

	@Override
	public void WXSendMusic(String title, String description, byte[] pdata1,
			String musicurl, String musicdateUrl, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendMusic(title, description, pdata1, musicurl,
				musicdateUrl, type);
	}

	@Override
	public void WXSendVideo(String title, String description, byte[] pdata1,
			String musicurl, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendVideo(title, description, pdata1, musicurl, type);
	}

	@Override
	public void WXSendAppMessage(String title, String description,
			byte[] pdata1, String extInfo, String url, byte[] pdata2, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendAppMessage(title, description, pdata1, extInfo, url,
				pdata2, type);
	}

	@Override
	public boolean WeiXinInStalled() {
		// TODO Auto-generated method stub
		return mMobileMain.WeiXinInStalled();
	}

	@Override
	public void WXSendNoGif(byte[] pdata1, byte[] pdata2, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendNoGif(pdata1, pdata2, type);
	}

	@Override
	public void WXSendGif(byte[] pdata1, byte[] pdata2, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendGif(pdata1, pdata2, type);
	}

	@Override
	public void WXSendFile(String title, String description, byte[] pdata1,
			byte[] pdata2, String fileExt, int type) {
		// TODO Auto-generated method stub
		mMobileMain.WXSendFile(title, description, pdata1, pdata2, fileExt,
				type);
	}

	@Override
	public void LoadPayUrl(String url) {
		// TODO Auto-generated method stub
		mMobileMain.LoadPayUrl(url);
	}

	@Override
	public void showDomobOfferWall(String userid) {
		// TODO Auto-generated method stub
		mMobileMain.showDomobOfferWall(userid);
	}

	@Override
	public void NoticePayUrl(String url) {
		// TODO Auto-generated method stub
		mMobileMain.NoticePayUrl(url);
	}

	@Override
	public void openWebViewUrl(String url) {
		// TODO Auto-generated method stub
		mMobileMain.openWebViewUrl(url, 3);

	}

}
