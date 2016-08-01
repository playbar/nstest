package com.jqbar;


import com.jqbar.MobileMain;

import android.R.integer;
import android.os.IBinder;
import android.view.KeyEvent;
import android.widget.LinearLayout;

/*
 * 实现类似MobileView的功能
 *
 */
 public interface IMobileView
{
	MobileMain mMobileMain = null;
	
	public IBinder getWindowToken();
	public void bringToFront();
	public void onPause();
	public void onResume();
	public void RegisterCallBack();
    public void invalidateView1(int x, int y, boolean b );
    public void invalidateView2(int x, int y, boolean b );

    public void postInvalidate();

    public  void postInvalidate(int left, int top, int right, int bottom);

    public  void invalidate();

    public void loadPageStart();

	 public void loadPagePercent( int per );

	 public void loadPageEnd();
	 
	 public void showBarandsale( int type );
	 public void flashLoadingEnd();
	 
	 public void closeSubView();

	public void startFlashView( int sta );

	public void closeFlashView();

    public void flashSaveFinish( );

    public void SetMode( int mode );
    
    public boolean WeiXinInStalled ();
    
    public void WXSendText( String pdata,int type);
    
    public void WXSendPhoto( byte[] pdata1,int pdata2,int type);
    
    public void WXSendLink( String title,String description,byte[] pdata1,String weburl,int type);
    
    public void WXSendMusic( String title,String description,byte[] pdata1,String musicurl,String musicdateUrl,int type);

    public void WXSendVideo( String title,String description,byte[] pdata1,String musicurl,int type);
   
    public void WXSendAppMessage( String title,String description,byte[] pdata1,String extInfo,String url, byte[] pdata2,int type);
  
    public void WXSendNoGif( byte[] pdata1,byte[] pdata2,int type);
   
    public void WXSendGif( byte[] pdata1,byte[] pdata2,int type);
   
    public void WXSendFile( String title,String description,byte[] pdata1,byte[] pdata2,String fileExt,int type);
    public void LoadPayUrl(String url);
    public void showDomobOfferWall(String userid);
    public void NoticePayUrl(String url);
    public void openWebViewUrl(String url);
    public void CallTextEdit(String inText, int ilength, int style );
    
    public void setScreenWHJava( int width, int height );
    
    public void showImgProgress();
    
    public String getLocalMacAddress();
    
    public void initCPP( int width, int height );
	
    public void finalize();

	public void Release();

	public void nativeInit();

    public void CreateView(int width, int height, String strCid, String strUA, String strMac );

    public void setScreenWH(int width, int height );
    
    public void freeParameter();
    
    public void nativeFinalize();

    public void OnIdle();

    public void timeTickCpp();
    
    public void setTopPos(int pos);
    public void setBottomPos(int pos);
    

    public boolean onKeyDownCpp(int keyCode, KeyEvent event);

    public boolean onKeyUpCpp(int keyCode, KeyEvent event);

    public void onTouchDownCpp( int x, int y, int pos );

    public void onTouchMoveCpp(int x, int y, int pos );

    public void onTouchUpCpp( int x, int y, int pos );

    public boolean onDomQuit();

    public boolean onDomStop();

    public boolean onDomHome();

    public boolean onDomBack();

    public boolean onDomForward();

    public boolean onDomAddbook();

    public boolean onDomUpdate();

	public void SetXEdit( String str );

	public int clearCache();

	public boolean isFlashGame();

	public boolean canFlashSave();

	public void flashSave();

	public void flashSaveCancel();

	public boolean exitFlash();
	
	public void exit_setShader();

	public void connectUrl( String strUrl );
	
	public void setDeviceId( String strDevice );

	public void upScore();

	public boolean isUpScore();

	public void flashPause();

	public void flashResume();

	public void flashRestart();

	public boolean canBack();

	public boolean canForward();

	public boolean isCurrentHomePage();

	public int flashContentMode();
	
	public void setFlashOriginalScreen( );

	public void flashZoomInOut( int data );

	public int flashLockMode();

	public void setFlashLockMode( boolean bLock );

	public void flashLockZoomInOut();

	public void flashUnLockZoomInOut();

	public void setFlashZoomMode( int mode );
	
	public void flashZoomInOrOut(int inOut);
	
	public void flashPointZoom();

	public int getZoomPecent();

	public void switchHelp();

	public void switchAcc();

	public void switchButton();

	public void senseAcc(float x, float y, float z );

	public boolean hasFlashACC();

	public boolean isFlashAccEnable();

	public boolean hasFlashButton();

	public void checkUpgrade();

	public void closePopView();

	public void openPayWindow();
	
	public boolean canPayWindow();
	
	public boolean isPause();
	
	public void setSilence( boolean bsilence );
	
	public void openFeedbackView();
	
	public void setViewRegion( int left, int top, int right, int bottom );

	public String encUIDStr( String str );	// zhengjl 2012.03
	public String getAppUA();//liupan 2012.05.16 	
	public String getAppHeaders();
	public void onDrawFrame();
	
	public void onSurfaceChanged( int width, int height );
	
	public int onSurfaceCreated();
}

