package com.jqbar;
//package com.jqbar;
//
//import java.io.BufferedReader;
//import java.io.File;
//import java.io.FileNotFoundException;
//import java.io.FileOutputStream;
//import java.io.FileReader;
//import java.io.IOException;
//import java.io.InputStream;
//import java.util.ArrayList;
//import java.util.List;
//import java.util.Timer;
//import java.util.TimerTask;
//
//import org.json.JSONException;
//import org.json.JSONObject;
//
//import android.app.Activity;
//import android.app.ActivityManager;
//import android.app.AlertDialog;
//import android.app.Dialog;
//import android.app.KeyguardManager;
//import android.app.ProgressDialog;
//import android.app.ActivityManager.MemoryInfo;
//import android.content.BroadcastReceiver;
//import android.content.Context;
//import android.content.DialogInterface;
//import android.content.Intent;
//import android.content.IntentFilter;
//import android.content.SharedPreferences;
//import android.content.SharedPreferences.Editor;
//import android.content.pm.ActivityInfo;
//import android.content.res.AssetManager;
//import android.content.res.Configuration;
//import android.graphics.Rect;
//import android.graphics.drawable.AnimationDrawable;
//import android.graphics.drawable.ColorDrawable;
//import android.hardware.Sensor;
//import android.hardware.SensorEvent;
//import android.hardware.SensorEventListener;
//import android.hardware.SensorManager;
//import android.net.Uri;
//import android.os.Bundle;
//import android.os.Handler;
//import android.os.Message;
//import android.os.Parcelable;
//import android.os.PowerManager;
//import android.os.PowerManager.WakeLock;
//import android.provider.Settings.Secure;
//import android.telephony.TelephonyManager;
//import android.text.InputFilter;
//import android.text.InputType;
//import android.text.format.Formatter;
//import android.text.format.Time;
//import android.util.FloatMath;
//import android.util.Log;
//import android.view.Display;
//import android.view.Gravity;
//import android.view.KeyEvent;
//import android.view.Menu;
//import android.view.MotionEvent;
//import android.view.View;
//import android.view.View.OnClickListener;
//import android.view.View.OnTouchListener;
//import android.view.Window;
//import android.view.WindowManager;
//import android.view.animation.AlphaAnimation;
//import android.view.animation.Animation;
//import android.view.animation.AnimationSet;
//import android.view.animation.AnimationUtils;
//import android.view.animation.LinearInterpolator;
//import android.view.animation.ScaleAnimation;
//import android.view.animation.TranslateAnimation;
//import android.view.inputmethod.InputMethodManager;
//import android.widget.AbsoluteLayout.LayoutParams;
//import android.widget.Button;
//import android.widget.EditText;
//import android.widget.FrameLayout;
//import android.widget.ImageView;
//import android.widget.LinearLayout;
//import android.widget.ProgressBar;
//import android.widget.TextView;
//import android.widget.Toast;
//
//import com.jqbar.layout.MulTouchPad;
//import com.jqbar.layout.ViewLayout;
//import com.jqbar.sdk.BwSDK;
//import com.skymobi.freesky.FreeSkySdk;//maopao
//
//public class MobileMain_maopao extends Activity
//{
//	//private boolean mbShowMenu = false; //菜单是否处于显示状态；true为显示，flash为隐藏
//	ArrayList<String> talkList;
//	
//    private int mMobileMainID; // accessed by native methods
//    private volatile boolean mbRuning = false;
//    private boolean mbFirstLoad = true;
//
//    public int mScreenWidth =0;
//    public int mScreenHeight = 0;
//    
//    private int mHeight44 = 0;
////    private int mHeight32=0;
//
//	private IMobileView mMobileView = null;
//	
//	private ViewLayout mViewLayout = null; //菜单布局
//
//	private volatile int mUserStatus;  //用户状态
//
//	private volatile int mViewStatus;   //视图大小状态
//
//	private volatile int mViewPopStatus; //弹出视图状态
//
//    private volatile int mUserActionStatus; //用户操作状态
//
//	private volatile int mMenuStatus;   //菜单状态
//	
//	private int miMoveShreshole = 0;  //移动阈值设置
//
//	 private static final String FILE_PROTOCOL = "D:";  //打开本地文件协议定义
//
//	 private FrameLayout mLayoutMobileMain;
//	 
//    private View mMenuBrowser;      //浏览器菜单
//    private View mMenuDirection;    //方向键菜单
//    private View mMenuBrowserPop;
//    private View mMenuFloatView;
//    private View mLinearView;
//    private View mMenuFloatGame;    
//    private View mMenuFloatMedia;
//    private View mMenuFloatWebGame;
//    private View mMenuUserSetting;
//    //旧版菜单
//   
//    private boolean mbShowOldMenu;//是否显示旧菜单
//    private View mMenuGameOld; //game menu
//    private Button mBtnGameSwitchOld;
//    private Button mBtnGameZoomOld;
//    private Button mBtnGameSettingOld;
//    private Button mBtnGameSaveOld;
//    private Button mBtnGameHelpOld;
//    private Button mBtnGameCloseOld;
//    private boolean mbExchangeMenu;
//    //webgame menu
//    private View mMenuWebGameOld;
//    private Button mBtnWebGameSwitchOld;
//    private Button mBtnWebGameZoomOld;
//    private Button mBtnWebGameSettingOld;
//    private Button mBtnWebGameRefreshOld;
//    private Button mBtnWebGameRechargeOld;
//    private Button mBtnWebGameCloseOld;
//    private TextView timeWebGame;
////    private Calendar mCalendar;
////    private Calendar mCalendarFloat;
//    private String mFormat;
//    private TextView timeWebGameFloat;
//    private int timeCount=0;
//    private boolean showTimeDot=true;//floatView中时间之间:显示之用
//    private final static String m12 = "h:mm:ss";
//	private final static String m24 = "k:mm:ss";
//	private String mFormat_float;
//	private final static String m12_float = "h:mm";
//	private final static String m24_float = "k:mm";
//    //media menu
//    private View mMenuMediaOld;
//    private Button mMenuMediaSwitchOld;
//    private Button mMenuMediaZoomOld;
//    private Button mMenuMediaSettingOld;
//    private Button mMenuMediaSaveOld;
//    private Button mMenuMediaCloseOld;
//    //旧版菜单
//
//    //浏览器工具栏
//    private Animation mAniEnter;
//    private Animation mAniExit;
//    private Button mBtnMenuHome;
//    private Button mBtnMenuLocal;
//    private Button mBtnMenuBack;
//    private Button mBtnMenuRefresh;
//    private Button mBtnMenuForward;  // for pad
//    private Button mBtnMenuMore;
//    private ProgressBar mProgressLoading;   //加载页面显示进度
//   
//    private Button mBtnGameSave;
//    private Button mBtnGameSetting;
//    private Button mBtnGameFloatBack;
//    private Button mBtnGameFloatHelp;
//    private Button mBtnGameFloatClose;
//    private Button mBtnGameFloatSwitch;
//    
//    private Button mBtnMediaSave;
//    private Button mBtnMediaBack;
//    private Button mBtnMediaClose;
//    private Button mBtnMediaSetting;
//    private Button mBtnMediaFloatSwitch;
//    
//    private Button mBntWebGameRecharge;
//    private Button mBtnWebGameSetting;
//    private Button mBtnWeGameBack;
//    private Button mBtnWebGameFresh;
//    private Button mBtnWebGameClose;
//    private Button mBtnWebGameFloatSwitch;
//    
//    private Button mBtnSettingVirbtn;
//    private Button mBtnSettingMouse;
//    private Button mBtnSettingBack;
//    private Button mBtnSettingSwitch;
//    private boolean mbShowVirBtn;
//    //private Button mBtnSettingEliminate;
//
//    private FrameLayout mFrameMore;
//    //private ImageView mImgLock;
//    private FrameLayout mFrameZoom;
//    
//    private ImageView mImgProgress;
//    private ImageView mImgProgressOld;
//    private ImageView mImgProgressGameOld;
//    private ImageView mImgProgressMediaOld;
//    
//    private ImageView mImgVLoading;
//    private ImageView mImgVLoadingProgress;
//    //private Animation mAniLoadingEnter;
//    //private Animation mAniLoadingExit;
//    //private static final long SPLASHTIME = 1800;
//    
//    private SensorManager mSensorManager = null;
//    private Sensor mSensor = null;
//    private boolean mbSensorOpen = false;  //重力感应是否打开
//    private WakeLock mWakeLock = null;
//    private SensorEventListener mSensorListener = null;
//    
//    //String mUAValue = "GoogleN1;10.0.035;Android2.3;BWTEST/2.2.1";
////    String mUAValue = "BW";			//UA
//    String mUAValue = null;	
//	String mChannelValue = null;	//渠道号
//	String strUrl = null;
////	String UA;//about activity
////	String appHeaders;
////	private boolean first_about=true;
//	public String mCpu = null;//由jni层调用
//	public String mRam = null;
//	// msg type
//	public static final int MSG_UPDATEVIEW = 1;
//	public static final int MSG_LOADINGBEGIN = 2;
//	public static final int MSG_LOADINGEND = 3;
//	public static final int MSG_USERPROCESS = 4;
//	public static final int MSG_TIMEER = 5;
//	public static final int MSG_SHOWFLOATVIEW = 6;
//	public static final int MSG_SDKINIT = 7;
//
//	private Handler mProcessUseMsg = new Handler()
//    {
//        public void handleMessage(Message msg)
//        {
//        	if( msg.what == MSG_UPDATEVIEW)
//        	{
//    			mMobileView.OnIdle();
//        		Message msgUpdate = new Message();
//        		msgUpdate.what = MSG_UPDATEVIEW;
//        		int ideleyTime = 20;
//        		if( ! mbRuning )
//        			ideleyTime = 1000;
//        		mProcessUseMsg.sendMessageDelayed(msgUpdate, ideleyTime );
//        	}
//        	else if( msg.what == MSG_LOADINGBEGIN )
//        	{
//        		startLoadingImg();
//        	}
//        	else if( msg.what == MSG_LOADINGEND )
//        	{
//        		closeLoadingImg();
//        	}
//        	else if( msg.what == MSG_USERPROCESS )
//        	{
//        		Log.e("MobileMain_maopao", "MSG_USERPROCESS begin" );
//        		//GlobalFun.GFunCopyFontFile( MobileMain_maopao.this );
//        		Thread userThread = new Thread( new UserProcessThread() );
//        		userThread.start();
//          		Log.e("MobileMain_maopao", "MSG_USERPROCESS end" );
//        	}
//        	else if( msg.what == MSG_TIMEER )
//        	{
//        		//Log.e("MobileMain_maopao", "MSG_TIMEER" );
//        	    mMobileView.timeTickCpp();
//        	    timeTickActoin();
//        	    if( mbRuning )
//        	    {
//	        	    Message msgTime = new Message();
//	        	    msgTime.what = MSG_TIMEER;
//	                mProcessUseMsg.sendMessageDelayed(msgTime, 50 );
//        	    }
//        	}
//        	else if( msg.what == MSG_SHOWFLOATVIEW )
//        	{
//        		showMenuFloatView();
//        	}
//        	else if( msg.what == MSG_SDKINIT )
//        	{
//        		bwSDK.initSDKPay();
//        	}
//            super.handleMessage(msg);
//        }
//    };
//    
//    class UserProcessThread implements Runnable
//    {
//    	public void run()
//    	{
//    		Log.e("MobileMain_maopao", "UserProcessThread begin" );
//    		GlobalFun.GFunCopyFontFile( MobileMain_maopao.this );
//    		try 
//    		{
//    			startPushService();
//    		}
//    		catch( Exception e)
//    		{
//    			Log.e("UserProcessThread", "Exception" );
//    		}
//      		Log.e("MobileMain_maopao", "UserProcessThread end" );
//    	}
//    }
//   
//    
//	 static AssetManager mAssetManager;
//	//start: zhengjl 2011.09.09
//    private void registerScreenActionReceiver()
//    {
//        final IntentFilter filter = new IntentFilter();
//        filter.addAction(Intent.ACTION_USER_PRESENT);
//        registerReceiver(mUnlockReceiver, filter);
//    }
//
//    private final BroadcastReceiver mUnlockReceiver = new BroadcastReceiver()
//    {
//        @Override
//        public void onReceive(final Context context, final Intent intent)
//        {
//			if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA || mUserStatus == UserStatus.FLASH_TYPE_GAME )
//			{
//				mMobileView.flashResume();
//			}
//			else if( mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
//			{
//			    mMobileView.setSilence( false );
//			}
//			mbRuning = true;
//        }
//    };
//    //end: zhengjl 2011.09.09
//    
//    public void btnMenuHomeEnabled( boolean b )
//    {
//    	if( b )
//    	{
//    		mBtnMenuHome.setEnabled( true );
// 	        mBtnMenuHome.setBackgroundResource( R.drawable.menu_home );
//    	}
//    	else
//    	{
//    		mBtnMenuHome.setEnabled( false );
//            mBtnMenuHome.setBackgroundResource( R.drawable.bar_home_untouch );
//    	}
//    }
//    
//    public void btnMenuLocalEnabled( boolean b )
//    {
//    	if( b )
//    	{
//    		  mBtnMenuLocal.setEnabled( true );
//    	      mBtnMenuLocal.setBackgroundResource( R.drawable.menu_local );
//    	}
//    	else
//    	{
//    		 mBtnMenuLocal.setEnabled( false );
//    	     mBtnMenuLocal.setBackgroundResource( R.drawable.bar_local_untouch );
//    	}
//    }
//    
//    public void btnMenuRefreshEnabled( boolean b )
//    {
//    	if( b )
//    	{
//    		 mBtnMenuRefresh.setEnabled( true );
//    	     mBtnMenuRefresh.setBackgroundResource( R.drawable.menu_fresh );
//    	}
//    	else
//    	{
//    		  mBtnMenuRefresh.setEnabled( false );
//    	      mBtnMenuRefresh.setBackgroundResource( R.drawable.bar_refresh_untouch );
//    	}
//    }
//    
//    public void btnMenuMoreEnabled( boolean b )
//    {
//    	if( b )
//    	{
//    		  mBtnMenuMore.setEnabled( true );
//    	      mBtnMenuMore.setBackgroundResource( R.drawable.menu_more ); 
//    	}
//    	else
//    	{
//    		 mBtnMenuMore.setEnabled( false );
//    	     mBtnMenuMore.setBackgroundResource( R.drawable.bar_more_untouch ); 
//    	}
//    }
//    
//    public void btnMenuBackEnabled( boolean b )
//    {
//    	if( b )
//    	{
//    		 mBtnMenuBack.setEnabled(true);
//             mBtnMenuBack.setBackgroundResource(R.drawable.menu_back);
//    	}
//    	else
//    	{
//    		  mBtnMenuBack.setEnabled(false);
//              mBtnMenuBack.setBackgroundResource(R.drawable.bar_back_untouch);
//    	}
//    }
//    
//    public void btnMenuForwardEnabled( boolean b )
//    {
//    	if( mBtnMenuForward != null && b )
//    	{
//    		mBtnMenuForward.setEnabled( true );
//	        mBtnMenuForward.setBackgroundResource( R.drawable.menu_forward_pad );
//    	}
//    	else if( mBtnMenuForward != null )
//    	{
//    		mBtnMenuForward.setEnabled( false );
// 	        mBtnMenuForward.setBackgroundResource( R.drawable.bar_forward_untouch );
//    	}
//    }
//    
//    public void btnSettingVirEnabled( boolean b )
//    {
//    	if( b )
//    	{
//    		mBtnSettingVirbtn.setEnabled( true );
//            mBtnSettingVirbtn.setBackgroundResource( R.drawable.menu_setting_virbtn);
//    	}
//    	else
//    	{
//    		mBtnSettingVirbtn.setEnabled( false );
//            mBtnSettingVirbtn.setBackgroundResource( R.drawable.bar_virbtn_untouch);
//    	}
//    }
//   
//    public void showProgressLoading()
//    {
//        HideMenu();
//        mMenuBrowser.setVisibility( View.VISIBLE );
//        mMenuBrowser.bringToFront();
//        mMenuStatus = UserStatus.MENU_LOADING;
//        mProgressLoading.setVisibility( View.VISIBLE );
//        mProgressLoading.setProgress(5);
//        mProgressLoading.bringToFront();
//        
//        btnMenuHomeEnabled( false );
//        btnMenuLocalEnabled( false );
//        btnMenuRefreshEnabled( false );
//        btnMenuMoreEnabled( false );
//        
//        mBtnMenuBack.setEnabled( true );
//        mBtnMenuBack.setBackgroundResource( R.drawable.menu_stop);
//    }
//    
//    public void stopProgressLoading()
//    {
//        mMenuStatus = UserStatus.MENU_BROWSER;
//        if( mUserStatus == UserStatus.USER_NORMAL )
//        {
//            mMobileView.onDomStop();
//            btnMenuHomeEnabled( ! mMobileView.isCurrentHomePage() );
//    	    btnMenuForwardEnabled( mMobileView.canForward() );
//    	    btnMenuBackEnabled( mMobileView.canBack() ); 
//            btnMenuLocalEnabled( true );
//            btnMenuRefreshEnabled( true );
//            btnMenuMoreEnabled( true );
//        }
//        else if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA ||
//                 mUserStatus == UserStatus.FLASH_TYPE_GAME ||
//                 mUserStatus == UserStatus.FLASH_TYPE_WEBGAME)
//        {
//            mMobileView.exitFlash();
//            closeFlashView();
//            mUserStatus = UserStatus.USER_NORMAL;
//        }
//    }
////	 public void showMediaMenu()
////	 {
////        HideMenu();
////        SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
////        mbShowOldMenu=settings.getBoolean("showOldMenu", true);
////        if(mbShowOldMenu)
////        {
////        	//显示旧菜单
////        	mMobileView.setBottomPos(mScreenHeight-mHeight44);
////        	if(mUserStatus==UserStatus.FLASH_TYPE_GAME)
////        	{
////        		showMenuGameOld();
////        	}
////        	if(mUserStatus==UserStatus.FLASH_TYPE_MEDIA)
////        	{
////        		showMenuMediaOld();
////        	}
////        	if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
////        	{
////        		showMenuWebGameOld();
////        	}
////        }
////        else
////        {
////            showMenuFloatView();
////        }
////	 }
//    
//	/*
//	 * 修改用户状态，并且根据用户状态，修改用户菜单,被c调用
//	*/
//	public void startFlashView( int sta )
//	{
//	    if( getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT )
//	    {
//	        showProgressLoading();
//	    }
//	    switch( sta )
//	    {
//	        case UserStatus.USER_NORMAL:
//	            mUserStatus = UserStatus.USER_NORMAL;
//                break;
//	        case UserStatus.FLASH_TYPE_MEDIA:
//	            mUserStatus = UserStatus.FLASH_TYPE_MEDIA;
//	            break;
//	        case UserStatus.FLASH_TYPE_GAME:
//	            mUserStatus = UserStatus.FLASH_TYPE_GAME;
//                break;
//	        case UserStatus.FLASH_TYPE_WEBGAME:
//	            mUserStatus = UserStatus.FLASH_TYPE_WEBGAME;
//                break;
//            default:
//                mUserStatus = UserStatus.USER_NORMAL;
//	    }
////	    showMediaMenu();//此时在进入Flash时进行菜单显示的判断
//
//        //保持Android手机屏幕长亮
//        PowerManager manager = ((PowerManager)getSystemService(POWER_SERVICE));
//        mWakeLock = manager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK| PowerManager.ON_AFTER_RELEASE, "ATAAW");
//        mWakeLock.acquire();
//
//        SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//        mbShowMouseSign = settings.getBoolean("USER_MOUSE", true );
//        if( !mbShowMouseSign )
//        {
//            mBtnSettingMouse.setBackgroundResource( R.drawable.menu_setting_mouse_off );
//        }
//        if( mMobileView.hasFlashButton() )
//        {
//            mbShowVirBtn = true;
//        }
//        else
//        {
//            mbShowVirBtn = false;
//            btnSettingVirEnabled( false );
//        }
//        //开启重力感应
//	    if( mMobileView.hasFlashACC() )
//	    {
//	        if( !mMobileView.isFlashAccEnable() )
//	            mMobileView.switchAcc();
//	        mSensorManager.registerListener(mSensorListener, mSensor, SensorManager.SENSOR_DELAY_GAME);
//	        mbSensorOpen = true;
//	    }
//	    switchZoomingStatus();
//		return;
//	}
//
//	public void closeFlashView()
//	{
//	    mViewLayout.MenuDirectionGone();
//	    if( mbSensorOpen )
//	    {
//	        mbSensorOpen = false;
//	        mSensorManager.unregisterListener(mSensorListener);
//	    }
//	    mWakeLock.release();
//	   
//	    mUserStatus = UserStatus.USER_NORMAL;
//        ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER );
//        mMobileView.setBottomPos(mScreenHeight);
//        
//        btnMenuHomeEnabled( ! mMobileView.isCurrentHomePage() );
//	    btnMenuForwardEnabled( mMobileView.canForward() );
//        btnMenuLocalEnabled( true );
//        btnMenuBackEnabled( mMobileView.canBack() );
//        btnMenuRefreshEnabled( true );
//        btnMenuMoreEnabled( true );
//        btnSettingVirEnabled( true );
//	}
//    
//    public void startLoadingImg()
//    {
//    	mImgVLoading.setImageResource(R.drawable.app_loading_logo );
//    	mImgVLoading.setBackgroundResource(R.drawable.app_loading_land );
//    	
//    	// 双启动画面时，渐隐渐现第二个画面	// zhengjl 2012.06.05
//    	//fade_animation(mImgVLoading);
//    	bwSDK.startFadeAni(mImgVLoading);
//    	
//        mImgVLoadingProgress = ( ImageView )findViewById( R.id.app_loading_progress );
//        FrameLayout.LayoutParams  param = new FrameLayout.LayoutParams( 
//                FrameLayout.LayoutParams.WRAP_CONTENT,
//                FrameLayout.LayoutParams.WRAP_CONTENT);
//        param.gravity = Gravity.CENTER;
//        param.topMargin = mScreenHeight / 8;
//        mImgVLoadingProgress.setLayoutParams( param );
//        mImgVLoadingProgress.setBackgroundResource( R.anim.img_progress );
//        mImgVLoadingProgress.bringToFront();
//        AnimationDrawable animationDrawable = (AnimationDrawable) mImgVLoadingProgress.getBackground();
//        animationDrawable.setOneShot( false );
//        animationDrawable.run();
//        
//        Message msg = new Message();
//        msg.what = MSG_LOADINGEND;
//        mProcessUseMsg.sendMessageDelayed(msg, 1800 );	// zhengjl 2012.06.05
//        
//    }
//
//    public void closeLoadingImg()  //关闭启动动画
//    {
//    	mbFirstLoad = false;
//        AnimationDrawable animationDrawable = (AnimationDrawable)mImgVLoadingProgress.getBackground();
//        animationDrawable.stop();
//        mImgVLoading.clearAnimation();
//        mImgVLoadingProgress.clearAnimation();
//        
//	   	 //mLayoutMobileMain.addView( (View)mMobileView );
//	     mMobileView.bringToFront();
//	    ((View)mMobileView).setVisibility(View.VISIBLE);
//	     
//        mImgVLoading.setVisibility(View.GONE);
//        mImgVLoadingProgress.setVisibility( View.GONE );
//
//        mLayoutMobileMain.removeView(mImgVLoading);
//        mLayoutMobileMain.removeView( mImgVLoadingProgress );
//        mImgVLoading = null;
//        mImgVLoadingProgress = null;
//        
//        if( mMenuStatus == UserStatus.MENU_BROWSER )
//        	ShowMenu( mMenuBrowser, UserStatus.MENU_BROWSER );
//        else
//        	showProgressLoading();
//        
//        Message msg = new Message();
//        msg.what = MSG_USERPROCESS;
//        mProcessUseMsg.sendMessageDelayed(msg, 20000);
//        return;
//    }
//    
//	public boolean isServiceRunning( ) 
//	{
//		String strServiceName = getPackageName() + ".BwPushService";
//        boolean isRunning = false;
//        ActivityManager activityManager = (ActivityManager)getSystemService(ACTIVITY_SERVICE);
//        List<ActivityManager.RunningServiceInfo> serviceList = activityManager.getRunningServices(Integer.MAX_VALUE);
//        if (!(serviceList.size()>0)) 
//        {
//            return false;
//        }
//      
//        for (int i=0; i<serviceList.size(); i++)
//        {
//            if (serviceList.get(i).service.getClassName().equals(strServiceName ) == true) 
//            {
//            	Log.e("MobileMain_maopao", "break" );
//            	isRunning = true;
//                break;
//            }  
//       }
//        return isRunning;
//    }
//    
//	// test adress http://192.168.1.102:8080/testweb/testmqtt.aspx
//    public void startPushService()
//    {
//  	  	if( !isServiceRunning() )
//  	  	{
//  	  		String deviceID = Secure.getString(this.getContentResolver(), Secure.ANDROID_ID);
//  	  		String strPushCfg = GlobalFun.readPushCfg( this );
//  	  		
//			JSONObject jbcfg = null;
//			try 
//			{
//				if( strPushCfg.length() == 0 )
//					jbcfg = new JSONObject();
//				else
//					jbcfg = new JSONObject(strPushCfg);
//				jbcfg.put("deviceID", deviceID);
//				GlobalFun.writePushCfg(this, jbcfg.toString());
//			} 
//			catch (JSONException e) 
//			{
//				//e.printStackTrace(System.err);
//			}
//  	  		mMobileView.setDeviceId( deviceID );
//	    	BwPushService.actionStart(getApplicationContext());	
//  	  	}
//  	  	return;
//    }
//
//    boolean createShortCut()
//    {
//        Intent shortcutIntent=new Intent("com.android.launcher.action.INSTALL_SHORTCUT");
//        Parcelable icon=Intent.ShortcutIconResource.fromContext(this, R.drawable.byone_logo); //获取快捷键的图标
//        Intent myIntent=new Intent(this, MobileMain_maopao.class);
//        shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_NAME, "百玩");//快捷方式的标题
//        shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, icon);//快捷方式的图标
//        shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, myIntent);//快捷方式的动作
//        //shortcutIntent(Intent.EXTRA_SHORTCUT_DUPLICATE, false);
//        sendBroadcast(shortcutIntent);//发送广播
//        return true;
//    }
//    
//    private void getOrientationHW()  //根据方向设置高宽
//    {
////        DisplayMetrics metric = new DisplayMetrics();
////        display.getMetrics(metric);
////        int densityDpi = metric.densityDpi;  // 屏幕密度DPI（120 / 160 / 240）
//    	
//        Display display = getWindowManager().getDefaultDisplay();
//        mScreenWidth = display.getWidth();
//        mScreenHeight = display.getHeight();
//        
//        if( getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT ) //为竖屏
//        {
//            screenModeVal( 0 );
//            if( mScreenWidth > mScreenHeight )
//            {
//                int itmp = mScreenHeight;
//                mScreenHeight = mScreenWidth;
//                mScreenWidth = itmp;
//            }
//        }
//        else if( getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE ) //为横屏
//        {
//            screenModeVal( 1 );
//            if( mScreenWidth < mScreenHeight )
//            {
//                int itmp = mScreenHeight;
//                mScreenHeight = mScreenWidth;
//                mScreenWidth = itmp;
//            }
//        }
//        if( android.os.Build.MODEL.equals("Kindle Fire"))
//        {
//        	mScreenHeight -= 20;
//        }
//        GlobalFun.BWScreenWidth=mScreenWidth;
//        GlobalFun.BWScreenHeight=mScreenHeight;
//    }
//   
//   public void AnalyzeJson()
//   {
//	   String strJson = "{\"aps\":{\"alert\":\"小柔示爱，您的游戏账户有100金币，点击领取\",\"badge\":1,\"sound\":\"nokia.caf\"},\"bw\":{\"url\":\"http://m.jqbar.com/app/SubmitPlay.aspx?parray=19_1_15_7&pid=599\",\"urltype\":1}}";
//	   JSONObject jb =null;
//	   try 
//	   {
//		   jb = new JSONObject(strJson);
//		   JSONObject jbAps = jb.getJSONObject("aps");
//		   String strAlert = jbAps.getString("alert");
//		   JSONObject jbBw = jb.getJSONObject("bw");
//		   String strUrl = jbBw.getString("url");
//		   int iurltype = jbBw.getInt("urltype");
//		   Log.e("MobileMain_maopao", strAlert );
////		   int jlength = jsons.length();
////		   for( int i = 0; i < jlength; i++ )
////		   {
////			   JSONObject tmpJson = new JSONObject( jsons.get(i));
////			   String strAlert = tmpJson.getString("alert");
////			   Log.e("MobileMain_maopao", strAlert );
////		   }
//	   } 
//	   catch (JSONException e)
//	   {
//           e.printStackTrace(System.err);
//       }
//       String strTmp = jb.toString();
//       Log.e("MobileMain_maopao", strTmp );
//       return;
//   }
//   
//   /** 判断是否模拟器。如果返回TRUE，则当前是模拟器
//    * @param context
//    * @return
//    */
//   public static boolean isEmulator(Context context)
//   {
//	   TelephonyManager tm = (TelephonyManager) context.getSystemService(Context.TELEPHONY_SERVICE);
//	   String imei = tm.getDeviceId();
//	   if (imei == null || imei.equals("000000000000000"))
//	   {
//	        return true;
//	   }
//	   return false;
//   }
//
//   // zhengjl 2012.06.05
//   void fade_animation(View view)
//   {
//		AnimationSet animationSet = new AnimationSet(true);
//		AlphaAnimation alphaAnimation = new AlphaAnimation(0.0f, 1.0f); 
//		alphaAnimation.setDuration(500); 
//		AlphaAnimation alphaAnimation1 = new AlphaAnimation(1.0f, 0.0f); 
//		alphaAnimation1.setDuration(500); 
//		alphaAnimation1.setStartOffset(2500);
//		animationSet.addAnimation(alphaAnimation); 
//		animationSet.addAnimation(alphaAnimation1);
//		view.startAnimation(animationSet); 
//   }
//   
//	protected void onCreate(Bundle savedInstanceState)
//	{
//		Log.e("MobileMain_maopao", "onCreate");
//		super.onCreate(savedInstanceState);
//		requestWindowFeature(Window.FEATURE_NO_TITLE);
//        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
//                WindowManager.LayoutParams.FLAG_FULLSCREEN);
//
////        talkList=new ArrayList<String>();
//        mViewLayout = new MulTouchPad( this );
//		mViewLayout.setOrientation( this );
//		//createShortCut();
//		
//		registerScreenActionReceiver();	// zhengjl 2011.09.09
//		
//		mViewLayout.setContentView();
//		
//		getOrientationHW();
//		initCPP();
//
//		// 获得 SDK 实例
//		FreeSkySdk sdk = FreeSkySdk.getInstance();
//		// 激活应用必须的接口,应用一启动就需要调用此接口
//		// 应用文件上传到冒泡开放平台后,需立即下载并执行
//		// 通过此接口上行的数据,平台会激活此应用,然后才允许提交发布
//		sdk.init(getApplicationContext());
//		// 检查是否有更新,若应用有更新,则会自动在通知栏提醒用户
//		// 请自行决定更新检查的策略,一般无需每次启动都做检查
//		sdk.doCheckUpdate();
//
//		Intent intent = getIntent();
//		Bundle  bl= intent.getExtras();
//		if( bl != null )
//		{
//			mUAValue = bl.getString("UA_CODE");
//			mChannelValue = bl.getString("CHANNEL_ID");
//			strUrl = bl.getString("BW_URL");
//		}
//		else
//		{
//			mChannelValue = GlobalFun.GFunGetCID( this );
//			strUrl = "http://m.jqbar.com/product/";
//		}
//		if( android.os.Build.VERSION.SDK_INT < 14 )
//			mMobileView = new MobileView(this, mScreenWidth, mScreenHeight );
//		else
//			mMobileView = new MobileViewGLES(this, mScreenWidth, mScreenHeight );	
//        mAssetManager = getAssets();
//        mMobileView.connectUrl( strUrl );
//        mMenuStatus = UserStatus.MENU_NONE;
//        mUserStatus = UserStatus.USER_NORMAL;
//        mViewStatus = UserStatus.VIEW_NORMAL;
//        mViewPopStatus = UserStatus.VIEW_POP_NONE;
//        mUserActionStatus = UserStatus.USER_ACTION_NONE;
//        mbFirstLoad = true;
//        
//        mLayoutMobileMain = (FrameLayout) findViewById( R.id.layoutMobileMain);
//		mViewLayout.CreateMenuEvent();
//		mLayoutMobileMain.addView( (View)mMobileView );
//		mImgVLoading = (ImageView) findViewById(R.id.app_loading);
////		mImgVLoading.setImageResource(R.drawable.app_loading_logo );
////		mImgVLoading.setBackgroundResource(R.drawable.app_loading_land );
//		//mImgVLoading.setImageResource(R.drawable.app_loading_logo_other );
//		//mImgVLoading.setBackgroundResource( R.drawable.app_loading_land_other );
//		mImgVLoading.bringToFront();
//
//        Message msg= new Message();
//        msg.what = MSG_LOADINGBEGIN;
//        HideMenu();
//    	bwSDK = BwSDK.DefaultSDK();
//		bwSDK.createSDKAni();
//		((View)mMobileView).setVisibility(View.GONE);
//		int delayTime = bwSDK.setStartAnimation(mImgVLoading);
//		mProcessUseMsg.sendMessageDelayed( msg, delayTime );
////    	// 双启动画面时，渐隐渐现第一个画面	// zhengjl 2012.06.05
////		delayTime = 3000;
////		((View)mMobileView).setVisibility(View.GONE);	// zhengjl 2012.06.05
////		mImgVLoading.setImageResource(R.drawable.app_loading_logo_other );
////		mImgVLoading.setBackgroundResource(R.drawable.app_loading_land_other );
////    	fade_animation(mImgVLoading);
//        
////        mProcessUseMsg.sendMessageDelayed( msg, 500 );	// zhengjl 2012.06.05
////        HideMenu();
//        
//		mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
//		mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
//		mSensorListener = new SensorEventListener()
//		{
//		    public void onAccuracyChanged(Sensor sensor, int accuracy)
//		    {
//		    }
//		    public void onSensorChanged(SensorEvent event)
//		    {
//    		    float x = event.values[SensorManager.DATA_X];
//    		    float y = event.values[SensorManager.DATA_Y];
//    		    float z = event.values[SensorManager.DATA_Z];
//    		   if( screenModeVal() == 1 )
//                {
//    		        //Log.e("Test-------------->", "x:" + y + ", y:" + -x + ", z:" + z );
//                    mMobileView.senseAcc( (int)(y * 1000), -(int)(x * 1000), (int)(z * 1000) );
//                }
//    		   else if( screenModeVal() == 0 )
//    		    {
//    		        mMobileView.senseAcc( -(int)(x * 1000), -(int)(y * 1000), (int)(z * 1000) );
//    		    }
//		    }
//		};
//		return;
//	}
//	
//	private static int mScreenMode = 0; //屏幕显示方向，为1则为横排，为0则为竖排
//	
//	public static void screenModeVal( int mode )
//	{
//	    mScreenMode = mode;  
//	}
//	public static int screenModeVal()
//	{
//	    return mScreenMode;
//	}
//	
//	public void switchViewHW( int mode )//为1则为横排，为0则为竖排
//	{
//	    if( mode == 1 )
//	    {
//	    	if( mScreenHeight > mScreenWidth )
//	    	{
//	    		int itmp = mScreenHeight;
//	    		mScreenHeight = mScreenWidth;
//	    		mScreenWidth = itmp;
//	    	}
//	    }
//	    else if( mode == 0 )
//	    {
//	    	if( mScreenHeight < mScreenWidth )
//	    	{
//	    		int itmp = mScreenHeight;
//	    		mScreenHeight = mScreenWidth;
//	    		mScreenWidth = itmp;
//	    	}   
//            
//	    }
//	    mMobileView.setScreenWHJava( mScreenWidth, mScreenHeight );
//	}
//	/*
//	 * 设置屏幕显示方向
//	 * mode ： 为1则为横排，为0则为竖排
//	 */
//	public void SetMode( int mode )
//	{
//	    Log.e("--------->", "setMode");
//	    mViewLayout.setMode(mode);
//	    if( mUserStatus == UserStatus.FLASH_TYPE_GAME || 
//	        mUserStatus == UserStatus.FLASH_TYPE_MEDIA ||
//	        mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
//	    {
//	        HideMenu();
//            showMenuFloatView();
//	    }
//		return;
//	}
//
//	public void ShowMenuPopMore() // 是否显示更多的弹出菜单
//	{
//	    if ( mMenuStatus == UserStatus.MENU_BROWSER )
//        {
//            mMenuBrowserPop.setVisibility(View.VISIBLE);
//            mMenuBrowserPop.startAnimation(mAniEnter);
//            mMenuStatus = UserStatus.MENU_BROWSER_POP;
//        }
//	}
//
//	public void HideMenuPopMore()
//	{
//	    if( mMenuStatus == UserStatus.MENU_BROWSER_POP )
//	    {
//	        mMenuBrowserPop.startAnimation(mAniExit);
//            mMenuBrowserPop.setVisibility(View.GONE);
//            mMenuStatus = UserStatus.MENU_BROWSER;
//	    }
//	}
//
//	public void HideMenu( )
//	{
//        mMenuBrowser.setVisibility(View.GONE);
//        mMenuFloatView.setVisibility( View.GONE );
//        mMenuFloatMedia.setVisibility( View.GONE );
//        mMenuFloatGame.setVisibility( View.GONE );
//        mMenuFloatWebGame.setVisibility( View.GONE );
//        mProgressLoading.setVisibility( View.GONE );
//        mMenuStatus = UserStatus.MENU_NONE;
//	}
//	public void ShowMenu( View vMenu, int menuStatus )
//	{
//	    HideMenu();
//	    if( menuStatus != UserStatus.MENU_NONE )
//	    {
//	        mMobileView.bringToFront();
//	        mMenuStatus = menuStatus;
//	        vMenu.setVisibility( View.VISIBLE );
//	        vMenu.bringToFront();
//	    }
//	}
//
//	public void menuExitFlash()
//	{
//	    if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA || mUserStatus == UserStatus.FLASH_TYPE_GAME )
//        {
//            mMobileView.flashPause();
//        }
//        else if( mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
//        {
//            mMobileView.setSilence( true );
//        }
//        Dialog dlg = new AlertDialog.Builder(MobileMain_maopao.this)
//        .setIcon(R.drawable.logo_36)
//        .setTitle("确定退出？")
//        .setPositiveButton(R.string.positive_button, new DialogInterface.OnClickListener()
//        {
//            public void onClick(DialogInterface dialog, int whichButton)
//            {
//                mMobileView.exitFlash();
//                closeFlashView();
//            }
//        }).setNegativeButton(R.string.negative_button, new DialogInterface.OnClickListener(){
//            public void onClick(DialogInterface dialog, int which) {
//                //mMobileView.flashResume();
//                if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA || mUserStatus == UserStatus.FLASH_TYPE_GAME )
//                {
//                    mMobileView.flashResume();
//                }
//                else if( mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
//                {
//                    mMobileView.setSilence( false );
//                }
//            }
//        })
//        .create();
//        dlg.setOnKeyListener( new DialogInterface.OnKeyListener()
//        {
//            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event)
//            {
//                if ( keyCode == KeyEvent.KEYCODE_BACK )
//                {
//                    //mMobileView.flashResume();
//                    if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA || mUserStatus == UserStatus.FLASH_TYPE_GAME )
//                    {
//                        mMobileView.flashResume();
//                    }
//                    else if( mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
//                    {
//                        mMobileView.setSilence( false );
//                    }
//                }
//                return false;
//            }
//        });
//        dlg.show();
//	}
//	
//	public void MenuDirectionVisible()
//	{
//	    mMenuDirection.setVisibility( View.VISIBLE);
//        mMenuDirection.bringToFront();
//	}
//	
//	public void MenuDirectionGone()
//	{
//	    mMenuDirection.setVisibility( View.GONE);
//	}
//	
//	private void switchZoomingStatus()
//	{
//	    if( (mMobileView.getZoomPecent() > 5 )  )
//        {
//            mViewStatus = UserStatus.VIEW_LARGE;
//            mViewLayout.MenuDirectionVisible();
//            mFrameZoom.setBackgroundResource( R.drawable.menu_zoom_min );
////            switch(mUserStatus)
////            {
////            case UserStatus.FLASH_TYPE_GAME:
////            	break;
////            case UserStatus.FLASH_TYPE_MEDIA:
////            	break;
////            case UserStatus.FLASH_TYPE_WEBGAME:
////          
////             
////            	break;
////            default:
////            		break;
////            }
//            mMenuMediaZoomOld.setBackgroundResource(R.drawable.menu_zoom_min_old);
//	        mBtnGameZoomOld.setBackgroundResource( R.drawable.menu_zoom_min_old);
//	        mBtnWebGameZoomOld.setBackgroundResource(R.drawable.menu_zoom_min_old);
//        }
//        else
//        {
//            mViewStatus = UserStatus.VIEW_NORMAL;
//            mViewLayout.MenuDirectionGone();
//            mFrameZoom.setBackgroundResource( R.drawable.menu_zoom_max );
////            switch(mUserStatus)
////            {
////            case UserStatus.FLASH_TYPE_GAME:
////            	break;
////            case UserStatus.FLASH_TYPE_MEDIA:
////            	break;
////            case UserStatus.FLASH_TYPE_WEBGAME:
////            	break;
////            default:
////            		break;
////            }
//            mMenuMediaZoomOld.setBackgroundResource(R.drawable.menu_zom_max_old);
//            mBtnGameZoomOld.setBackgroundResource( R.drawable.menu_zom_max_old);
//            mBtnWebGameZoomOld.setBackgroundResource(R.drawable.menu_zom_max_old);
//        }
//	}
//
//	private void switchZooming( )
//	{
//	    if( (mMobileView.getZoomPecent() <= 0) && mViewStatus == UserStatus.VIEW_NORMAL )
//        {
//           //mMobileView.flashLockZoomInOut();
//           //mMobileView.flashZoomInOut( 100 );
//           //mMobileView.flashUnLockZoomInOut();
//	    	 mViewStatus = UserStatus.VIEW_LARGE;
//	         mMobileView.flashPointZoom();
//	         mViewLayout.MenuDirectionVisible();
//	         mMenuMediaZoomOld.setBackgroundResource(R.drawable.menu_zoom_min_old);
//	         mBtnGameZoomOld.setBackgroundResource( R.drawable.menu_zoom_min_old);
//	         mBtnWebGameZoomOld.setBackgroundResource(R.drawable.menu_zoom_min_old);
//	         mFrameZoom.setBackgroundResource( R.drawable.menu_zoom_min );
//           
//        }
//        else
//        {
//            //mMobileView.flashLockZoomInOut();
//            //mMobileView.flashZoomInOut( 0 );
//            //mMobileView.flashUnLockZoomInOut();
//        	  mMobileView.flashPointZoom();
//              mViewStatus = UserStatus.VIEW_NORMAL;
//              mViewLayout.MenuDirectionGone();
//              mMenuMediaZoomOld.setBackgroundResource(R.drawable.menu_zom_max_old);
//              mBtnGameZoomOld.setBackgroundResource( R.drawable.menu_zom_max_old);
//              mBtnWebGameZoomOld.setBackgroundResource(R.drawable.menu_zom_max_old);
//              mFrameZoom.setBackgroundResource( R.drawable.menu_zoom_max );
//          
//        }
//	}
//	
//	/*
//	 * 创建用户菜单响应
//	 */
//    public void CreateMenuEvent()
//    {
//        mViewLayout.CreateMenuEvent();
//
//	 }  // end of CreateMenuEvent()
//
//    private final static int TIME_GAP = 4;
//
//    public void displayTips(String str)
//    {
//    	   Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
//    }
//    private int mMenuGameOldHeight;
//    private boolean mbMenuGameOldMove;
//    private int mMenuGameOldMoveY;
//
//    private boolean mbMenuWebGameOldMove;
//    private int mMenuWebGameOldMoveY;
//
//    private boolean mbMenuMediaOldMove;
//    private int mMenuMediaOldMoveY;
//    private int mBtnFloatImgWidth = 56;
//    private int mBtnFloatImgHeight = 56;
//    private int mMoveX = 0;
//    private int mMoveY = 0;
//    private boolean mbFloatViewMove = false;
//	private int mIFloatViewCount = -1;
//    public int miImgProgressShow = 0;
//
//	public void showImgProgress()
//	{
//		miImgProgressShow = 36;
//		SharedPreferences settings = getSharedPreferences( GlobalFun.BW_CONFIGURE_FILE, 0);
//		boolean showoldmenu = settings.getBoolean("showOldMenu", true);
//		
//		if (showoldmenu && GlobalFun.showMenu)
//		{
//			if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) 
//			{
//				mImgProgressOld.setVisibility(View.VISIBLE);
//				AnimationDrawable animationDrawable = (AnimationDrawable) mImgProgressOld.getBackground();
//				animationDrawable.setOneShot(false);
//				if (!animationDrawable.isRunning())
//				{
//					animationDrawable.run();
//				}
//			}
//			else if (mUserStatus == UserStatus.FLASH_TYPE_GAME) 
//			{
//				mImgProgressGameOld.setVisibility(View.VISIBLE);
//				AnimationDrawable animationGameDrawable = (AnimationDrawable) mImgProgressGameOld.getBackground();
//				animationGameDrawable.setOneShot(false);
//				if (!animationGameDrawable.isRunning())
//				{
//					animationGameDrawable.run();
//				}
//			}
//			else if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA)
//			{
//				mImgProgressMediaOld.setVisibility(View.VISIBLE);
//				AnimationDrawable animationMediaDrawable = (AnimationDrawable) mImgProgressMediaOld.getBackground();
//				animationMediaDrawable.setOneShot(false);
//				if (!animationMediaDrawable.isRunning()) 
//				{
//					animationMediaDrawable.run();
//				}
//			}
//		} 
//		else
//		{
//			mImgProgress.setVisibility(View.VISIBLE);
//			AnimationDrawable animationDrawable = (AnimationDrawable) mImgProgress.getBackground();
//			animationDrawable.setOneShot(false);
//			if (!animationDrawable.isRunning()) 
//			{
//				animationDrawable.run();
//			}
//		}
//
//	}
//    
//    public void setMenuFloatViewPos( int iposx, int iposy)
//    {
//    	int iposright = mScreenWidth - 2 * mBtnFloatImgWidth;
//        iposx = iposx < 0 ? 0 : iposx;
//        iposx = iposx > iposright ? iposright: iposx;
//        
//        int iposbottom = mScreenHeight - mBtnFloatImgHeight;
//        iposy = iposy < 0 ? 0 : iposy;
//        iposy = iposy > iposbottom ? iposbottom: iposy;
//        
//        if(iposy==(mScreenHeight - mBtnFloatImgHeight))
//        {
//        	mbExchangeMenu=true;
//        }
//        if(!mbExchangeMenu)
//        {
//	        LayoutParams btnMoreParams = new LayoutParams(LayoutParams.WRAP_CONTENT,
//	                LayoutParams.WRAP_CONTENT,  iposx ,  iposy );
//	        mLinearView.setLayoutParams(btnMoreParams);
//        }
//    }
//    
////    public void showMenuFloatView()
////    {
////        SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
////        int iposx = settings.getInt("USER_POS_X", 20 );
////        int iposy = settings.getInt("USER_POS_Y", 20 );
////        setMenuFloatViewPos( iposx, iposy );
////        mMenuStatus = UserStatus.MENU_FLOAT_VIEW;
////        
//////        AnimationSet animationSet = new AnimationSet(true);
//////        AlphaAnimation alphaAni = new AlphaAnimation( 1.0f, 0.4f );
//////        alphaAni.setDuration( 400 ); 
//////        alphaAni.setInterpolator( new LinearInterpolator() );
//////        alphaAni.setRepeatCount( 3 );
//////        alphaAni.setRepeatMode( Animation.REVERSE );
//////        animationSet.addAnimation( alphaAni );
//////        mMenuFloatView.setAnimation(animationSet);
//////        animationSet.start();
////        if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
////        {
////        timeWebGameFloat.setVisibility(View.VISIBLE);
////        timeWebGameFloat.bringToFront();
////        }
////        else
////        {
////        	timeWebGameFloat.setVisibility(View.GONE);
////        }
////        mMenuFloatView.setVisibility(View.VISIBLE );
////        mMenuFloatView.bringToFront();
////        //mMobileView.flashResume();
////    }
////   public void showMenuGameOld()
////   {
////	   HideMenu();
////	   mMenuStatus=UserStatus.MENU_GAME_OLD;
////	   GlobalFun.showMenu=true;
////	   mMenuGameOld.setVisibility(View.VISIBLE);
////	   mMenuGameOld.bringToFront();
////
////   }
////   public void showMenuWebGameOld()
////   {
////	   HideMenu();
////	   mMenuStatus=UserStatus.MENU_WEBGAME_OLD;
////	   GlobalFun.showMenu=true;   	
////	   
//////	   String str4 = getString( R.dimen.bwpx4 );
//////   	   str4 = str4.substring(0, str4.indexOf(".") );
//////   	   int len4 = Integer.parseInt( str4 );
//////   	   int menuWebGameWidth=mMenuWebGameOld.getWidth();
//////   	   int switchBtnWidth=mBtnWebGameSwitchOld.getWidth();
//////   	   int timeWidth=timeWebGame.getWidth();
//////   	   int progressWidth=mImgProgressOld.getWidth();
//////   	   int local_x=(menuWebGameWidth-switchBtnWidth*6-timeWidth-progressWidth-len4)/2;
//////   	   RelativeLayout.LayoutParams param=(RelativeLayout.LayoutParams)mImgProgressOld.getLayoutParams();
//////   	   param.setMargins(local_x, 0, 0, 0);
//////   	   mImgProgressOld.setLayoutParams(param);
//////   	   mImgProgressOld.invalidate();
////   	   
////	   mMenuWebGameOld.setVisibility(View.VISIBLE);
////	   mMenuWebGameOld.bringToFront();
////   }
////   public void showMenuMediaOld()
////   {
////	   HideMenu();
////	   mMenuStatus=UserStatus.MENU_MEDIA_OLD;
////	   GlobalFun.showMenu=true;
////	   mMenuMediaOld.setVisibility(View.VISIBLE);
////	   mMenuMediaOld.bringToFront();
////   }
//    
//
//    private boolean mbShowMouseSign = true;
//
//    
//    public void hideSettingFromBrowser()
//    {
//        AnimationParam param = new AnimationParam();
//        param.mView = mMenuUserSetting;
//        param.mDuration = 200;
//        param.mTranFromX = 0;
//        param.mTranToX = 120;
//        param.mTranFromY = 0;
//        param.mTranToY = 120;
//        param.mScaleFromX = 1;
//        param.mScaleToX = 0;
//        param.mScaleFromY = 1;
//        param.mScaleToY = 0;
//        MenuAnimation(param);
//
//        mMenuUserSetting.setVisibility(View.GONE);
//    }
//    
//    public void showSettingFormBrowser()
//    {
//        AnimationParam param = new AnimationParam();
//        param.mView = mMenuUserSetting;
//        param.mDuration = 200;
//        param.mTranFromX = 120;
//        param.mTranToX = 0;
//        param.mTranFromY = 120;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation(param);
//
//        mMenuUserSetting.setVisibility(View.VISIBLE);
//        mMenuUserSetting.bringToFront();
//    }
//    
//    public void showSettingFromFlash()
//    {
//        AnimationParam param = new AnimationParam();
//        param.mView = mBtnSettingMouse;
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = 120;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnSettingVirbtn;
//        param.mDuration = 200;
//        param.mTranFromX = 120;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnSettingBack;
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnSettingSwitch;
//        param.mDuration = 200;
//        param.mTranFromX = -40;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
////        param.mView = mBtnSettingEliminate;
////        param.mDuration = 200;
////        param.mTranFromX = 40;
////        param.mTranToX = 0;
////        param.mTranFromY = -40;
////        param.mTranToY = 0;
////        param.mScaleFromX = 0;
////        param.mScaleToX = 1;
////        param.mScaleFromY = 0;
////        param.mScaleToY = 1;
////        MenuAnimation( param );
//
////          AnimationParam param = new AnimationParam();
////          param.mView = mMenuUserSetting;
////          param.mDuration = 200;
////          param.mTranFromX = 120;
////          param.mTranToX = 0;
////          param.mTranFromY = 120;
////          param.mTranToY = 0;
////          param.mScaleFromX = 0;
////          param.mScaleToX = 1;
////          param.mScaleFromY = 0;
////          param.mScaleToY = 1;
////          MenuAnimation( param );
//        
//        mMenuUserSetting.setVisibility( View.VISIBLE );
//        mMenuUserSetting.bringToFront();
//    }
//    
//    public void showMediaFromSetting()
//    {
//        AnimationParam param = new AnimationParam();
//        param.mView = mBtnMediaSave;
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = 120;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnMediaSetting;
//        param.mDuration = 200;
//        param.mTranFromX = 120;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnMediaBack;
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnMediaClose;
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = -40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
////        param.mView = mBtnMediaFloatSwitch;
////        param.mDuration = 200;
////        param.mTranFromX = 120;
////        param.mTranToX = 0;
////        param.mTranFromY = -40;
////        param.mTranToY = 0;
////        param.mScaleFromX = 0;
////        param.mScaleToX = 1;
////        param.mScaleFromY = 0;
////        param.mScaleToY = 1;
////        MenuAnimation( param );
//        
//        mMenuFloatMedia.setVisibility( View.VISIBLE );
//        mMenuFloatMedia.bringToFront();
//    }
//    
//    public void showGameFromSetting()
//    {
//        AnimationParam param = new AnimationParam();
//        param.mView = mBtnGameSave;
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = 120;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnGameSetting;
//        param.mDuration = 200;
//        param.mTranFromX = 120;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnGameFloatBack;
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnGameFloatHelp;
//        param.mDuration = 200;
//        param.mTranFromX = -80;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnGameFloatClose;
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = -40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
////        param.mView = mBtnGameFloatSwitch;
////        param.mDuration = 200;
////        param.mTranFromX = 120;
////        param.mTranToX = 0;
////        param.mTranFromY = -40;
////        param.mTranToY = 0;
////        param.mScaleFromX = 0;
////        param.mScaleToX = 1;
////        param.mScaleFromY = 0;
////        param.mScaleToY = 1;
////        MenuAnimation( param );
//        
//        mMenuFloatGame.setVisibility( View.VISIBLE );
//        mMenuFloatGame.bringToFront();
//    }
//    
//    public void showWebGameFromSetting()
//    {
//        AnimationParam param = new AnimationParam();
//        param.mView = mBntWebGameRecharge;  //上
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = 120;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnWebGameSetting;  //左
//        param.mDuration = 200;
//        param.mTranFromX = 120;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnWeGameBack; // 中
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnWebGameFresh;  //右
//        param.mDuration = 200;
//        param.mTranFromX = -80;
//        param.mTranToX = 0;
//        param.mTranFromY = 40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
//        param.mView = mBtnWebGameClose;  //下
//        param.mDuration = 200;
//        param.mTranFromX = 40;
//        param.mTranToX = 0;
//        param.mTranFromY = -40;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0;
//        param.mScaleToY = 1;
//        MenuAnimation( param );
//        
////        param.mView = mBtnWebGameFloatSwitch;
////        param.mDuration = 200;
////        param.mTranFromX = 120;
////        param.mTranToX = 0;
////        param.mTranFromY = -40;
////        param.mTranToY = 0;
////        param.mScaleFromX = 0;
////        param.mScaleToX = 1;
////        param.mScaleFromY = 0;
////        param.mScaleToY = 1;
////        MenuAnimation( param );
//        
//        mMenuFloatWebGame.setVisibility( View.VISIBLE );
//        mMenuFloatWebGame.bringToFront();
//    }
//    
//    private void changeUserMenu()  //显示菜单状态 
//    {
//        //mMobileView.flashPause();
//        switch( mUserStatus )
//        {
//            case UserStatus.FLASH_TYPE_MEDIA:
//                HideMenu();
//                //mMobileView.flashPause();
//                mMenuStatus = UserStatus.MENU_MEDIA;
//                showMenuAnimation( mMenuFloatMedia );
//               
//                break;
//            case UserStatus.FLASH_TYPE_GAME:
//                HideMenu();
//                //mMobileView.flashPause();
//                mMenuStatus = UserStatus.MENU_GAME;
//                showMenuAnimation( mMenuFloatGame );
//                break;
//            case UserStatus.FLASH_TYPE_WEBGAME:
////                if( mMobileView.canPayWindow())
////                {
////                    mBntWebGameRecharge.setEnabled( true );
////                    mBntWebGameRecharge.setBackgroundResource( R.drawable.menu_recharge );
////                }
////                else
////                {
////                    mBntWebGameRecharge.setEnabled(false);
////                    mBntWebGameRecharge.setBackgroundResource(R.drawable.bar_recharge_untouch);
////                }
//                HideMenu();
//                //mMobileView.setSilence( true );
//                mMenuStatus = UserStatus.MENU_WEBGAME;
//                showMenuAnimation( mMenuFloatWebGame );
//                //ShowMenu(mMenuFloatWebGame, UserStatus.MENU_WEBGAME );
//                break;
//            default:
//                mUserStatus = UserStatus.USER_NORMAL;
//                ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER  );
//        }
//        return;
//    }
//    
//    public void backFromUserSetting()
//    {
//       SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
//       mbShowOldMenu=settings.getBoolean("showOldMenu", true);
//        switch( mUserStatus )
//        {
//            case UserStatus.FLASH_TYPE_MEDIA:
////              
//                if(mbShowOldMenu)
//                {
//                	mMobileView.setBottomPos(mScreenHeight-mHeight44);
//                	showMenuMediaOld();	
//                }
//                else
//                {
//                	mMenuStatus = UserStatus.MENU_MEDIA;
//                	showMediaFromSetting();
//                }
//                break;
//            case UserStatus.FLASH_TYPE_GAME:
//               
//                if(mbShowOldMenu)
//                {
//                	mMobileView.setBottomPos(mScreenHeight-mHeight44);
//                	showMenuGameOld();	
//                }
//                else
//                {
//                	mMenuStatus = UserStatus.MENU_GAME;
//                	showGameFromSetting();
//                }
//                break;
//            case UserStatus.FLASH_TYPE_WEBGAME:
//                if(mbShowOldMenu)
//                {
//                	mMobileView.setBottomPos(mScreenHeight-mHeight44);
//                	showMenuWebGameOld();
//                }
//                else
//                {
//                	mMenuStatus = UserStatus.MENU_WEBGAME;
//                	showWebGameFromSetting();
//                }
//                break;
//            default:
//                mUserStatus = UserStatus.USER_NORMAL;
//                ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER  );
//        }
//        return;
//    }
//    
//    public void MenuAnimation(AnimationParam param )
//    {
//        AnimationSet animationSet = new AnimationSet(true);
//        
//        TranslateAnimation translateAnimation = new TranslateAnimation( 
//                param.mTranFromX, param.mTranToX, param.mTranFromY, param.mTranToY ); 
//        translateAnimation.setDuration( param.mDuration );      
//
//        ScaleAnimation scaleAnimation = new ScaleAnimation( 
//                param.mScaleFromX, param.mScaleToX, param.mScaleFromY, param.mScaleToY );
//        scaleAnimation.setDuration( param.mDuration ); 
//  
//        animationSet.addAnimation(scaleAnimation);  
//        animationSet.addAnimation(translateAnimation);  
//    
//        param.mView.setAnimation(animationSet);
//        animationSet.start();
//        param.mView.setVisibility( View.VISIBLE );
//        param.mView.bringToFront();
//    }
//    
//    public void showMenuAnimation(View view)
//    {
//    	String str240 = getString( R.dimen.bwpx240 );
//    	str240 = str240.substring(0, str240.indexOf(".") );
//    	int len240 = Integer.parseInt( str240 );
//        SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
//        int iposx = settings.getInt("USER_POS_X", 20 );
//        int iposy = settings.getInt("USER_POS_Y", 20 ); 
//        
//        int iposright = mScreenWidth - 2* mBtnFloatImgWidth;
//        iposx = iposx < 0 ? 0 : iposx;
//        iposx = iposx > iposright ? iposright: iposx;
//        
//        int iposbottom = mScreenHeight - mBtnFloatImgHeight;
//        iposy = iposy < 0 ? 0 : iposy;
//        iposy = iposy > iposbottom ? iposbottom : iposy;
//
//        AnimationParam param = new AnimationParam();
//        param.mView = view;
//        param.mDuration = 200;
//        param.mTranFromX = iposx - mScreenWidth / 2 + len240 / 2;
//        param.mTranToX = 0;
//        param.mTranFromY = iposy - mScreenHeight / 2 + len240 / 2;
//        param.mTranToY = 0;
//        param.mScaleFromX = 0.367f;
//        param.mScaleToX = 1;
//        param.mScaleFromY = 0.225f;
//        param.mScaleToY = 1;
//        
//        MenuAnimation( param );
//        return;
//        
//    }
//    
////    public void showMenuAnimation(View view)
////    {
////        SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
////        int startx = settings.getInt("USER_POS_X", 20 );
////        int starty = settings.getInt("USER_POS_Y", 20 ) - 14; 
////        
////        AnimationSet animationSet = new AnimationSet(true);
////        
////        TranslateAnimation translateAnimation = new TranslateAnimation( 
////                startx - mScreenWidth / 2 + 95, 0, starty - mScreenHeight / 2 + 81, 0 ); 
////        //TranslateAnimation translateAnimation = new TranslateAnimation(  startx - endx, 0, starty - endy, 0 ); 
////        translateAnimation.setDuration(200);      
////
////        ScaleAnimation scaleAnimation = new ScaleAnimation( 0.367f, 1, 0.225f, 1 );
////        scaleAnimation.setDuration(200); 
//////          scaleAnimation.setStartOffset(300);
////        
////        animationSet.addAnimation(scaleAnimation);  
////        animationSet.addAnimation(translateAnimation);  
//////          animationSet.setFillAfter(true);      
//////          animationSet.setFillBefore(false);      
////        
////        view.setAnimation(animationSet);
////        animationSet.start();
////        
////    }
//    
//    public void hideMenuAnimation(View view)
//    {
//    	String str240 = getString( R.dimen.bwpx240 );
//    	str240 = str240.substring(0, str240.indexOf(".") );
//    	int len240 = Integer.parseInt( str240 );
//        view.setVisibility( View.GONE );
//        SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
//        int iposx = settings.getInt("USER_POS_X", 20 );
//        int iposy = settings.getInt("USER_POS_Y", 20 );
//        
//        int iposright = mScreenWidth - 2* mBtnFloatImgWidth;
//        iposx = iposx < 0 ? 0 : iposx;
//        iposx = iposx > iposright ? iposright: iposx;
//        
//        int iposbottom = mScreenHeight - mBtnFloatImgHeight;
//        iposy = iposy < 0 ? 0 : iposy;
//        iposy = iposy > iposbottom ? iposbottom : iposy;
//
//        AnimationSet animationSet = new AnimationSet(true);
//        
//        TranslateAnimation translateAnimation = new TranslateAnimation( 
//                0, iposx - mScreenWidth / 2 + len240 / 2, 0, iposy - mScreenHeight / 2 + len240/ 2 ); 
//        //TranslateAnimation translateAnimation = new TranslateAnimation(  startx - endx, 0, starty - endy, 0 ); 
//        translateAnimation.setDuration( 200 );      
//
//        ScaleAnimation scaleAnimation = new ScaleAnimation( 1, 0.367f, 1, 0.225f );
//        scaleAnimation.setDuration( 200 ); 
////          scaleAnimation.setStartOffset(300);
//        
//        animationSet.addAnimation(scaleAnimation);  
//        animationSet.addAnimation(translateAnimation);  
////          animationSet.setFillAfter(true);      
////          animationSet.setFillBefore(false);      
//        
//        view.setAnimation(animationSet);
//        animationSet.start();
//    }
//
//    
//	 public void loadPageStart()
//	 {
//		 if( mbFirstLoad )
//			 mMenuStatus = UserStatus.MENU_LOADING;
//		 else if (mUserStatus == UserStatus.USER_NORMAL)	// zhengjl 2011.09.09
//    	 {
//    	     showProgressLoading();
//    	 }  
//	 }
//
//	 public void loadPagePercent( int per )
//	 {
//		 if(  ! mbFirstLoad )
//		 {
//			 if( per < 5 )
//				 per = 5;
//			 mProgressLoading.setProgress( per );
//			 //mProgressLoading.bringToFront();
//		}
//	 }
//
//	 public void loadPageEnd()
//	 {
//		 //Log.e("MobileMain_maopao", "loadPageEnd" );
//		 if(  mbFirstLoad )
//			 mMenuStatus = UserStatus.MENU_BROWSER;
//		 else if (mUserStatus == UserStatus.USER_NORMAL)
//        {
//            ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER);
//            btnMenuHomeEnabled( ! mMobileView.isCurrentHomePage() );
//    	    btnMenuForwardEnabled( mMobileView.canForward() );
//            btnMenuLocalEnabled( true );
//            btnMenuBackEnabled( mMobileView.canBack() );
//            btnMenuRefreshEnabled( true );
//            btnMenuMoreEnabled( true );
//        }
//        else if (mMenuStatus == UserStatus.MENU_LOADING)
//        {
//            HideMenu();
//        } 
//	 }
//
//	   //flash加载完成回调函数
//     public void flashLoadingEnd()
//     {
//    	 //Log.e("--------->", "flashLoadingEnd");
//         if( mUserStatus == UserStatus.FLASH_TYPE_GAME)
//         {
//             mBtnGameSave.setEnabled( true );
//             mBtnGameSave.setBackgroundResource( R.drawable.menu_save );
//             mBtnGameSaveOld.setEnabled(true);
//             mBtnGameSaveOld.setBackgroundResource(R.drawable.menu_save_old);
//         }
//         else if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA )
//         {
//             mBtnMediaSave.setEnabled( true );
//             mBtnMediaSave.setBackgroundResource( R.drawable.menu_save );
//             mMenuMediaSaveOld.setEnabled(true);
//             mMenuMediaSaveOld.setBackgroundResource(R.drawable.menu_save_old);
//         }
//     }
//     
//     public void closeSubView()
//     {
////    	 Log.e("test", "closeSubView");
//    	 SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
//    	 boolean showOldMenu=settings.getBoolean("showOldMenu", true);
//    	 if(showOldMenu)
//    	 {
//	    	  if(!GlobalFun.showMenu)
//	    	  {
//	    		  mMobileView.setBottomPos(mScreenHeight-mHeight44);
//	    		
//	    		  if(mUserStatus==UserStatus.FLASH_TYPE_GAME)
//	    		  {
//	    			  showMenuGameOld();
//	    		  }
//	    		  else if(mUserStatus==UserStatus.FLASH_TYPE_MEDIA)
//	    		  {
//	    			  showMenuMediaOld();
//	    		  }
//	    		  else if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
//	    		  {
//	    			 showMenuWebGameOld();
//	    		  }
//	    	  }
//    	 }
//    	 else
//    	 {
//    		 HideMenu();
//    		 showMenuFloatView();
//    	 }
//    	  mViewPopStatus = UserStatus.VIEW_POP_NONE;
//     }
//
////	public void setContentView(View view)
////	{
////		super.setContentView(view);
////	}
//	 private boolean get24HourMode() {
//		 return android.text.format.DateFormat.is24HourFormat(this);
//	 }
//	 private void setFormat() {
//	      if (get24HourMode()) 
//	      {
//	         mFormat = m24;
//	      } else 
//	      {
//	         mFormat = m12;
//	      }
//	 }
//	 private void setFloatFormat() {
//	      if (get24HourMode()) 
//	      {
//	         mFormat_float = m24_float;
//	      } else 
//	      {
//	         mFormat_float = m12_float;
//	      }
//	 }
//	 private static String format(int t) {
//		 String s = "" + t;
//		 if (s.length() == 1) 
//		 {
//			 s = "0" + s;
//		 }
//		 return s;
//
//	 }
//	 
////	public BwPaySDK bwPaySDK = null;
//	public BwSDK bwSDK = null;	 
//    Timer startSDKTimer = null;
//    TimerTask startSDKTask = null;
//    public void enterUserCenter()
//    {
//    	bwSDK.enterUserCenter();
//    }
//    public void paySDK( String strUrl )
//    {
//    	bwSDK.pay(strUrl);
//    }
//    public void setOutLogin(int b)
//    {
//    	if (!bwSDK.isCreatedPay)
//    	{
//        	bwSDK.createSDKPay(this,b);
//	        Message msg = new Message();
//	        msg.what = MSG_SDKINIT;
//	        mProcessUseMsg.sendMessageDelayed(msg, bwSDK.delayTime+3000);
//    	}
//    }
//
//	public void closeSoftKey()
//	{
//		InputMethodManager imm = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
//		if (imm.isAcceptingText())
//		{
//			imm.toggleSoftInput(InputMethodManager.SHOW_IMPLICIT,
//					InputMethodManager.HIDE_NOT_ALWAYS);
//		}
//		if (mMobileView != null)
//		{
//			imm.hideSoftInputFromWindow(mMobileView.getWindowToken(), 0);
//		}
//	}
//	public String encUIDStr(String str)
//	{
//		return mMobileView.encUIDStr(str);
//	}
//	public String getAppUA()
//	{
//		return mMobileView.getAppUA();
//	}
//	public String getAppHeaders()
//	{
//		return mMobileView.getAppHeaders();
//	}
//	public void connectUrl(String str)
//	{
//		mMobileView.connectUrl(str);
//	}
//	// end zhengjl 2012.03
//
//	
//	@Override
//    protected void onActivityResult( int requestCode, int resultCode, Intent data )
//    {
////    	if (bwPaySDK != null)
////    	{
////    		boolean flag = bwPaySDK.result(requestCode, resultCode, data);  
////    		if (flag) return;
////    	}
//		boolean flag = bwSDK.result(requestCode, resultCode, data);  
//		if (flag) return;
//		
//    	switch( resultCode )
//    	{
//    	case RESULT_OK:
//    		Bundle bl = data.getExtras();
//    		String strFile = bl.getString("strFile");
//    		strFile = FILE_PROTOCOL + strFile;
//    		//Log.e("test", strFile );
//    		mMobileView.connectUrl( strFile );
//    		//mMobileView.connectUrl("/data/testcs.swf");
//    		break;
//
//    	default:
//    			break;
//    	}
//    	return;
//    }
//
//	protected void onStart()
//	{
//		Log.e("MobileMain_maopao", "onStart");
//		super.onStart();
//		
//        Message msg = new Message();
//        msg.what = MSG_UPDATEVIEW;
//        mProcessUseMsg.sendMessageDelayed(msg, 1000);
//        
//	}
//
////	protected void onRestart()
////	{
////		Log.e("MobileMain_maopao", "onRestart");
////		super.onRestart();
////	}
//	
//	protected void onResume()
//	{
//		Log.e("MobileMain_maopao", "onResume");
//		super.onResume();
//		mMobileView.onResume();
//		
//        Message msgTime = new Message();
//        msgTime.what = MSG_TIMEER;
//        mProcessUseMsg.sendMessageDelayed(msgTime, 50 );
//        mbRuning = true;
//	    KeyguardManager mKeyguardManager = (KeyguardManager) getSystemService(KEYGUARD_SERVICE);  
//	    if ( ! mKeyguardManager.inKeyguardRestrictedInputMode()) 
//	    { 
//	        if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA || mUserStatus == UserStatus.FLASH_TYPE_GAME )
//            {
//                mMobileView.flashResume();
//            }
//            else if( mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
//            {
//                mMobileView.setSilence( false );
//            }
//	        mbRuning = true;
//	    }  
//	}
//
//	@Override
//	protected void onPause()
//	{
//		Log.e("MobileMain_maopao", "onPause");
//		super.onPause();
//		mMobileView.onPause();
//		if (mbRuning)
//		{
//			if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA || mUserStatus == UserStatus.FLASH_TYPE_GAME )
//			{
//			    mbRuning = false;
//				mMobileView.flashPause();
//			}
//			else if( mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
//			{
//			    mMobileView.setSilence( true );
//			}
//			else
//			{
//			    mbRuning = false;
//			}
//		}
//	}
//
//	protected void onStop()
//	{
//		Log.e("MobileMain_maopao", "onStop");
//		super.onStop();
////		if (mbRuning || mUserStatus != UserStatus.FLASH_TYPE_WEBGAME )
////		{
////			mbRuning = false;
////		}
//	}
//
//	protected void onDestroy()
//	{
//	    //Debug.stopMethodTracing();
//		Log.e("MobileMain_maopao", "onDestroy");
//		super.onDestroy();
//		mbRuning = false;
//
//		mMobileView.Release();
//		nativeFinalize();
////    	if (bwPaySDK != null)
////    	{
////    		bwPaySDK.destory();    		
////    	}
//		bwSDK.destory();    		
//    	
//		try
//		{
//			Thread.sleep(200);
//		}
//		catch (InterruptedException e)
//		{
//			Thread.currentThread().interrupt();
//		}
//	}
//
//	@Override
//	public void onConfigurationChanged(Configuration newConfig)
//	{
//		//Log.e("MobileMain_maopao", "onConfigurationChanged");
//		super.onConfigurationChanged(newConfig);
//        if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) 
//        {
//            // 加入横屏要处理的代码
//
//        }
//        else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT)
//        {
//            // 加入竖屏要处理的代码
//        }
//	}
//
//	
//	protected void onSaveInstanceState(Bundle outState)
//	{
//		super.onSaveInstanceState(outState);
//	}
//
//    private float mfOldDist = 1f;
//    private int mfFlashZoom = 0;
//    private int mfFlashZoomTmp = 0;
//
//    private void ZoomFlashView( float fscale)
//    {
//        if( fscale > 1 )
//        {
//            mfFlashZoomTmp = mfFlashZoom + (int)((fscale -1 ) * 40);
//            if( mfFlashZoomTmp > 100 )
//                mfFlashZoomTmp = 100;
//        }
//        else
//        {
//            mfFlashZoomTmp = mfFlashZoom - (int)((1-fscale)* 4 * 40);
//            if( mfFlashZoomTmp < 0 )
//                mfFlashZoomTmp = 0;
//        }
//        mMobileView.flashZoomInOut( mfFlashZoomTmp );
//    }
//
//    //判断点是否在矩形内
//    public boolean pointInRect( Rect rect, int x, int y )
//    {
//        if( x > rect.left && x < rect.right &&
//            y > rect.top && y < rect.bottom )
//        {
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//    }
//    
//    public boolean menuBackFromTouch()
//    {
//    	//Log.e("menuBackFromTouch -------------mViewPopStatus-----------", ""+mViewPopStatus);
//         if(mViewPopStatus==UserStatus.VIEW_POP_PAY||mViewPopStatus==UserStatus.VIEW_POP_HELP)
//	     {
//	    	 return false;
//	     }
//
//    	 SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
//         mbShowOldMenu=settings.getBoolean("showOldMenu", true);
//        //Log.e("---------->", "menuBackFromTouch" );
//    	if( mMenuStatus == UserStatus.MENU_GAME)
//        {
//            //mMobileView.flashResume();
//            hideMenuAnimation(mMenuFloatGame);
//            //showMenuFloatView();
//            Message msgTime = new Message();
//    	    msgTime.what = MSG_SHOWFLOATVIEW;
//            mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//            return true;
//        }
//        else if( mMenuStatus == UserStatus.MENU_MEDIA )
//        {
//            //mMobileView.flashResume();
//            hideMenuAnimation(mMenuFloatMedia);
////            showMenuFloatView();
//            Message msgTime = new Message();
//    	    msgTime.what = MSG_SHOWFLOATVIEW;
//            mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//            return true;
//        }
//        else if( mMenuStatus == UserStatus.MENU_WEBGAME )
//        {
//            mMobileView.setSilence( false );
//            hideMenuAnimation(mMenuFloatWebGame);
////            showMenuFloatView();
//            Message msgTime = new Message();
//    	    msgTime.what = MSG_SHOWFLOATVIEW;
//            mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//            return true;
//        }
////        else if(mMenuStatus==UserStatus.MENU_GAME_OLD)
////        {
////            mMenuUserSetting.setVisibility( View.GONE );
////            mMenuGameOld.setVisibility(View.VISIBLE);
////            mMenuGameOld.bringToFront();
////            
////        }
//        else if( mMenuStatus == UserStatus.MENU_SETTING_FLASH )
//        {
//            //Log.e("---------->", "x:" + mX1 +", width:" + mScreenWidth / 2 + ", y:" + mY1 + ", height" + mScreenHeight / 2);
//            if( Math.abs(mX1 -  mScreenWidth / 2) < 120 && Math.abs(mY1 -  mScreenHeight / 2) < 120  )
//            {
//                //Log.e("---------->", "in MENU_FLOAT_SETTING");
//                mMenuUserSetting.setVisibility( View.GONE );
//                backFromUserSetting();
//            }
//            else
//            {
//                //Log.e("---------->", "out MENU_FLOAT_SETTING");
//                //mMobileView.flashResume();
//                mMobileView.setSilence( false );
//                hideMenuAnimation( mMenuUserSetting );
//                if(mbShowOldMenu)
//                {
//                	if(mUserStatus==UserStatus.FLASH_TYPE_GAME)
//                		showMenuGameOld();
//                	if(mUserStatus==UserStatus.FLASH_TYPE_MEDIA)
//                		showMenuMediaOld();
//                	if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
//                		showMenuWebGameOld();
//                }
//                if(!mbShowOldMenu)
//                {
//                	showMenuFloatView();
//                }
//            }
//            return true;
//        }
//        return false;
//    }
//
//    private boolean mbSwap = false;  // 判断两个点是否交换
//    private int mX1, mY1;
//    private int mX2, mY2;
//
//	public boolean onTouchEvent(MotionEvent event)
//	{
//	    //Log.e("MobileMain_maopao-------------->", "in onTouchEvent fun");
//	    HideMenuPopMore();
//	    mX1 = (int)event.getX( 0 );
//	    mY1 = (int)event.getY( 0 );
//		
//		switch (event.getAction() & MotionEvent.ACTION_MASK)
//		{
//        case MotionEvent.ACTION_DOWN:
//            if( menuBackFromTouch() )
//                return true;
//         
//            //Log.e("MobileMain_maopao-------------->", "in onTouchEvent fun ACTION_DOWN");
//            if( mbSensorOpen )
//            {
//                mSensorManager.unregisterListener(mSensorListener);
//            }
//            mbSwap = false;
//            //mGestureStatus = UserStatus.USER_ACTION_TOUCH_DOWN;
//            mUserActionStatus = UserStatus.USER_ACTION_TOUCH_DOWN;
//            mMobileView.onTouchDownCpp(mX1, mY1, 0 );
//            mMobileView.invalidateView1(mX1, mY1, mbShowMouseSign );
//            break;
//        case MotionEvent.ACTION_POINTER_DOWN:
//            //Log.e("MobileMain_maopao-------------->", "in onTouchEvent fun ACTION_POINTER_DOWN");
//            mMobileView.invalidateView1(mX1, mY1, false );
//            if( mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN )
//            {
//                int x2 = (int)event.getX( 1 );
//                int y2 = (int)event.getY( 1 );
//                //Log.e("---------------->onTouchEvent", "ACTION_POINTER_DOWN" + x2 + " " + y2 );
//                if( mbSwap )
//                {
//                    mMobileView.onTouchDownCpp(x2, y2, 0 );
//                }
//                else
//                {
//                    mMobileView.onTouchDownCpp(x2, y2, 1 );
//                }
//                //mGestureStatus = UserStatus.USER_ACTION_TOUCH_POINTER_DOWN;
//                mUserActionStatus = UserStatus.USER_ACTION_TOUCH_POINTER_DOWN;
//                //mMobileView.invalidateView2( x2, y2, true );
//            }
//            break;
//        case MotionEvent.ACTION_MOVE:
//            //Log.e("MobileMain_maopao-------------->", "in onTouchEvent fun ACTION_MOVE");
//            if( mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN )
//            {
//                mMobileView.invalidateView1(mX1, mY1, mbShowMouseSign );
//            }
//            if( mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN ||
//                mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_UP)
//            {
//                if( mbSwap )
//                {
//                    mMobileView.onTouchMoveCpp(mX1, mY1, 1 );
//                }
//                else
//                {
//                    mMobileView.onTouchMoveCpp(mX1, mY1, 0 );
//                }
//            }
//            else if( mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN)
//            {
//                //Log.e("---------------->onTouchEvent", "ACTION_MOVE x1:" + mX1 + ", y1:" + mY1 +", x2:" + mX2 + ", y2:" + mY2 );
//                mX2 = (int)event.getX( 1 );
//                mY2 = (int)event.getY( 1 );
//                //mMobileView.invalidateView2( mX2, mY2, true );
//                if( mbSwap )
//                {
//                    mMobileView.onTouchMoveCpp(mX1, mY1, 1);
//                    mMobileView.onTouchMoveCpp(mX2, mY2, 0);
//                }
//                else
//                {
//                    mMobileView.onTouchMoveCpp(mX1, mY1, 0);
//                    mMobileView.onTouchMoveCpp(mX2, mY2, 1);
//                }
//            }
//        	break;
//        case MotionEvent.ACTION_POINTER_UP:
//            //Log.e("MobileMain_maopao-------------->", "in onTouchEvent fun ACTION_POINTER_UP");
//            if( mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN )
//            {
//                //Log.e("---------------->onTouchEvent", "ACTION_POINTER_UP" );
//                int x2 = (int)event.getX( 1 );
//                int y2 = (int)event.getY( 1 );
//                if( mX2 > x2 - 5 && mX2 < x2 + 5 &&
//                    mY2 > y2 - 5 && mY2 < y2 + 5)
//                {
//                    mbSwap = false;
//                }
//                else
//                {
//                    mbSwap = true;
//                }
//                mMobileView.onTouchUpCpp(x2, y2, 1 );
//
//                if ( (mUserStatus == UserStatus.FLASH_TYPE_MEDIA ||
//                      mUserStatus == UserStatus.FLASH_TYPE_GAME ||
//                      mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) &&
//                      mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN  )
//               {
//                   switchZoomingStatus();
//               }
//
//                //mGestureStatus = UserStatus.USER_ACTION_TOUCH_DOWN;
//                mUserActionStatus = UserStatus.USER_ACTION_TOUCH_POINTER_UP;
//                miTimeCount = 4;
//            }
//        	break;
//        case MotionEvent.ACTION_UP:
//            //Log.e("MobileMain_maopao-------------->", "in onTouchEvent fun ACTION_UP");
//            //Log.e("---------------->onTouchEvent", "ACTION_UP" );
//            mbSwap = false;
//            mMobileView.onTouchUpCpp(mX1, mY1, 0);
//            //mGestureStatus = UserStatus.USER_ACTION_NONE;
//            if( mbSensorOpen )
//            {
//                mSensorManager.registerListener(mSensorListener, mSensor, SensorManager.SENSOR_DELAY_GAME);
//            }
//            mUserActionStatus = UserStatus.USER_ACTION_TOUCH_UP;
//            miTimeCount = 4;
//            break;
//
//        }
//		return super.onTouchEvent( event );
//	}
//
//    private float spacingDist(MotionEvent event)
//    {
//        float x = event.getX(0) - event.getX(1);
//        float y = event.getY(0) - event.getY(1);
//        return FloatMath.sqrt(x * x + y * y);
//    }
//
//	public boolean dispatchTouchEvent(MotionEvent ev)
//	{
//		super.dispatchTouchEvent(ev);
//		return false;
//	}
//
//	public boolean onKeyDown(int keyCode, KeyEvent event)
//	{
//		//Log.e("MobileMain_maopao","onKeyDown");
//		switch (keyCode)
//		{
//		case KeyEvent.KEYCODE_BACK:
//			return true;
//
//		case KeyEvent.KEYCODE_DPAD_CENTER:
//			mMobileView.onKeyDownCpp( 0x0D, event);
//			break;
//		case KeyEvent.KEYCODE_DPAD_UP:
//			mMobileView.onKeyDownCpp( 0x26, event);
//			break;
//		case KeyEvent.KEYCODE_DPAD_DOWN:
//			mMobileView.onKeyDownCpp( 0x28, event);
//			break;
//		case KeyEvent.KEYCODE_DPAD_LEFT:
//			mMobileView.onKeyDownCpp( 0x25, event);
//			break;
//		case KeyEvent.KEYCODE_DPAD_RIGHT:
//			mMobileView.onKeyDownCpp( 0x27, event);
//			break;
//		}
//		return super.onKeyDown( keyCode, event);
//	}
//
//	public boolean processKeyBack( )
//	{ 
//		//Log.e("processKeyBack ---------mViewPopStatus--------",""+mViewPopStatus);
//		SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
//		boolean showOldMenu=settings.getBoolean("showOldMenu", true);
//	    if( mEditText != null )
//	    {
//	        SetEditText( );
//	    }
//        else if ( (mViewPopStatus == UserStatus.VIEW_POP_HELP ||
//                   mViewPopStatus == UserStatus.VIEW_POP_PAY))
//        {
//	        mMobileView.closePopView();
//	        mViewPopStatus = UserStatus.VIEW_POP_NONE;
//	    	 if(showOldMenu&&!GlobalFun.showMenu)
//	    	 {
//	    		 mMobileView.setBottomPos(mScreenHeight-mHeight44);
//		        if(mUserStatus==UserStatus.FLASH_TYPE_GAME)
//		        {
//		        	showMenuGameOld();
//		        }
//		        if(mUserStatus==UserStatus.FLASH_TYPE_MEDIA)
//		        {
//		        	showMenuMediaOld();
//		        }
//		        if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
//		        {
//		        	showMenuWebGameOld();
//		        }
//	    	 }
//	    	 if(!showOldMenu)
//	    	 {
//	    		 HideMenu();
//	    		 showMenuFloatView();
//	    	 }
//	        if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
//	        {
//	        		mMobileView.setSilence( false );
//	        }
//           
//        }
//	    else if( mMenuStatus == UserStatus.MENU_BROWSER_POP)
//	    {
//	        HideMenuPopMore();
//	    }
//	    else if( mMenuStatus == UserStatus.MENU_MEDIA )
//        {
//	        //mMobileView.flashResume();
//            hideMenuAnimation(mMenuFloatMedia);
////            showMenuFloatView();
//            Message msgTime = new Message();
//    	    msgTime.what = MSG_SHOWFLOATVIEW;
//            mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//
//        }
//        else if( mMenuStatus == UserStatus.MENU_GAME )
//        {
//            //mMobileView.flashResume();
//        	hideMenuAnimation(mMenuFloatGame);
////            showMenuFloatView();
//            Message msgTime = new Message();
//    	    msgTime.what = MSG_SHOWFLOATVIEW;
//            mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//        }
//        else if( mMenuStatus == UserStatus.MENU_WEBGAME )
//        {
//            mMobileView.setSilence( false );
//            hideMenuAnimation(mMenuFloatWebGame);
////            showMenuFloatView();
//            Message msgTime = new Message();
//    	    msgTime.what = MSG_SHOWFLOATVIEW;
//            mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//        }
////        else if(mMenuStatus==UserStatus.MENU_GAME_OLD)
////        {
////        	if(GlobalFun.showMenu)
////	    		{
////	    			mMenuGameOld.setVisibility(View.GONE);
////	    			GlobalFun.showMenu=false;
////	    			mMobileView.setBottomPos(mScreenHeight);
////	    		}
////        	else
////	    		{
////	    			menuExitFlash();
////	    		}
////        }
////        else if(mMenuStatus==UserStatus.MENU_WEBGAME_OLD)
////        {
////        	if(GlobalFun.showMenu)
////	    		{
////	    			mMenuWebGameOld.setVisibility(View.GONE);
////	    			GlobalFun.showMenu=false;
////	    			mMobileView.setBottomPos(mScreenHeight);
////	    		}
////        	else
////    		{
////    			menuExitFlash();
////    		}
////        }
////        else if(mMenuStatus==UserStatus.MENU_MEDIA_OLD)
////        {
////    		if(GlobalFun.showMenu)
////    		{
////    			mMenuMediaOld.setVisibility(View.GONE);
////    			GlobalFun.showMenu=false;
////    			mMobileView.setBottomPos(mScreenHeight);
////    		}
////    		else
////    		{
////    			menuExitFlash();
////    		}
////        }
//        else if( mMenuStatus == UserStatus.MENU_SETTING_FLASH )
//        {
//            mMenuUserSetting.setVisibility( View.GONE );
//            backFromUserSetting();
//        }
////	    else if( mMobileView.canBack() )
////		{
////			mMobileView.onDomBack();
////			btnHomeBackEnable();
////		}
//		else if( mUserStatus == UserStatus.FLASH_TYPE_MEDIA ||
//		         mUserStatus == UserStatus.FLASH_TYPE_GAME  ||
//		         mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
//		{
//		    menuExitFlash();
//		}
//		else
//		{
//		    AppQuit();
//		}
//		return false;
//	}
//
//	public boolean onKeyUp(int keyCode, KeyEvent event)
//	{
//       //Log.e("MobileMain_maopao","onKeyUp");
//	   SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
//   	   boolean showOldMenu=settings.getBoolean("showOldMenu", true);
//		switch (keyCode)
//		{
//        case KeyEvent.KEYCODE_SEARCH:
//            if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA ||
//                mUserStatus == UserStatus.FLASH_TYPE_GAME ||
//                mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) 
//            {
//                switchZooming();
//            }
//            return true;
//		case KeyEvent.KEYCODE_BACK:
//		{
//			if( processKeyBack())
//			    return true;
//			else
//			    break;
//		}
//		case KeyEvent.KEYCODE_DPAD_CENTER:
//			mMobileView.onKeyUpCpp( 0x0D, event);
//			break;
//		case KeyEvent.KEYCODE_DPAD_UP:
//			mMobileView.onKeyUpCpp( 0x26, event);
//			break;
//		case KeyEvent.KEYCODE_DPAD_DOWN:
//			mMobileView.onKeyUpCpp( 0x28, event);
//			break;
//		case KeyEvent.KEYCODE_DPAD_LEFT:
//			mMobileView.onKeyUpCpp( 0x25, event);
//			break;
//		case KeyEvent.KEYCODE_DPAD_RIGHT:
//			mMobileView.onKeyUpCpp( 0x27, event);
//			break;
//
//		case KeyEvent.KEYCODE_MENU:
//			{
////			    if(  mMenuStatus == UserStatus.MENU_BROWSER || mMenuStatus == UserStatus.MENU_LOADING )
////	                    break;
////			    else if( mMenuStatus == UserStatus.MENU_BROWSER_POP )
////                {
////                    HideMenuPopMore();
////                }
////			    else if ( (mViewPopStatus == UserStatus.VIEW_POP_HELP ||
////		                  mViewPopStatus == UserStatus.VIEW_POP_PAY))
////		        {
////		            mMobileView.closePopView();
////			    	 if(showOldMenu&&!GlobalFun.showMenu)
////			    	 {
////			    		mMobileView.setBottomPos(mScreenHeight-mHeight44);
////				        if(mUserStatus==UserStatus.FLASH_TYPE_GAME)
////				        {
////				        	showMenuGameOld();
////				        }
////				        if(mUserStatus==UserStatus.FLASH_TYPE_MEDIA)
////				        {
////				        	showMenuMediaOld();
////				        }
////				        if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
////				        {
////				        	showMenuWebGameOld();
////				        }
////			    	 }
////			    	 else if(!showOldMenu)
////			    	 {
////			    		 HideMenu();
////			    		 showMenuFloatView();
////			    	 }
////		            if( mUserStatus == UserStatus.FLASH_TYPE_WEBGAME )
////		                mMobileView.setSilence( false );
////		            mViewPopStatus = UserStatus.VIEW_POP_NONE;
////		            
////		        }
////			    else if( mMenuStatus == UserStatus.MENU_FLOAT_VIEW || 
////			             mMenuStatus == UserStatus.MENU_NONE )
////	            {
////			    	
////			        changeUserMenu();
////                } 
////			    else if( mMenuStatus == UserStatus.MENU_MEDIA )
////			    {			    	
////			        //mMobileView.flashResume();
////			        hideMenuAnimation(mMenuFloatMedia);
//////			        showMenuFloatView();
////		            Message msgTime = new Message();
////	        	    msgTime.what = MSG_SHOWFLOATVIEW;
////	                mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
////		    
////			    }
////			    else if( mMenuStatus == UserStatus.MENU_GAME )
////			    {		      
////			    	//mMobileView.flashResume();
////			        hideMenuAnimation(mMenuFloatGame);
//////			        showMenuFloatView();
////			        Message msgTime = new Message();
////	        	    msgTime.what = MSG_SHOWFLOATVIEW;
////	                mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
////			     
////			    }
////			    else if( mMenuStatus == UserStatus.MENU_WEBGAME )
////			    {
////			    	mMobileView.setSilence( false );
////			        hideMenuAnimation(mMenuFloatWebGame);
//////			        showMenuFloatView();
////		             Message msgTime = new Message();
////		        	 msgTime.what = MSG_SHOWFLOATVIEW;
////		             mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
////			    }
////			    else if(mMenuStatus==UserStatus.MENU_GAME_OLD)
////			    {
////			    	if(GlobalFun.showMenu)
////			    		{
////			        		mMenuGameOld.setVisibility(View.GONE);
////			        		GlobalFun.showMenu=false;
////			        		mMobileView.setBottomPos(mScreenHeight);
////			    		}
////			    		else
////			    		{
////			    			mMenuGameOld.setVisibility(View.VISIBLE);
////			    			mMenuGameOld.bringToFront();
////			    			GlobalFun.showMenu=true;
////			    			mMobileView.setBottomPos(mScreenHeight-mHeight44);
////			    		}
////			    }
////			    else if(mMenuStatus==UserStatus.MENU_WEBGAME_OLD)
////			    {
////			    	if(GlobalFun.showMenu)
////			    		{
////			        		mMenuWebGameOld.setVisibility(View.GONE);
////			        		GlobalFun.showMenu=false;
////			        		mMobileView.setBottomPos(mScreenHeight);
////			    		}
////			    		else
////			    		{
////			    			mMenuWebGameOld.setVisibility(View.VISIBLE);
////			    			mMenuWebGameOld.bringToFront();
////			    			GlobalFun.showMenu=true;
////			    			mMobileView.setBottomPos(mScreenHeight-mHeight44);
////			    		}
////			    }
////			    else if(mMenuStatus==UserStatus.MENU_MEDIA_OLD)
////			    {
////			    	if(GlobalFun.showMenu)
////		    		{
////		    			mMenuMediaOld.setVisibility(View.GONE);
////		    			GlobalFun.showMenu=false;
////		    			mMobileView.setBottomPos(mScreenHeight);
////		    		}
////		    		else
////		    		{
////		    			mMenuMediaOld.setVisibility(View.VISIBLE);
////		    			mMenuMediaOld.bringToFront();
////		    			GlobalFun.showMenu=true;
////		    			mMobileView.setBottomPos(mScreenHeight-mHeight44);
////		    		}
////		    
////			    }
////			    else if( mMenuStatus == UserStatus.MENU_SETTING_FLASH )
////			    {
////			        mMenuUserSetting.setVisibility( View.GONE );
////	                backFromUserSetting();
////			    }
//				break;
//			} // end of KeyEvent.KEYCODE_MENU
//		default:
//			return super.onKeyUp(keyCode, event);
//
//		}  // endof switch
//		return super.onKeyUp(keyCode, event);
//	}
//
//	public void onBackPressed()
//	{
//		super.onBackPressed();
//	}
//
//	 public boolean onTrackballEvent(MotionEvent event)
//	 {
//		 super.onTrackballEvent( event );
//		 return false;
//	 }
//
//	public boolean onCreateOptionsMenu(Menu menu)
//	{
//		super.onCreateOptionsMenu(menu);
//		return true;
//	}
//
//    private void ClearBuffer()
//    {
//    	Dialog dlg = new AlertDialog.Builder(MobileMain_maopao.this)
//        .setIcon(R.drawable.logo_36)
//        .setTitle("是否清除缓存？")
//        .setPositiveButton(R.string.positive_button, new DialogInterface.OnClickListener()
//        {
//            public void onClick(DialogInterface dialog, int whichButton)
//            {
//            	mMobileView.clearCache();
//            	Toast.makeText(MobileMain_maopao.this, "缓存清除完成", Toast.LENGTH_SHORT ).show();
//            }
//        }).setNegativeButton(R.string.negative_button, new DialogInterface.OnClickListener(){
//			public void onClick(DialogInterface dialog, int which) {
//
//			}
//		})
//        .create();
//
//    	dlg.show();
//    	return;
//    }
//    private void AppQuit()
//    {
//		Dialog quitDlg = new AlertDialog.Builder(MobileMain_maopao.this)
//        .setIcon(R.drawable.logo_36)
//        .setTitle(R.string.app_exit)
//        .setPositiveButton(R.string.positive_button, new DialogInterface.OnClickListener() {
//            public void onClick(DialogInterface dialog, int whichButton) {
//		    	mMobileView.onDomQuit();
//		    	mMobileView.Release();
//		    	nativeFinalize();
//		    
//		    	// 主动退出 freesky sdk 服务
//		    	// 注意不要在闪现界面进行 init()和 unInit()
//		    	// 会导致数据来不及完成发送,进而服务端无法判断应用已经正确接入了SDK
//		    	FreeSkySdk.getInstance().unInit();
//		    	
//            	unregisterReceiver(mUnlockReceiver);
//		    	finish();
//		    	android.os.Process.killProcess(android.os.Process.myPid());
//		    	startPushService();
//            }
//        }).setNegativeButton(R.string.negative_button, new DialogInterface.OnClickListener() {
//            public void onClick(DialogInterface dialog, int whichButton) {
//            }
//        }).create();
//
//        quitDlg.show();
//
//        return;
//    }
//
//    public void MobileNative()
//    {
//    	Intent intent = new Intent(this, LocalFlashAct.class);
//    	Bundle bl = new Bundle();
//    	bl.putString("LOCAL_PATH", GlobalFun.BWWORKPATH + "/Local" );
//    	intent.putExtras(bl );
//    	startActivityForResult( intent, 0 );
//    }
//
//    public void AboutUs()
//    {
//    	Intent intent = new Intent(this, AboutActivity.class);
////    	if(first_about)
////    	{
////    	UA=getAppUA();
////    	appHeaders=getAppHeaders();
////    	first_about=false;
////    	}
//    	
//     	
//     	String UA=getAppUA();
//     	String appHeaders=getAppHeaders(); 	
//    	Bundle bl=new Bundle();
//    	bl.putString("UA", UA);
//    	String deviceID = Secure.getString(this.getContentResolver(), Secure.ANDROID_ID);
//    	bl.putString("DeviceID", deviceID );
//    	bl.putString("header", appHeaders);
//    	intent.putExtras(bl);
//    	startActivity(intent);
////        Intent intent = new Intent(this, BwWebView.class);
////        startActivity(intent);       
//    	//openInBrowser( "http://www.baidu.com" );
//    }
//    
//    public void openInBrowser( String strUrl )
//    {
//        Intent intent = new Intent(this, BwWebView.class);
//        Bundle bl = new Bundle();
//        bl.putString("BW_URL", strUrl );
//        intent.putExtras(bl );
//        startActivity(intent);  
//    }
//
//    public void CheckUpgrade()
//    {
//        mMobileView.checkUpgrade();
//    	Toast.makeText( this,"检测新版本", Toast.LENGTH_SHORT).show();
//    }
//
//    private int mbUpgradeStatus = UserStatus.UPGRADE_NOMAL;
//    public void upgradeApp( String strUrl, int status ) //升级程序，由c调用
//    {
//    	int ipos=strUrl.indexOf('|');	
//        if( mbUpgradeStatus == UserStatus.UPGRADE_CANCEL )
//        {
//            return;
//        }
//        if(ipos==-1)
//        {
//	        if( status == 1 )
//	        {
//	            mbUpgradeStatus = UserStatus.UPGRADE_CAN;
//	            final String url = strUrl;
//	            Dialog upDlg = new AlertDialog.Builder(MobileMain_maopao.this)
//	            .setIcon(R.drawable.logo_36)
//	            .setTitle("有新版本，请更新!")
//	            .setPositiveButton("确定", new DialogInterface.OnClickListener() {
//	                public void onClick(DialogInterface dialog, int whichButton) {
//	                    openUpgradeUrl( url );
//	                }
//	            }).setNegativeButton("取消", new DialogInterface.OnClickListener() {
//	                public void onClick(DialogInterface dialog, int whichButton) {
//	                    mbUpgradeStatus = UserStatus.UPGRADE_CANCEL;
//	                }
//	            }).create();
//	            upDlg.setOnKeyListener(new DialogInterface.OnKeyListener()
//	            {
//	                public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event)
//	                {
//	                    if ( keyCode == KeyEvent.KEYCODE_BACK )
//	                        return true;
//	                    else
//	                        return false;
//	                }
//	            });
//	
//	            upDlg.show();
//	        }
//	        else if( status == 2 )
//	        {
//	            mbUpgradeStatus = UserStatus.UPGRADE_MUST;
//	            final String url = strUrl;
//	            Dialog upmustDlg = new AlertDialog.Builder(MobileMain_maopao.this)
//	            .setIcon(R.drawable.logo_36)
//	            .setTitle("有新版本，请更新!")
//	            .setPositiveButton("确定", new DialogInterface.OnClickListener() {
//	                public void onClick(DialogInterface dialog, int whichButton) {
//	                    openUpgradeUrl( url );
//	                }
//	            }).create();
//	
//	            upmustDlg.setOnKeyListener(new DialogInterface.OnKeyListener()
//	            {
//	                public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event)
//	                {
//	                    if ( keyCode == KeyEvent.KEYCODE_BACK )
//	                        return true;
//	                    else
//	                        return false;
//	                }
//	            });
//	
//	            upmustDlg.show();
//	        }
//        }
//        else
//        {
//        	String updateTip=strUrl.substring(ipos+1);
//        	String  newtip=updateTip.replace('|', '\n');
//        	if( status == 1 )
//	        {
//	            mbUpgradeStatus = UserStatus.UPGRADE_CAN;
//	            final String url = strUrl.substring(0,ipos);
//	            Dialog upDlg = new AlertDialog.Builder(MobileMain_maopao.this)
//	            .setIcon(R.drawable.logo_36)
//	            .setTitle("有新版本，请更新!")
//	            .setMessage(newtip)
//	            .setPositiveButton("确定", new DialogInterface.OnClickListener() {
//	                public void onClick(DialogInterface dialog, int whichButton) {
//	                    openUpgradeUrl( url );
//	                }
//	            }).setNegativeButton("取消", new DialogInterface.OnClickListener() {
//	                public void onClick(DialogInterface dialog, int whichButton) {
//	                    mbUpgradeStatus = UserStatus.UPGRADE_CANCEL;
//	                }
//	            }).create();
//	            upDlg.setOnKeyListener(new DialogInterface.OnKeyListener()
//	            {
//	                public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event)
//	                {
//	                    if ( keyCode == KeyEvent.KEYCODE_BACK )
//	                        return true;
//	                    else
//	                        return false;
//	                }
//	            });
//	
//	            upDlg.show();
//	        }
//        	 else if( status == 2 )
// 	        {
// 	            mbUpgradeStatus = UserStatus.UPGRADE_MUST;
// 	           final String url = strUrl.substring(0,ipos);
// 	            Dialog upmustDlg = new AlertDialog.Builder(MobileMain_maopao.this)
// 	            .setIcon(R.drawable.logo_36)
// 	            .setTitle("有新版本，请更新!")
// 	            .setMessage(newtip)
// 	            .setPositiveButton("确定", new DialogInterface.OnClickListener() {
// 	                public void onClick(DialogInterface dialog, int whichButton) {
// 	                    openUpgradeUrl( url );
// 	                }
// 	            }).create();
// 	
// 	            upmustDlg.setOnKeyListener(new DialogInterface.OnKeyListener()
// 	            {
// 	                public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event)
// 	                {
// 	                    if ( keyCode == KeyEvent.KEYCODE_BACK )
// 	                        return true;
// 	                    else
// 	                        return false;
// 	                }
// 	            });
// 	
// 	            upmustDlg.show();
// 	        }
//        	
//        }
//    }
//    public void openViewUrl( String strUrl )
//    {
//    	//ext://usercenter_ucsdk/
////  调取第三方SDK时调用
////    	if(strUrl.contains("http://usercenter_ucsdk/"))
////    	{
////    		enterUserCenter();
////    	}
////    	else
////    	{
//        final String str = strUrl;
//        Dialog dlg = new AlertDialog.Builder(MobileMain_maopao.this)
//        .setIcon(R.drawable.logo_36)
//        .setTitle("程序将打开浏览器,并转到后台运行,是否继续")
//        .setPositiveButton(R.string.positive_button, new DialogInterface.OnClickListener()
//        {
//            public void onClick(DialogInterface dialog, int whichButton)
//            {
//                //openInBrowser(str);
//                Intent viewIntent = new Intent("android.intent.action.VIEW", Uri.parse( str ));
//                startActivity(viewIntent);
//            }
//        }).setNegativeButton(R.string.negative_button, new DialogInterface.OnClickListener(){
//            public void onClick(DialogInterface dialog, int which) {
//            }
//        })
//        .create();
//        dlg.show();
////    	}
//    }
//
//    public void openUpgradeUrl(String strUrl)
//    {
//        Intent intent = new Intent(MobileMain_maopao.this, UpgradeService.class);
//        File file = null;
//        file = new File( GlobalFun.BWWORKPATH + "/tmp.file");
//        FileOutputStream fout = null;
//        try
//        {
//            file.createNewFile();
//            fout = new FileOutputStream(file);
//            byte buf[] = strUrl.getBytes();
//            fout.write(buf, 0, buf.length);
//            fout.close();
//        }
//        catch (IOException e)
//        {
//            Log.e("tag", e.getMessage());
//        }
//        startService( intent );
//        
//        mMobileView.onDomQuit();
//    	mMobileView.Release();
//    	nativeFinalize();
//        unregisterReceiver(mUnlockReceiver);
//        finish();
//        return;
//
//        // Intent viewIntent = new Intent("android.intent.action.VIEW",
//        // Uri.parse( strUrl ));
//        // startActivity(viewIntent);
//    }
//
//    public void  HelpMsg()
//    {
//    	Intent intent = new Intent(this, HelpActivity.class);
//    	startActivity(intent);
////        Toast.makeText( this,"帮助信息", Toast.LENGTH_SHORT).show();
//    }
//
//	 private ProgressDialog mDlgSaveMedia = null;//保存flash进度条
//	 /*
//	  * 保存多媒体文件
//	  */
//    private void saveMediaFile()
//    {
//        if (!mMobileView.canFlashSave())
//            return;
//
//        mDlgSaveMedia = new ProgressDialog(this);
//        mDlgSaveMedia.setIcon(R.drawable.logo_36);
//        mDlgSaveMedia.setTitle("百玩游戏");
//        mDlgSaveMedia.setMessage("游戏保存中，请稍候...");
//        mDlgSaveMedia.setIndeterminate(true);
//        mDlgSaveMedia.setCancelable(true);
//        ColorDrawable draw= new ColorDrawable( 0xFFFF0000);
//        mDlgSaveMedia.setProgressDrawable( draw );
////        mSaveMediaDlg.setOnCancelListener(new DialogInterface.OnCancelListener()
////        {
////            public void onCancel(DialogInterface dialog)
////            {
////                mMobileView.flashSaveCancel();
////                mSaveMediaDlg.cancel();
////            }
////        });
//        mDlgSaveMedia.show();
//        mMobileView.flashSave();
//        return;
//    }
//
//    /*
//     * flash 保存完成时，被c调用
//     */
//	  public void flashSaveFinish( )
//	  {
//	      mDlgSaveMedia.cancel();
//	      mDlgSaveMedia = null;
//    	  //mBtnMenuPlayPause.setBackgroundResource(R.drawable.menu_media_pause );
//	      Toast.makeText(this, "保存成功", Toast.LENGTH_SHORT).show();
//	      System.gc();
//	  }
//
//	  private EditText mEditText = null;
//	  private LinearLayout mLayoutEdit = null;
//	  private Button mBtnEdit = null;
//
//	  private void SetEditText( )
//	  {
//		  if( mEditText == null )
//		  {
//			  return;
//		  }
//	      InputMethodManager imm = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
//          imm.hideSoftInputFromWindow(mEditText.getWindowToken(), 0);
//          String outText = mEditText.getText().toString();
//          //Log.e("MobileMain_maopao", outText );
//          mMobileView.SetXEdit(outText);
//          mEditText.setText( "" );
//		    mLayoutEdit.removeAllViews();
//		    mLayoutMobileMain.removeView(mLayoutEdit );
//		    mEditText = null;
//		    mLayoutEdit = null;
//		    mBtnEdit = null;
//		    System.gc();
////          if(outText.length()!=0)
////          {
////	          mMobileView.SetXEdit(outText);
////	          if( talkList.size()!=30)
////	          {
////	        	  if(!talkList.get(talkList.size()-1).equals(outText))
////	        	  {
////	        	  talkList.add(outText);
////	        	  }
////	          }
////	          else
////	          {
////	        	  if(!talkList.get(talkList.size()-1).equals(outText))
////	        	  {
////	        	  talkList.remove(0);
////	        	  talkList.add(outText);
////	        	  }
////	        	  
////	          }
////          }
//          
//          //setContentView(mLayoutMobileMain);
//	  }
//	/*
//	 * XEdit回调该函数，处理用户输入信息
//	 */
//	public void CallTextEdit(String inText, int ilength, int style )
//	{
//		if( mEditText != null )
//		{
//			SetEditText();
//		}
//		//mMobileView.setViewRegion( 0, 0, mScreenWidth, 400 );
////		ArrayAdapter<String> adapter =new ArrayAdapter<String>(this, R.layout.talk_list_item, talkList);
////	    final ListView myListView=new ListView(this);
//		mLayoutEdit = new LinearLayout(this);
//	    mEditText = new EditText(this);
//		//editLayout.setHorizontalGravity(5);
//		mLayoutEdit.setOrientation(LinearLayout.HORIZONTAL ); // 设置排列方式为水平
//		//mLayoutEdit.setGravity(Gravity.BOTTOM);
//		mLayoutEdit.setGravity(Gravity.TOP);
//		mLayoutEdit.setPadding(0, 0, 0, 0 );
//		LinearLayout.LayoutParams layoutParamEdit = new LinearLayout.LayoutParams(
//				mScreenWidth -100,
//				LinearLayout.LayoutParams.WRAP_CONTENT);
//
//		InputFilter[] filters = new InputFilter[1];
//		if( ilength > 0 )
//		{
//    		filters[0] = new InputFilter.LengthFilter( ilength );
//    		mEditText.setFilters( filters);
//		}
//		mEditText.setText(inText);
//
//		if( (style & UserStatus.XEDS_PASSWORD) != 0 )  // 设置密码风格
//		{
//		    mEditText.setInputType( InputType.TYPE_TEXT_VARIATION_PASSWORD | InputType.TYPE_CLASS_TEXT );
//		}
////		if( (style & UserStatus.XEDS_NUMBER) != 0 )
////		{
////		    mEditText( new DigitsKeyListener(false, true));
////		}
//		mEditText.setLayoutParams(layoutParamEdit);
//		mLayoutEdit.addView(mEditText);
//
//		mBtnEdit = new Button(this);
//		mBtnEdit.setText("完成");
//		mBtnEdit.setOnClickListener(new View.OnClickListener()
//		{
//			public void onClick(View v)
//			{
//			    SetEditText( );
//			}
//		});
//		LinearLayout.LayoutParams layoutParamsBtn = new LinearLayout.LayoutParams(
//				100,
//				LinearLayout.LayoutParams.WRAP_CONTENT);
//		mBtnEdit.setLayoutParams(layoutParamsBtn);
//		mLayoutEdit.addView(mBtnEdit);
//		
//		mLayoutMobileMain.addView( mLayoutEdit );
//		mEditText.requestFocus();
//		((InputMethodManager)getSystemService(INPUT_METHOD_SERVICE)).showSoftInput(mEditText, 0);
//		return;
//	}
//	
//	class AnimationParam
//	{
//	    View mView;
//	    long mDuration;
//	    float mTranFromX;
//	    float mTranToX;
//	    float mTranFromY;
//	    float mTranToY;
//	    float mScaleFromX;
//	    float mScaleToX;
//	    float mScaleFromY;
//	    float mScaleToY;
//	}
//
//	private final static int CENTER_XL = 140;
//	private final static int CENTER_XR = 160;
//	private final static int CENTER_Y = 100;
//	private void btnMoveScreen( int x, int y )
//	{
//	    mMobileView.onTouchDownCpp( CENTER_XL, CENTER_Y, 0 );
//        mMobileView.onTouchDownCpp( CENTER_XR, CENTER_Y, 1 );
//
//        mMobileView.onTouchMoveCpp( CENTER_XL, CENTER_Y, 0);
//        mMobileView.onTouchMoveCpp( CENTER_XR, CENTER_Y, 1);
//
//        mMobileView.onTouchMoveCpp( CENTER_XL + x, CENTER_Y + y, 0);
//        mMobileView.onTouchMoveCpp( CENTER_XR + x, CENTER_Y + y, 1);
//
//        mMobileView.onTouchMoveCpp( CENTER_XL + x * 2, CENTER_Y + y * 2, 0);
//        mMobileView.onTouchMoveCpp( CENTER_XR + x * 2, CENTER_Y + y * 2, 1);
//
//        mMobileView.onTouchUpCpp( CENTER_XL + x * 2, CENTER_Y + y * 2, 0 );
//        mMobileView.onTouchUpCpp( CENTER_XR + x * 2, CENTER_Y + y * 2, 1 );
//	}
//	private int miTimeCount = 0;     //定时器计数
//	private void timeTickActoin() //处理用户操作状态
//	{
//		timeCount+=1;
//		if(timeCount==20)
//		{
//			timeCount=0;
//			 Time time = new Time();    
//	         time.setToNow();     
//	         int minute = time.minute;   
//	         int hour = time.hour;   
//	         int sec = time.second; 
//	         timeWebGame.setText(format(hour)+":"+format(minute)+":"+format(sec));
////			mCalendar.setTimeInMillis(System.currentTimeMillis());
////		    timeWebGame.setText(DateFormat.format(mFormat, mCalendar));
////		    mCalendarFloat.setTimeInMillis(System.currentTimeMillis());
////		    timeWebGameFloat.setText(DateFormat.format(mFormat_float, mCalendarFloat));
//		   
//	         if(showTimeDot)
//	         {
//		         timeWebGameFloat.setText(format(hour)+":"+format(minute));
//		         showTimeDot=false;
//	         }
//	         else
//	         {
//	        	 timeWebGameFloat.setText(format(hour)+" "+format(minute));
//	        	 showTimeDot=true;
//	         }
//			
//		}
//	    if(  mViewPopStatus == UserStatus.VIEW_POP_PAY && !mMobileView.isPause())
//	    {
//	    	//Log.e("MobileMain_maopao", "processUserActoin" );
////	        mViewPopStatus = UserStatus.VIEW_POP_NONE;
//	        mMobileView.setSilence( false );
//	    }
//	    
//	    if( miImgProgressShow > 0 )
//	    {
//	    	//Log.e("processUserAction", "" + miImgProgressShow );
//	        miImgProgressShow--;
//	        if( miImgProgressShow == 0 )
//	        {
//	            AnimationDrawable animationDrawable = (AnimationDrawable)mImgProgress.getBackground();
//	            animationDrawable.stop();
//	            mImgProgress.setVisibility( View.GONE );
//	            AnimationDrawable animationDrawableOld=(AnimationDrawable)mImgProgressOld.getBackground();
//	            animationDrawableOld.stop();
//	            mImgProgressOld.setVisibility(View.GONE);
//	            
//	            AnimationDrawable animationDrawableGameOld=(AnimationDrawable)mImgProgressGameOld.getBackground();
//	            animationDrawableGameOld.stop();
//	            mImgProgressGameOld.setVisibility(View.GONE);
//	      
//	            AnimationDrawable animationDrawableMediaOld=(AnimationDrawable)mImgProgressMediaOld.getBackground();
//	            animationDrawableMediaOld.stop();
//	            mImgProgressMediaOld.setVisibility(View.GONE);
//	        }  
//	    }
////	    if( mMenuStatus == UserStatus.MENU_FLOAT_VIEW )
////	    {
////	        if( mIFloatViewCount > 0 )
////	        {
////	            mIFloatViewCount--;
////	        }
////	        else if(mIFloatViewCount == 0 )
////	        {
////	            mMenuFloatView.setVisibility(View.VISIBLE );
////                mMenuFloatView.bringToFront();
////                mIFloatViewCount--;
////	        }
////	    }
//	    
//	    if( mUserActionStatus == UserStatus.USER_ACTION_NONE )
//	        return;
//        if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_UP)
//        {
//            if (miTimeCount > 0)
//            {
//                miTimeCount--;
//            }
//            else
//            {
//                miTimeCount = 0;
//                //mMobileView.invalidateView2(mX2, mY2, false);
//                mUserActionStatus = UserStatus.USER_ACTION_NONE;
//            }
//        }
//        else if( mUserActionStatus == UserStatus.USER_ACTION_TOUCH_UP )
//	    {
//	        if( miTimeCount > 0 )
//	        {
//	            miTimeCount--;
//	        }
//	        else
//	        {
//	            miTimeCount = 0;
//	            mMobileView.invalidateView1(mX1, mY1, false );
//	            //mMobileView.invalidateView2(mX2, mY2, false);
//	            mUserActionStatus = UserStatus.USER_ACTION_NONE;
//	        }
//	    }
//	    else if( mUserActionStatus == UserStatus.USER_ACTION_BTN_UP_DOWN )
//	    {
//	        if( miTimeCount == TIME_GAP )
//            {
//	            btnMoveScreen( 0, 30 );
//            }
//	        miTimeCount--;
//	        if( miTimeCount == 0 )
//	        {
//	            miTimeCount = TIME_GAP;
//	        }
//
//	    }
//	    else if( mUserActionStatus == UserStatus.USER_ACTION_BTN_DOWN_DOWN )
//	    {
//	        if( miTimeCount == TIME_GAP )
//	        {
//	            btnMoveScreen( 0, -30 );
//	        }
//	        miTimeCount--;
//            if( miTimeCount == 0 )
//            {
//                miTimeCount = TIME_GAP;
//            }
//	    }
//	    else if( mUserActionStatus == UserStatus.USER_ACTION_BTN_LEFT_DOWN )
//	    {
//	        if( miTimeCount == TIME_GAP )
//	        {
//	            btnMoveScreen( 30, 0 );
//	        }
//	        miTimeCount--;
//            if( miTimeCount == 0 )
//            {
//                miTimeCount = TIME_GAP;
//            }
//	    }
//	    else if( mUserActionStatus == UserStatus.USER_ACTION_BTN_RIGHT_DOWN )
//	    {
//	        if( miTimeCount == TIME_GAP )
//	        {
//	            btnMoveScreen( -30, 0 );
//	        }
//	        miTimeCount--;
//            if( miTimeCount == 0 )
//            {
//                miTimeCount = TIME_GAP;
//            }
//	    }
//	}  // end of processUserActoin()
//	
//	private void initCPP()
//	{
//		String str14 = getString( R.dimen.bwpx14 );
//		str14 = str14.substring(0, str14.indexOf(".") );
//    	miMoveShreshole = Integer.parseInt( str14 );
//		
//    	String str44 = getString( R.dimen.bwpx44 );
//    	str44 = str44.substring(0, str44.indexOf(".") );
//    	mHeight44 = Integer.parseInt( str44 );
//    	
//    	String str64 = getString( R.dimen.bwpx64 );
//    	str64 = str64.substring(0, str64.indexOf(".") );
//    	GlobalFun.BWStartPoint = Integer.parseInt( str64 );
//    	
//    	String maxCpu = this.getMaxCpuFreq();
//    	double dMaxCpu = Double.parseDouble( maxCpu );
//    	double temp = dMaxCpu;
//    	String suffix = "KHz";
//
//    	if(dMaxCpu >= 1000)
//    	{
//    		temp = dMaxCpu / 1000.0;
//	    	suffix = "MHz";
//	    	if(temp >=1000)
//	    	{
//	    		suffix = "GHz";
//	    		temp /=1000.0;
//	    	}
//    	}
//    	
//    	java.text.DecimalFormat df = new java.text.DecimalFormat("#0.#");
//    	StringBuilder result = new StringBuilder(df.format(temp));
//    	result.append(suffix);
////      /CPU:1.2GHz,RAM:800M  按照这种方式
//        // /CPU:1.2GHz,RAM:800MB  目前是这种格式
////    	String ram = this.getTotalMemory();
//    	mCpu =  result.toString();
//    	mRam = this.getTotalMemory();
//    	
//    	nativeInit(mScreenWidth, mScreenHeight, android.os.Build.VERSION.SDK_INT, mHeight44);
//	}
//		
//	public void loadMenuBrowserPhone() //浏览器菜单
//	{
//	    mMenuBrowser = findViewById(R.id.menubrowser);
//	    mMenuBrowserPop = findViewById(R.id.mobilemenupop);
//	    mBtnMenuHome = (Button)findViewById( R.id.menu_home);
//	    btnMenuHomeEnabled( false );
//	    mBtnMenuHome.setOnClickListener(new OnClickListener()
//	    {
//	        public void onClick(View v)
//	        {
//	            HideMenuPopMore();
//	            mMobileView.onDomHome();
//	            btnMenuHomeEnabled( ! mMobileView.isCurrentHomePage() );
//	    	    btnMenuForwardEnabled( mMobileView.canForward() );
//	            btnMenuBackEnabled( mMobileView.canBack() );
//	        }
//	    });
//	
//	    mBtnMenuLocal = (Button)findViewById( R.id.menu_local);
//	    mBtnMenuLocal.setOnClickListener(new OnClickListener()
//	    {
//	        public void onClick(View v)
//	        {
//	            HideMenuPopMore();
//	            MobileNative();
//	        }
//	    });
//	
//	    mBtnMenuBack = (Button)findViewById( R.id.menu_back );
//	    btnMenuBackEnabled( false );
//	    mBtnMenuBack.setOnClickListener(new OnClickListener()
//	    {
//	        public void onClick(View v)
//	        {
//	            HideMenuPopMore();
//	            if(  mMenuStatus ==  UserStatus.MENU_LOADING )
//	            {
//	                stopProgressLoading();
//	            }
//	            else
//	            {
//	                mMobileView.onDomBack();
//	                btnMenuHomeEnabled( ! mMobileView.isCurrentHomePage() );
//	        	    btnMenuForwardEnabled( mMobileView.canForward() );
//	                btnMenuBackEnabled( mMobileView.canBack() );
//	            }
//	        }
//	    });
//	
//	    mBtnMenuRefresh = (Button) findViewById( R.id.menu_fresh );
//	    mBtnMenuRefresh.setOnClickListener(new OnClickListener()
//	    {
//	        public void onClick(View v)
//	        {
//	            HideMenuPopMore();
//	            mMobileView.onDomUpdate();
//	        }
//	    });
//	
//	    mAniEnter = AnimationUtils.loadAnimation(this, R.anim.popup_enter);
//	    mAniExit = AnimationUtils.loadAnimation(this, R.anim.popup_exit);
//	    mBtnMenuMore = (Button) findViewById(R.id.menu_more);
//	    mBtnMenuMore.setOnClickListener(new OnClickListener()
//	    {
//	        public void onClick(View v)
//	        {
//	            if ( mMenuStatus == UserStatus.MENU_BROWSER )
//	            {
//	                ShowMenuPopMore();
//	            }
//	            else if( mMenuStatus == UserStatus.MENU_BROWSER_POP )
//	            {
//	                HideMenuPopMore();
//	            }
//	
//	        }
//	    });
//	    
//	    final Button btnMenuMouse = ( Button ) findViewById( R.id.menu_mouse );
//	    btnMenuMouse.setOnClickListener( new OnClickListener()
//	    {
//	        public void onClick( View v )
//	        {
//	            SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//	            mbShowMouseSign = settings.getBoolean("USER_MOUSE", true );
//	            mbShowMouseSign = !mbShowMouseSign;
//	            SharedPreferences.Editor editor = settings.edit();
//	            editor.putBoolean("USER_MOUSE", mbShowMouseSign );
//	            editor.commit();
//	            if( mbShowMouseSign )
//	            {
//	                btnMenuMouse.setBackgroundResource( R.drawable.menu_mouse); 
//	            }
//	            else
//	            {
//	                btnMenuMouse.setBackgroundResource( R.drawable.menu_mouse_off );
//	            }
//	        }
//	    });
//	    
//	    Button btnMenuEliminate = ( Button )findViewById( R.id.menu_elimiate );
//	    btnMenuEliminate.setOnClickListener( new OnClickListener()
//	    {
//	        public void onClick( View v )
//	        {
//	            HideMenuPopMore();
//	            ClearBuffer();
//	        }
//	    });
//	    
//	
//	    Button btnMenuFeedback = (Button)findViewById( R.id.menu_feedback );
//	    btnMenuFeedback.setOnClickListener(new OnClickListener()
//	    {
//	        public void onClick(View v)
//	        {
//	            HideMenuPopMore();
//	            mMobileView.openFeedbackView();
//	        }
//	    });
//	    
//	//	        Button btnMenuBrowserSeting = (Button)findViewById( R.id.menu_browser_setting );
//	//	        btnMenuBrowserSeting.setOnClickListener(new OnClickListener()
//	//	        {
//	//	            public void onClick(View v)
//	//	            {
//	//	                HideMenuPopMore();
//	//	                showSettingFormBrowser();
//	//	                mMenuStatus = UserStatus.MENU_SETTING_BROWSER;
//	//	            }
//	//	        });
//	    
//	    Button btnMenuHelp = (Button)findViewById( R.id.menu_apphelp );
//	    btnMenuHelp.setOnClickListener(new OnClickListener()
//	    {
//	        public void onClick(View v)
//	        {
//	            HideMenuPopMore();
//	            HelpMsg();
//	        }
//	    });
//	
//	    Button btnMenuAbout = (Button)findViewById( R.id.menu_about );
//	    btnMenuAbout.setOnClickListener(new OnClickListener()
//	    {
//	        public void onClick(View v)
//	        {
//	            HideMenuPopMore();
//	            AboutUs();
//	        }
//	    });
//	    
//	    Button btnMenuExit = (Button)findViewById( R.id.menu_exit );
//	    if( btnMenuExit != null )
//	    {
//	        btnMenuExit.setOnClickListener( new OnClickListener()
//	        {
//	        	public void onClick( View v )
//	        	{
//	        		HideMenuPopMore();
//	        		AppQuit();
//	        	}
//	        });
//	    }
//	    mProgressLoading = (ProgressBar) findViewById(R.id.menu_loading_progress);
//	}
//	
//    public void loadMenuDirection()  //方向建菜单
//    {
//        mMenuDirection = findViewById( R.id.menudirection );
//        Button btnMenuDirectionUp = ( Button )findViewById( R.id.menu_direction_up );
//        btnMenuDirectionUp.setOnTouchListener(new OnTouchListener()
//        {
//             public boolean onTouch(View v, MotionEvent event)
//            {
//                if (event.getAction() == MotionEvent.ACTION_DOWN)
//                {
//                    mUserActionStatus = UserStatus.USER_ACTION_BTN_UP_DOWN;
//                    miTimeCount = TIME_GAP;
//                }
//                else if (event.getAction() == MotionEvent.ACTION_UP)
//                 {
//                    mUserActionStatus = UserStatus.USER_ACTION_BTN_UP_UP;
//                    miTimeCount = 0;
//                 }
//                 return false;
//             }
//        });
//
//        Button btnMenuDirectionDown = (Button)findViewById( R.id.menu_direction_down );
//        btnMenuDirectionDown.setOnTouchListener( new OnTouchListener()
//        {
//            public boolean onTouch( View v, MotionEvent event )
//            {
//                if( event.getAction() == MotionEvent.ACTION_DOWN )
//                {
//                    mUserActionStatus = UserStatus.USER_ACTION_BTN_DOWN_DOWN;
//                    miTimeCount = TIME_GAP;
//                }
//                else if( event.getAction() == MotionEvent.ACTION_UP )
//                {
//                    mUserActionStatus = UserStatus.USER_ACTION_BTN_DOWN_UP;
//                    miTimeCount = 0;
//                }
//                return false;
//            }
//        });
//        Button btnMenuDirectionLeft = ( Button )findViewById( R.id.menu_direction_left );
//        btnMenuDirectionLeft.setOnTouchListener( new OnTouchListener()
//        {
//            public boolean onTouch( View v, MotionEvent event )
//            {
//                if( event.getAction() == MotionEvent.ACTION_DOWN )
//                {
//                    mUserActionStatus = UserStatus.USER_ACTION_BTN_LEFT_DOWN;
//                    miTimeCount = TIME_GAP;
//                }
//                else if( event.getAction() == MotionEvent.ACTION_UP )
//                {
//                    mUserActionStatus = UserStatus.USER_ACTION_BTN_LEFT_UP;
//                    miTimeCount = 0;
//                }
//                return false;
//            }
//        });
//        
//        Button btnMenuDirectionRight = (Button)findViewById( R.id.menu_direction_right);
//        btnMenuDirectionRight.setOnTouchListener( new OnTouchListener()
//        {
//            public boolean onTouch( View v, MotionEvent event )
//            {
//                if( event.getAction() == MotionEvent.ACTION_DOWN )
//                {
//                    mUserActionStatus = UserStatus.USER_ACTION_BTN_RIGHT_DOWN;
//                    miTimeCount = TIME_GAP;
//                }
//                else if( event.getAction() == MotionEvent.ACTION_UP )
//                {
//                    mUserActionStatus = UserStatus.USER_ACTION_BTN_RIGHT_UP;
//                    miTimeCount = 0;
//                }
//                return false;
//            }
//        });
//    }
//    
//    public void loadMenuGameOld()//旧游戏菜单事件处理
//    {
//    	mMenuGameOld=findViewById(R.id.menugame);
//    	mImgProgressGameOld=(ImageView)findViewById(R.id.img_progress_game_old);
//    	mImgProgressGameOld.setBackgroundResource(R.anim.img_progress);
//    	mMenuGameOld.setOnLongClickListener(new View.OnLongClickListener() {
//			
//			public boolean onLongClick(View v) {
//				// TODO Auto-generated method stub
//				if(mbMenuGameOldMove)
//				{
//					return false;
//				}
//				return false;
//			}
//		});
//    	mMenuGameOld.setOnTouchListener(new OnTouchListener() {
//			
//			public boolean onTouch(View v, MotionEvent event) {
//				// TODO Auto-generated method stub
//				mMenuGameOldHeight=mMenuGameOld.getHeight();
//				int ea=event.getAction();
//				switch(ea)
//				{
//					case MotionEvent.ACTION_DOWN:
//						mbMenuGameOldMove=false;
//						mMenuGameOldMoveY=(int)event.getRawY();
//						break;
//					case MotionEvent.ACTION_MOVE:
//						if(GlobalFun.showMenu)
//						{
//							int dy=(int)event.getRawY()-mMenuGameOldMoveY;
//							if (Math.abs(dy) > miMoveShreshole) //
//							{
//								mbMenuGameOldMove=true;
//							} 
//							else
//							{
//								break;
//							}
//						}
//						break;
//					case MotionEvent.ACTION_UP:
//						if(mbMenuGameOldMove)
//						{	
//							
//							mbMenuGameOldMove=false;
//							int posx=(int)event.getRawX();
//							int posy=(int)event.getRawY();
//							SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//							Editor editor = settings.edit();//获取编辑器
//							editor.putBoolean("showOldMenu", false);
//							editor.putInt("USER_POS_X", posx);
//							editor.putInt("USER_POS_Y", posy);
//							editor.commit();
//							HideMenu();
//							mMenuGameOld.setVisibility(View.GONE);
////							mMobileView.setTopPos( 0 );
//							mMobileView.setBottomPos(mScreenHeight);
//							showMenuFloatView();
//							
//						}
//						break;
//				
//				}
//				return false;
//			}
//		});
//    	 mBtnGameSwitchOld=(Button)findViewById(R.id.menu_switch_game_old);
//    	 mBtnGameSwitchOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//				Editor editor = settings.edit();//获取编辑器
//				editor.putBoolean("showOldMenu", false);
//				editor.putInt("USER_POS_X", 20);
//				editor.putInt("USER_POS_Y", 20);
//				editor.commit();
//				boolean firstSwitchToNew =settings.getBoolean("firstSwitchToNew", true);
//				if(firstSwitchToNew)
//				{
//					displayTips("可通过向下滑动菜单进行切换");
//					editor.putBoolean("firstSwitchToNew", false);
//					editor.commit();
//				}
//				HideMenu();
//				mMenuGameOld.setVisibility(View.GONE);
////				mMobileView.setTopPos( 0 );
//				mMobileView.setBottomPos(mScreenHeight);
//				showMenuFloatView();
//			}
//		});
//    	 mBtnGameZoomOld=(Button)findViewById(R.id.menu_zooming_game_old);
//    	 mBtnGameZoomOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				switchZooming();
//				
//			}
//		});
//    	 mBtnGameSettingOld=(Button)findViewById(R.id.menu_setting_old);
//    	 mBtnGameSettingOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				 mMenuStatus = UserStatus.MENU_SETTING_FLASH;
//	             showSettingFromFlash();
//	             mMobileView.setBottomPos(mScreenHeight);
//	             mMenuGameOld.setVisibility(View.GONE);
//				
//			}
//		});
//    	 mBtnGameSaveOld=(Button)findViewById(R.id.menu_save_game_old);
//    	 mBtnGameSaveOld.setEnabled(false);
//    	 mBtnGameSaveOld.setBackgroundResource(R.drawable.bar_save_untouch_old);
//    	 mBtnGameSaveOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				saveMediaFile();
//			}
//		});
//    	 mBtnGameHelpOld=(Button)findViewById(R.id.menu_gamehelp_old);
//    	 mBtnGameHelpOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				
//                     mViewPopStatus = UserStatus.VIEW_POP_HELP;
//                     mMobileView.setBottomPos(mScreenHeight);
//                     mMobileView.switchHelp();
//                     mMenuGameOld.setVisibility(View.GONE);
//                     GlobalFun.showMenu=false;
//                     
//			}
//		});
//    	 mBtnGameCloseOld=(Button)findViewById(R.id.menu_close_game_old);
//    	 mBtnGameCloseOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				menuExitFlash();
//			}
//		});
//    }
//    
//    public void loadMenuWebGameOld()
//    {
//    	mImgProgressOld=(ImageView)findViewById(R.id.img_progress_old);
//    	mImgProgressOld.setBackgroundResource(R.anim.img_progress);
//    	timeWebGame=(TextView)findViewById(R.id.time_webgame);
////    	mCalendar=Calendar.getInstance();
////    	setFormat();
////        mCalendar.setTimeInMillis(System.currentTimeMillis());
////    	  timeWebGame.setText(DateFormat.format(mFormat, mCalendar));
//    	 Time time = new Time();    
//         time.setToNow();    
//         int minute = time.minute;   
//         int hour = time.hour;   
//         int sec = time.second;  
//        timeWebGame.setText(format(hour)+":"+format(minute)+":"+format(sec));
//        
//    	mMenuWebGameOld=findViewById(R.id.menuwebgame);	  	
//    	mMenuWebGameOld.setOnLongClickListener(new View.OnLongClickListener() {
//			
//			public boolean onLongClick(View v) {
//				// TODO Auto-generated method stub
//				if(mbMenuWebGameOldMove)
//				{
//					return false;
//				}
//				return false;
//			}
//		});
//    	mMenuWebGameOld.setOnTouchListener(new OnTouchListener() {
//			
//			public boolean onTouch(View v, MotionEvent event) {
//				// TODO Auto-generated method stub
//				int ea=event.getAction();
//				switch(ea)
//				{
//				case MotionEvent.ACTION_DOWN:
//					mbMenuWebGameOldMove=false;
//					mMenuWebGameOldMoveY=(int)event.getRawY();
//					break;
//				case MotionEvent.ACTION_MOVE:
//					if(GlobalFun.showMenu)
//					{
//						int dy=(int)event.getRawY()-mMenuWebGameOldMoveY;
//						if(Math.abs(dy)>miMoveShreshole)
//						{
//							mbMenuWebGameOldMove=true;
//						}
//						else
//						{
//							break;
//						}
//					}
//					break;
//				case MotionEvent.ACTION_UP:
//					if(mbMenuWebGameOldMove)
//					{
//						mbMenuWebGameOldMove=false;
//						int posx=(int)event.getRawX();
//						int posy=(int)event.getRawY();
//						SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//						Editor editor = settings.edit();//获取编辑器
//						editor.putBoolean("showOldMenu", false);
//						editor.putInt("USER_POS_X", posx);
//						editor.putInt("USER_POS_Y", posy);
//						editor.commit();
//						HideMenu();
//						mMenuWebGameOld.setVisibility(View.GONE);
////						mMobileView.setTopPos( 0 );
//						mMobileView.setBottomPos(mScreenHeight);
//						showMenuFloatView();
//					}
//					break;
//				}
//				return false;
//			}
//		});
//    	mBtnWebGameSwitchOld=(Button)findViewById(R.id.menu_switch_webgame_old);
//    	mBtnWebGameSwitchOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//				Editor editor = settings.edit();//获取编辑器
//				editor.putBoolean("showOldMenu", false);
//				editor.putInt("USER_POS_X", 20);
//				editor.putInt("USER_POS_Y", 20);
//				editor.commit();
//				boolean firstSwitchToNew =settings.getBoolean("firstSwitchToNew", true);
//				if(firstSwitchToNew)
//				{
//					displayTips("可通过向下滑动菜单进行切换");
//					editor.putBoolean("firstSwitchToNew", false);
//					editor.commit();
//				}
//				HideMenu();
//				mMenuWebGameOld.setVisibility(View.GONE);
////				mMobileView.setTopPos( 0 );
//				mMobileView.setBottomPos(mScreenHeight);
//				showMenuFloatView();
//			}
//		});
//    	mBtnWebGameZoomOld=(Button)findViewById(R.id.menu_zooming_webgame_old);
//    	mBtnWebGameZoomOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				switchZooming();
//			}
//		});
//    	mBtnWebGameSettingOld=(Button)findViewById(R.id.menu_setting_webgame_old);
//    	mBtnWebGameSettingOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				 mMenuStatus=UserStatus.MENU_SETTING_FLASH;
//	             showSettingFromFlash();
//	             mMobileView.setBottomPos(mScreenHeight);
//	             mMenuWebGameOld.setVisibility(View.GONE);
//			}
//		});
//    	mBtnWebGameRefreshOld=(Button)findViewById(R.id.menu_fresh_webgame_old);
//    	mBtnWebGameRefreshOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				 Dialog dlg = new AlertDialog.Builder(MobileMain_maopao.this)
//               .setIcon(R.drawable.logo_36)
//               .setTitle("刷新将重新加载游戏，建议您在游戏无法正常加载时使用；是否刷新？")
//               .setPositiveButton("确定", new DialogInterface.OnClickListener()
//               {
//                   public void onClick(DialogInterface dialog, int whichButton)
//                   {
//                       mMobileView.flashRestart();
//                   }
//               }).setNegativeButton("取消", new DialogInterface.OnClickListener(){
//                   public void onClick(DialogInterface dialog, int which) {
//                	   mMobileView.setSilence( false );
//                   }
//               })
//               .create();
//               dlg.show();
//			}
//		});
//    	mBtnWebGameRechargeOld=(Button)findViewById(R.id.menu_recharge_webgame_old);
////    	if(!mMobileView.canPayWindow())
////    	{
////    		mBtnWebGameRechargeOld.setEnabled(false);
////    		mBtnWebGameRechargeOld.setBackgroundResource(R.drawable.bar_recharge_webgame_untouch_old);
////    	}
////    	else
////    	{
////    		mBtnWebGameRechargeOld.setEnabled(true);
////    		mBtnWebGameRechargeOld.setBackgroundResource(R.drawable.menu_recharge_webgame_old);
////    	}
//    	mBtnWebGameRechargeOld.setOnClickListener(new OnClickListener() 
//    	{
//			public void onClick(View v) 
//			{
//				// TODO Auto-generated method stub
//				if( mMobileView.canPayWindow())
//				{
//					mMenuWebGameOld.setVisibility(View.GONE);
//			        GlobalFun.showMenu=false;
//					mViewPopStatus = UserStatus.VIEW_POP_PAY;
//		            mMobileView.openPayWindow();
//		            mMobileView.setBottomPos(mScreenHeight);
//				}
//			}
//		});
//    	mBtnWebGameCloseOld=(Button)findViewById(R.id.menu_close_webgame_old);
//    	mBtnWebGameCloseOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				menuExitFlash();
//			}
//		});
//    }
//	
//    public void loadMenuMediaOld()
//    {
//    	mMenuMediaOld=findViewById(R.id.menumedia);
//    	mImgProgressMediaOld=(ImageView)findViewById(R.id.img_progress_media_old);
//    	mImgProgressMediaOld.setBackgroundResource(R.anim.img_progress);
//    	mMenuMediaOld.setOnLongClickListener(new View.OnLongClickListener() {
//			
//			public boolean onLongClick(View v) {
//				// TODO Auto-generated method stub
//				if(mbMenuMediaOldMove)
//				{
//					return false;
//				}
//				return false;
//			}
//		});
//    	mMenuMediaOld.setOnTouchListener(new OnTouchListener() {
//			
//			public boolean onTouch(View v, MotionEvent event) {
//				// TODO Auto-generated method stub
//				int ea=event.getAction();
//				switch(ea)
//				{
//					case MotionEvent.ACTION_DOWN:
//						mbMenuMediaOldMove=false;
//						mMenuMediaOldMoveY=(int)event.getRawY();
//						break;
//					case MotionEvent.ACTION_MOVE:
//						if(GlobalFun.showMenu)
//						{
//							int dy=(int)event.getRawY()-mMenuMediaOldMoveY;
//							if(Math.abs(dy)>miMoveShreshole)
//							{
//								mbMenuMediaOldMove=true;
//							}
//							else
//							{
//								break;
//							}
//						}
//						break;
//					case MotionEvent.ACTION_UP:
//						if(mbMenuMediaOldMove)
//						{
//							mbMenuMediaOldMove=false;
//							int posx=(int)event.getRawX();
//							int posy=(int)event.getRawY();
//							SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//							Editor editor = settings.edit();//获取编辑器
//							editor.putBoolean("showOldMenu", false);
//							editor.putInt("USER_POS_X", posx);
//							editor.putInt("USER_POS_Y", posy);
//							editor.commit();
//							HideMenu();
//							mMenuMediaOld.setVisibility(View.GONE);
////							mMobileView.setTopPos( 0 );
//							mMobileView.setBottomPos(mScreenHeight);
//							showMenuFloatView();
//						}
//						break;
//				}
//				return false;
//			}
//		});
//    	mMenuMediaSwitchOld=(Button)findViewById(R.id.menu_switch_media_old);
//    	mMenuMediaSwitchOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//				Editor editor = settings.edit();//获取编辑器
//				editor.putBoolean("showOldMenu", false);
//				editor.putInt("USER_POS_X", 20);
//				editor.putInt("USER_POS_Y", 20);
//				editor.commit();
//				boolean firstSwitchToNew =settings.getBoolean("firstSwitchToNew", true);
//				if(firstSwitchToNew)
//				{
//					displayTips("可通过向下滑动菜单进行切换");
//					editor.putBoolean("firstSwitchToNew", false);
//					editor.commit();
//				}
//				HideMenu();
//				mMenuMediaOld.setVisibility(View.GONE);
////				mMobileView.setTopPos( 0 );
//				mMobileView.setBottomPos(mScreenHeight);
//				showMenuFloatView();
//			}
//		});
//    	mMenuMediaZoomOld=(Button)findViewById(R.id.menu_zooming_media_old);
//    	mMenuMediaZoomOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				switchZooming();
//			}
//		});
//    	mMenuMediaSettingOld=(Button)findViewById(R.id.menu_setting_media_old);
//    	mMenuMediaSettingOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				  mMenuStatus=UserStatus.MENU_SETTING_FLASH;
//				  showSettingFromFlash();
//				  mMobileView.setBottomPos(mScreenHeight);
//				  mMenuMediaOld.setVisibility(View.GONE);
//			}
//		});
//    	mMenuMediaSaveOld=(Button)findViewById(R.id.menu_save_media_old);
//    	mMenuMediaSaveOld.setEnabled(false);
//    	mMenuMediaSaveOld.setBackgroundResource(R.drawable.bar_save_untouch_old);
//    	mMenuMediaSaveOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				saveMediaFile();
//			}
//		});
//    	mMenuMediaCloseOld=(Button)findViewById(R.id.menu_close_media_old);
//    	mMenuMediaCloseOld.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				menuExitFlash();
//			}
//		});
//    }
//    
////	public void loadMenuFloatView() 
////	{
////		mMenuFloatView = findViewById(R.id.absolute_view);
////		mLinearView = findViewById(R.id.float_linear_view);
////		timeWebGameFloat=(TextView)findViewById(R.id.time_float_webgame);
//////		mCalendarFloat=Calendar.getInstance();
//////		setFloatFormat();
//////		mCalendarFloat.setTimeInMillis(System.currentTimeMillis());
//////		timeWebGameFloat.setText(DateFormat.format(mFormat_float, mCalendarFloat));
////		
////		 Time time = new Time();    
////         time.setToNow();    
////         int minute = time.minute;   
////         int hour = time.hour;   
////         if(showTimeDot)
////         {
////	         timeWebGameFloat.setText(format(hour)+":"+format(minute));
////	         showTimeDot=false;
////         }
////         else
////         {
////        	 timeWebGameFloat.setText(format(hour)+" "+format(minute));
////        	 showTimeDot=true;
////         }
////
////		
////		mFrameMore = (FrameLayout)findViewById( R.id.frame_float_more);
////		mFrameMore.setOnLongClickListener(new View.OnLongClickListener()
////		{
////			public boolean onLongClick(View v) 
////			{
////				if (mbFloatViewMove)
////				{
////					return false;
////				}
////				return false;
////			}
////		});
////		mFrameMore.setOnTouchListener(new OnTouchListener() 
////		{
////			public boolean onTouch(View v, MotionEvent event) 
////			{
////				mBtnFloatImgWidth = mLinearView.getWidth() / 2;
////				mBtnFloatImgHeight = mLinearView.getHeight();
////				int ea = event.getAction();
////				switch (ea)
////				{
////				case MotionEvent.ACTION_DOWN:
////					mbFloatViewMove = false;
////					mMoveX = (int) event.getRawX();
////					mMoveY = (int) event.getRawY();
////					break;
////				case MotionEvent.ACTION_MOVE:
////					int dx = (int) event.getRawX() - mMoveX;
////					int dy = (int) event.getRawY() - mMoveY;
////					if (Math.abs(dx) > miMoveShreshole || Math.abs(dy) > miMoveShreshole) 
////					{
////						mbFloatViewMove = true;
////					} 
////					else
////					{
////						break;
////					}
////					int left = mLinearView.getLeft() + dx;
////					int top = mLinearView.getTop() + dy;
////					
////					setMenuFloatViewPos(left, top);
////					mMoveX = (int) event.getRawX();
////					mMoveY = (int) event.getRawY();
////					break;
////				case MotionEvent.ACTION_UP:
////					// Log.e("------->pos", "x:" + mMoveX + ", y:" + mMoveY );
////					if (mbFloatViewMove)
////					{
////						mbFloatViewMove = false;
////						if(mbExchangeMenu)
////						{
////							mbExchangeMenu=false;
//////							mMobileView.setTopPos( mHeight44 );
////							mMobileView.setBottomPos(mScreenHeight-mHeight44);
////							mMenuFloatView.setVisibility(View.GONE);
////							if(mUserStatus==UserStatus.FLASH_TYPE_GAME)
////							{
////								showMenuGameOld();
////							}
////							else if(mUserStatus==UserStatus.FLASH_TYPE_MEDIA)
////							{
////								showMenuMediaOld();
////							}
////							else if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
////							{
////								showMenuWebGameOld();
////							}
////							SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
////							Editor editor=settings.edit();
////							editor.putBoolean("showOldMenu", true);
////							editor.commit();//提交修改
////						}
////						else
////						{
////						
////						SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
////						SharedPreferences.Editor editor = settings.edit();
////						editor.putInt("USER_POS_X", mMoveX - mBtnFloatImgWidth / 2 );
////	                    editor.putInt("USER_POS_Y", mMoveY -mBtnFloatImgHeight / 2 );
////						editor.commit();
////						}
////					}
////					else
////					{
////						changeUserMenu();
////					}
////					break;
////				}
////				return false;
////			}
////		});
////
////		mFrameZoom = (FrameLayout) findViewById(R.id.frame_float_zoom);
////		mImgProgress = (ImageView) findViewById(R.id.img_progress);
////		mImgProgress.setBackgroundResource(R.anim.img_progress);
////		
////		mFrameZoom.setOnLongClickListener(new View.OnLongClickListener() 
////		{
////			public boolean onLongClick(View v) 
////			{
////				if (mbFloatViewMove) 
////				{
////					return false;
////				}
////				return false;
////			}
////		});
////		mFrameZoom.setOnTouchListener(new OnTouchListener()
////        {  
////            public boolean onTouch(View v, MotionEvent event)
////            {   
////            	mBtnFloatImgWidth = mLinearView.getWidth() / 2;
////        		mBtnFloatImgHeight = mLinearView.getHeight();
////                int ea = event.getAction();  
////                switch(ea)
////                {  
////                case MotionEvent.ACTION_DOWN:  
////                    //Log.e("-------->", "mFrameZoom  ACTION_DOWN");
////                    mbFloatViewMove = false;
////                    mMoveX = (int) event.getRawX();  
////                    mMoveY = (int) event.getRawY();  
////                    break;  
////                case MotionEvent.ACTION_MOVE: 
////                    //Log.e("-------->", "mFrameZoom  ACTION_MOVE");
////                    int dx =(int)event.getRawX() - mMoveX;  
////                    int dy =(int)event.getRawY() - mMoveY; 
////                    if( Math.abs( dx ) > miMoveShreshole || Math.abs( dy ) > miMoveShreshole )
////                    {
////                        mbFloatViewMove = true;
////                    }
////                    else
////                    {
////                        break;  
////                    }
////                    int left = mLinearView.getLeft() + dx;  
////                    int top = mLinearView.getTop() + dy;  
////                    
////                    setMenuFloatViewPos( left, top );
////                    mMoveX = (int) event.getRawX();  
////                    mMoveY = (int) event.getRawY(); 
////                
////                    break;  
////                case MotionEvent.ACTION_UP: 
////                    if( mbFloatViewMove )
////                    {
////                    	mbFloatViewMove = false;
////                        if(mbExchangeMenu)
////						{
////							mbExchangeMenu=false;
//////							mMobileView.setTopPos( mHeight44 );
////							mMobileView.setBottomPos(mScreenHeight-mHeight44);
////							mMenuFloatView.setVisibility(View.GONE);
////							if(mUserStatus==UserStatus.FLASH_TYPE_GAME)
////							{
////								showMenuGameOld();
////							}
////							else if(mUserStatus==UserStatus.FLASH_TYPE_MEDIA)
////							{
////								showMenuMediaOld();
////							}
////							else if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
////							{
////								showMenuWebGameOld();
////							}
////							SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
////							Editor editor=settings.edit();
////							editor.putBoolean("showOldMenu", true);
////							editor.commit();//提交修改
////						}
////                        else
////                        {
////                        SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
////                        SharedPreferences.Editor editor = settings.edit();
////                        editor.putInt("USER_POS_X", mMoveX - 3 * mBtnFloatImgWidth / 2 );
////                        editor.putInt("USER_POS_Y", mMoveY - mBtnFloatImgHeight / 2 );
////                        editor.commit();
////                        }
////                    }
////                    else
////                    {
////                        switchZooming( );
////                    }
////                    break;  
////                }  
////                return false;  
////            }
////        });  
////		//mBtnFloatImgWidth = mLinearView.getWidth() / 2;
////		//mBtnFloatImgHeight = mLinearView.getHeight();
////		SharedPreferences settings = getSharedPreferences( GlobalFun.BW_CONFIGURE_FILE, 0);
////		int iposx = settings.getInt("USER_POS_X", 20);
////		int iposy = settings.getInt("USER_POS_Y", 20);
////		setMenuFloatViewPos(iposx, iposy);
////		return;
////	}
//    
//    public void loadMenuFloatGame()
//    {
//        mMenuFloatGame = findViewById( R.id.menu_float_game );
//        
//        mBtnGameSave = (Button)findViewById( R.id.btn_game_save );
//        mBtnGameSave.setEnabled( false );
//        mBtnGameSave.setBackgroundResource( R.drawable.bar_save_untouch );
//        mBtnGameSave.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                //mMobileView.flashResume();
//                hideMenuAnimation(mMenuFloatGame);
////                showMenuFloatView();
//                Message msgTime = new Message();
//        	    msgTime.what = MSG_SHOWFLOATVIEW;
//                mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//                saveMediaFile();
//            }
//        });
//        mBtnGameSetting = ( Button )findViewById( R.id.btn_game_setting );
//        mBtnGameSetting.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                mMenuFloatGame.setVisibility( View.GONE );                
//                mMenuStatus = UserStatus.MENU_SETTING_FLASH;
//                showSettingFromFlash();
//            }
//        });
//        
//        mBtnGameFloatBack = (Button)findViewById( R.id.btn_game_back );
//        mBtnGameFloatBack.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                //mMobileView.flashResume();
//                hideMenuAnimation(mMenuFloatGame);
////                showMenuFloatView();
//                Message msgTime = new Message();
//        	    msgTime.what = MSG_SHOWFLOATVIEW;
//                mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//            }
//        });
//        
//        mBtnGameFloatHelp = ( Button )findViewById( R.id.btn_game_help );
//        mBtnGameFloatHelp.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                //mMobileView.flashResume();
//                mViewPopStatus = UserStatus.VIEW_POP_HELP;
//                mMobileView.switchHelp();
//                hideMenuAnimation(mMenuFloatGame);
////                showMenuFloatView();
//            }
//        });
//        
//        mBtnGameFloatClose = ( Button)findViewById( R.id.btn_game_close );
//        mBtnGameFloatClose.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                //mMobileView.flashResume();
//                //hideMenuAnimation(mMenuFloatGame);
//                //showMenuFloatView();
//                menuExitFlash();
//            }
//        });
////        mBtnGameFloatSwitch=(Button)findViewById(R.id.btn_game_switch_to_old);
////        mBtnGameFloatSwitch.setOnClickListener(new OnClickListener() {
////			
////			@Override
////			public void onClick(View v) {
////				// TODO Auto-generated method stub
////				 hideMenuAnimation(mMenuFloatGame);
////				 showMenuGameOld();
////				 SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
////				 Editor editor = settings.edit();//获取编辑器
////				 editor.putBoolean("showOldMenu", true);
////				 editor.commit();//提交修改
////				 boolean firstSwitchToOld =settings.getBoolean("firstSwitchToOld", true);
////				 if(firstSwitchToOld)
////					{
////						displayTips("可通过滑动浮动菜单到屏幕顶部进行切换");
////						editor.putBoolean("firstSwitchToOld", false);
////						editor.commit();
////					}
////			}
////		});
//        return;
//    }
//
//    public void loadMenuFloatMedia()
//    {
//        mMenuFloatMedia = findViewById( R.id.menu_float_media );
//        mBtnMediaSave = ( Button )findViewById( R.id.btn_media_save );
//        mBtnMediaSave.setEnabled( false );
//        mBtnMediaSave.setBackgroundResource( R.drawable.bar_save_untouch );
//        mBtnMediaSave.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                //mMobileView.flashResume();
//                hideMenuAnimation(mMenuFloatMedia);
//                //showMenuFloatView();
//                Message msgTime = new Message();
//        	    msgTime.what = MSG_SHOWFLOATVIEW;
//                mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//                saveMediaFile();
//            }
//        });
//        
//        mBtnMediaSetting = ( Button )findViewById( R.id.btn_media_setting );
//        mBtnMediaSetting.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                mMenuFloatMedia.setVisibility( View.GONE );
//                mMenuStatus = UserStatus.MENU_SETTING_FLASH;
//                showSettingFromFlash();
//            }
//        });
//        
//        mBtnMediaBack = (Button)findViewById( R.id.btn_media_back );
//        mBtnMediaBack.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                hideMenuAnimation(mMenuFloatMedia);
//                //showMenuFloatView();
//                Message msgTime = new Message();
//        	    msgTime.what = MSG_SHOWFLOATVIEW;
//                mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//                //mMobileView.flashResume();
//            }
//        });
//        
//        mBtnMediaClose = ( Button)findViewById( R.id.btn_media_close );
//        mBtnMediaClose.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                //mMobileView.flashResume();
//                //hideMenuAnimation(mMenuFloatMedia);
//                //showMenuFloatView();
//                menuExitFlash();
//            }
//        });
////        mBtnMediaFloatSwitch=(Button)findViewById(R.id.btn_media_switch_to_old);
////        mBtnMediaFloatSwitch.setOnClickListener(new OnClickListener() {
////			
////			@Override
////			public void onClick(View v) {
////				// TODO Auto-generated method stub
////			hideMenuAnimation(mMenuFloatMedia);
////			showMenuMediaOld();
////			SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
////			 Editor editor = settings.edit();//获取编辑器
////			 editor.putBoolean("showOldMenu", true);
////			 editor.commit();//提交修改
////			 boolean firstSwitchToOld =settings.getBoolean("firstSwitchToOld", true);
////			 if(firstSwitchToOld)
////				{
////					displayTips("可通过滑动浮动菜单到屏幕顶部进行切换");
////					editor.putBoolean("firstSwitchToOld", false);
////					editor.commit();
////				}
////			}
////		});
//        
//        return;
//    }
//
//    public void loadMenuFloatWebGame()
//    {
//        
//        mMenuFloatWebGame = findViewById( R.id.menu_float_webgame );
//        
//      mBntWebGameRecharge = ( Button )findViewById( R.id.btn_webgame_recharge );
////      if( ! mMobileView.canPayWindow() )
////      {
////          mBntWebGameRecharge.setEnabled( false );
////          mBntWebGameRecharge.setBackgroundResource( R.drawable.bar_recharge_untouch );
////      }
//      mBntWebGameRecharge.setOnClickListener( new OnClickListener()
//      {
//          public void onClick( View v )
//          {
//        	  if( mMobileView.canPayWindow())
//        	  {
//	              mViewPopStatus = UserStatus.VIEW_POP_PAY;
//	              mMobileView.openPayWindow();
//	              hideMenuAnimation(mMenuFloatWebGame);
//        	  }
////            showMenuFloatView();
//          }
//      });
//      
//      mBtnWebGameSetting = ( Button )findViewById( R.id.btn_webgame_setting );
//      mBtnWebGameSetting.setOnClickListener( new OnClickListener()
//      {
//          public void onClick( View v )
//          {
//              mMenuFloatWebGame.setVisibility( View.GONE );
//              mMenuStatus = UserStatus.MENU_SETTING_FLASH;
//              showSettingFromFlash();
//          }
//      });
//      
//      mBtnWeGameBack = (Button)findViewById( R.id.btn_webgame_back );
//      mBtnWeGameBack.setOnClickListener( new OnClickListener()
//      {
//          public void onClick( View v )
//          {
//              mMobileView.setSilence( false );
//              hideMenuAnimation(mMenuFloatWebGame);
//              //showMenuFloatView();
//              Message msgTime = new Message();
//      	      msgTime.what = MSG_SHOWFLOATVIEW;
//              mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//          }
//      });
//
//        mBtnWebGameFresh = (Button) findViewById(R.id.btn_webgame_fresh);
//        mBtnWebGameFresh.setOnClickListener(new OnClickListener()
//        {
//            public void onClick(View v)
//            {
//                hideMenuAnimation(mMenuFloatWebGame);
//                //showMenuFloatView();
//                Message msgTime = new Message();
//        	    msgTime.what = MSG_SHOWFLOATVIEW;
//                mProcessUseMsg.sendMessageDelayed(msgTime, 200 );
//                Dialog dlg = new AlertDialog.Builder(MobileMain_maopao.this)
//                .setIcon(R.drawable.logo_36)
//                .setTitle("刷新将重新加载游戏，建议您在游戏无法正常加载时使用；是否刷新？")
//                .setPositiveButton(R.string.positive_button, new DialogInterface.OnClickListener()
//                {
//                    public void onClick(DialogInterface dialog, int whichButton)
//                    {
//                        mMobileView.flashRestart();
//                    }
//                    })
//                    .setNegativeButton(R.string.negative_button, new DialogInterface.OnClickListener() 
//                    {
//                    public void onClick(DialogInterface dialog, int which)
//                    {
//                            mMobileView.setSilence( false );
//                    }
//                }).create();
//                dlg.show();
//            }
//        });
//        
//        mBtnWebGameClose = (Button)findViewById( R.id.btn_webgame_close );
//        mBtnWebGameClose.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                mMobileView.setSilence( false );
//                //hideMenuAnimation(mMenuFloatWebGame);
//                //showMenuFloatView();
//                menuExitFlash();
//            }
//        });
////        mBtnWebGameFloatSwitch=(Button)findViewById(R.id.btn_webgame_switch_to_old);
////        mBtnWebGameFloatSwitch.setOnClickListener(new OnClickListener() {
////			
////			@Override
////			public void onClick(View v) {
////				// TODO Auto-generated method stub
////				 hideMenuAnimation(mMenuFloatWebGame);
////				 showMenuWebGameOld();
////				 SharedPreferences   settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
////				 Editor editor = settings.edit();//获取编辑器
////				 boolean firstSwitchToOld =settings.getBoolean("firstSwitchToOld", true);
////				 editor.putBoolean("showOldMenu", true);
////				 editor.commit();//提交修改
////				 if(firstSwitchToOld)
////					{
////						displayTips("可通过滑动浮动菜单到屏幕顶部进行切换");
////						editor.putBoolean("firstSwitchToOld", false);
////						editor.commit();
////					}
////			}
////		});
//        
//        return;
//    }
//    
//    public void loadMenuUserSetting()
//    {
//        SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE,  0 );
//        mMenuUserSetting = findViewById( R.id.menufloatsetting );
//        mBtnSettingVirbtn = ( Button )findViewById( R.id.btn_setting_virbtn );  
//        mBtnSettingVirbtn.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v)
//            {           
//                if( mbShowVirBtn )
//                {
//                    mBtnSettingVirbtn.setBackgroundResource( R.drawable.menu_setting_virbtn_off );
//                }
//                else
//                {
//                    mBtnSettingVirbtn.setBackgroundResource( R.drawable.menu_setting_virbtn );
//                }
//                mbShowVirBtn = !mbShowVirBtn;
//                mMobileView.switchButton();
//              
//            }
//        });
//        mBtnSettingMouse = ( Button)findViewById( R.id.btn_setting_mouse );
//        mbShowMouseSign = settings.getBoolean("USER_MOUSE", true );
//        if( mbShowMouseSign )
//        {
//            mBtnSettingMouse.setBackgroundResource( R.drawable.menu_setting_mouse); 
//        }
//        else
//        {
//            mBtnSettingMouse.setBackgroundResource( R.drawable.menu_setting_mouse_off );
//        }
//        mBtnSettingMouse.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//                mbShowMouseSign = settings.getBoolean("USER_MOUSE", true );
//                mbShowMouseSign = !mbShowMouseSign;
//                SharedPreferences.Editor editor = settings.edit();
//                editor.putBoolean("USER_MOUSE", mbShowMouseSign );
//                editor.commit();
//                if( mbShowMouseSign )
//                {
//                    mBtnSettingMouse.setBackgroundResource( R.drawable.menu_setting_mouse); 
//                }
//                else
//                {
//                    mBtnSettingMouse.setBackgroundResource( R.drawable.menu_setting_mouse_off );
//                }
//            }
//        });
//        
//        mBtnSettingBack = (Button)findViewById( R.id.btn_setting_back );
//        mBtnSettingBack.setOnClickListener( new OnClickListener()
//        {
//            public void onClick( View v )
//            {
//                mMenuUserSetting.setVisibility( View.GONE );
//                backFromUserSetting();    
//            }
//        });
//        mBtnSettingSwitch=(Button)findViewById(R.id.btn_switch_old);
//        mBtnSettingSwitch.setOnClickListener(new OnClickListener() {
//			
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				 SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
//				 Editor editor= settings.edit();
//				 boolean showoldmenu=settings.getBoolean("showOldMenu", true);
//				 if(showoldmenu)
//				 {
//					 editor.putBoolean("showOldMenu", false);
//					 editor.commit();
////					 displayTips("切换成功,当前菜单模式是滑动菜单模式");
//					 mMenuUserSetting.setVisibility(View.GONE);
////					 mMobileView.setTopPos( 0 );
//					 mMobileView.setBottomPos(mScreenHeight);
//					 showMenuFloatView();
//				 }
//				 else
//				 {
//					 editor.putBoolean("showOldMenu", true);
//					 editor.commit();
////					 displayTips("切换成功,当前模式是经典菜单模式");
////					 mMobileView.setTopPos( mHeight44 );
//					 mMobileView.setBottomPos(mScreenHeight-mHeight44);
//					 mMenuUserSetting.setVisibility(View.GONE);
//					 if(mUserStatus==UserStatus.FLASH_TYPE_GAME)
//					 {
//						 showMenuGameOld();
//					 }
//					 else  if(mUserStatus==UserStatus.FLASH_TYPE_MEDIA)
//					 {
//						 showMenuMediaOld();
//					 }
//					 else if(mUserStatus==UserStatus.FLASH_TYPE_WEBGAME)
//					 {
//						 showMenuWebGameOld();
//					 }
//				 }
//			}
//		});
//
//        return;
//    }
//	
//    private String getAvailMemory()
//    {  
//        //获取android当前可用内存大小  
//        ActivityManager am=(ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);  
//        MemoryInfo mi=new MemoryInfo();  
//        am.getMemoryInfo(mi);  
//        //mi.avaiMem;当前系统可用内存  
//        return Formatter.formatFileSize(getBaseContext(), mi.availMem);  
//        //将获得的内存大小规格化  
//   	 }  
//   	       
//	 private String getTotalMemory()
//	 {  
//	     String str1="/proc/meminfo";//系统内存信息文件  
//	     String str2;  
//	     String[] arrayOfString;  
//	     long initial_memory=0;  
//	       
//	     try{  
//	         FileReader localFileReader=new FileReader(str1);  
//	         BufferedReader localBufferedReader=new BufferedReader(localFileReader,8192);  
//	         str2=localBufferedReader.readLine();//读取meminfo第一行，系统内存大小  
//	         arrayOfString=str2.split("\\s+");  
//	         for(String num:arrayOfString){  
//	             Log.i(str2,num+"\t");  
//	         }  
//	         initial_memory=Integer.valueOf(arrayOfString[1]).intValue()*1024;//获得系统总内存，单位KB  
//	         localBufferedReader.close();  
//	     }catch(IOException e){  
//	           
//	     }  
//	     return Formatter.formatFileSize(getBaseContext(), initial_memory);  
//	     //Byte转位KB或MB  
//	       
//	 }  
//	 // 获取CPU最大频率（单位KHZ）
//     // "/system/bin/cat" 命令行
//     // "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq" 存储最大频率的文件的路径
//        public  String getMaxCpuFreq() {
//                String result = "";
//                ProcessBuilder cmd;
//                try {
//                        String[] args = { "/system/bin/cat",
//                                        "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq" };
//                        cmd = new ProcessBuilder(args);
//                        Process process = cmd.start();
//                        InputStream in = process.getInputStream();
//                        byte[] re = new byte[24];
//                        while (in.read(re) != -1) {
//                                result = result + new String(re);
//                        }
//                        in.close();
//                } catch (IOException ex) {
//                        ex.printStackTrace();
//                        result = "N/A";
//                }
//                return result.trim();
//        }
// 
//         // 获取CPU最小频率（单位KHZ）
//        public  String getMinCpuFreq() {
//                String result = "";
//                ProcessBuilder cmd;
//                try {
//                        String[] args = { "/system/bin/cat",
//                                        "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq" };
//                        cmd = new ProcessBuilder(args);
//                        Process process = cmd.start();
//                        InputStream in = process.getInputStream();
//                        byte[] re = new byte[24];
//                        while (in.read(re) != -1) {
//                                result = result + new String(re);
//                        }
//                        in.close();
//                } catch (IOException ex) {
//                        ex.printStackTrace();
//                        result = "N/A";
//                }
//                return result.trim();
//        }
// 
//         // 实时获取CPU当前频率（单位KHZ）
//        public  String getCurCpuFreq() {
//                String result = "N/A";
//                try {
//                        FileReader fr = new FileReader(
//                                        "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
//                        BufferedReader br = new BufferedReader(fr);
//                        String text = br.readLine();
//                        result = text.trim();
//                } catch (FileNotFoundException e) {
//                        e.printStackTrace();
//                } catch (IOException e) {
//                        e.printStackTrace();
//                }
//                return result;
//        }
//
//	private native final void nativeInit(int width, int height, int iversion, int browerHeight);
//	private native final void nativeFinalize();
//	private native final void copyFontFile(AssetManager assetManager);
//    //////////////////////////////
//    
//    static
//    {
//    	//System.loadLibrary("GGlibrary");
//    	try
//    	{
//    		System.loadLibrary("GGlibrary");
//    	}
//    	catch(UnsatisfiedLinkError ule)
//    	{
//    	        Log.e("GGlibrary error", "Could not load library GGlibrary, Model:" + android.os.Build.MODEL +
//                    ", SDK:" + android.os.Build.VERSION.SDK + ", Release:" + android.os.Build.VERSION.RELEASE);
//    	}
//    }
//
//} // end class of MobileMain_maopao
