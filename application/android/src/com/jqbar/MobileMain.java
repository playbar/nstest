 package com.jqbar;

import java.io.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.*;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.KeyguardManager;
import android.app.ProgressDialog;
import android.app.ActivityManager.MemoryInfo;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.content.pm.ActivityInfo;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.graphics.Paint.FontMetrics;
import android.graphics.drawable.ClipDrawable;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Parcelable;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.provider.Settings;
import android.provider.Settings.Secure;
import android.telephony.TelephonyManager;
import android.text.format.Formatter;
import android.text.format.Time;
import android.util.DisplayMetrics;
import android.util.FloatMath;
import android.util.Log;
import android.view.*;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.animation.*;
import android.view.animation.Animation.AnimationListener;
import android.widget.*;
import android.widget.RelativeLayout.LayoutParams;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;

import com.jqbar.layout.*;
import com.jqbar.offerwall.BwOfferwall;
import com.jqbar.pay.BwSDK;

import com.tencent.bugly.crashreport.CrashReport;
import com.tencent.bugly.crashreport.CrashReport.UserStrategy;


public class MobileMain extends Activity implements OnTouchListener
{
	public int mOrientation = 0;//游戏启动时的默认状态
	public static int mCurOrientation = 0;// 屏幕显示方向，为1则为修改后状态，为0则为初始状态
	private boolean isAbout=false;
	// menu菜单显示
	private boolean mbShowBar = false;
	private boolean mbOnsale = false;  
	private boolean mbOrientation = false; // true 是竖屏; false 是横屏
	private boolean mbMakeErrorPage = false;
	private EditText mEditText = null;
	private Button mSure = null;
	private boolean isSetXEdit = false;
	private boolean isSetHandel = false;
	private String inputMethod;
	private int mMobileMainID; // accessed by native methods
	private volatile boolean mbRuning = false;
	private boolean mbFirstLoad = true;
	public static int mScreenWidth = 0;
	public static int mScreenHeight = 0;
	public int exitTimes = 0;
	private int mHeight44 = 0;
	
//	public SurfaceView surfaceView=null;
	public IMobileView mMobileView = null;//MobileViewGLES 继承的 GLSurfaceView
	public PageWebView mPageView = null;//自定义的一个WebView
	private boolean mbPageView = false;
	public String mStrWebvurl = null;
	public String mStrWebburl = null;
	
	float mCurrentScale = 1;
	float last_x = -1;
	float last_y = -1;
	
	private ViewLayout mViewLayout = null;// 自定义的类 multouchphone
	private volatile int mUserStatus; // 用户状态
	private volatile int mViewStatus; // 视图大小状态
	private volatile int mViewPopStatus; // 弹出视图状态
	private volatile int mUserActionStatus; // 用户操作状态
	private volatile int mMenuStatus; // 菜单状态
	private int miMoveShreshole = 0; // 移动阈值设置
	private static final String FILE_PROTOCOL = "D:"; // 打开本地文件协议定义

	
	private LinearLayout mLayoutInput;//动态布局中新建的一个LinearLayout  动态把它加到mLayoutMobileMain中
	private MyFrameLayout mLayoutMobileMain;//multouchphone中的自定义的MyFrameLayout
	private RelativeLayout root;//启动动画的跟布局

	private View mMenuBrowser; // 浏览器浮动工具栏
	private View mMenuDirection; // 方向键菜单
	private View mLinearView;//左上角浮动工具栏中的LinearLayout
	private View mMenuUserSetting;//中间浮动工具栏（鼠标指示、虚拟键盘、菜单切换等）

	//游戏中浮动工具栏
	private boolean mbShowOldMenu;//是否显示游戏中浮动菜单
//	private View mMenuGameOld; // 游戏中的中间浮动工具栏（设置、保存、帮助等）显示在下方  同mMenuFloatGame
//	private Button mBtnGameSwitchOld;
//	private Button mBtnGameZoomOld;
//	private Button mBtnGameSettingOld;
//	private Button mBtnGameSaveOld;
//	private Button mBtnGameHelpOld;
//	private Button mBtnGameCloseOld;
	private boolean mbExchangeMenu;//菜单模式切换
	//网页游戏menu菜单
//	private View mMenuWebGameOld;//游戏中的游戏中的中间浮动工具栏（充值、刷新、退出游戏等） 显示在下方 同mMenuFloatWebGame
//	private Button mBtnWebGameSwitchOld;
//	private Button mBtnWebGameSettingOld;
//	private Button mBtnWebGameRefreshOld;
//	private Button mBtnWebGameRechargeOld;
//	private Button mBtnWebGameCloseOld;
	private String mStrLocalFile = FILE_PROTOCOL+ GlobalFun.BWWORKPATH + "/tante.swf";
	private String mStrQualityFile = FILE_PROTOCOL + GlobalFun.BWWORKPATH + "/quality.swf";
	private String mFormat;
	private int timeCount = 0;
	
    
//	private View mMenuMediaOld;//游戏中的没有帮助功能的中间浮动工具栏 显示在下方 同mMenuFloatMedia
//	private Button mMenuMediaSwitchOld;
//	private Button mMenuMediaZoomOld;
//	private Button mMenuMediaSettingOld;
//	private Button mMenuMediaSaveOld;
//	private Button mMenuMediaCloseOld;

	// 浏览器工具栏
	private Button mBtnMenuHome;//浏览器工具栏中的home键
	private Button mBtnMenuBack;//浏览器工具栏中的back键
	private Button mBtnMenuRefresh;//浏览器工具栏中的refresh键
	private Button mBtnMenuForward;
	private ProgressBar mProgressLoading; //  加载页面显示进度

//	private Button mBtnGameSave;
//	private Button mBtnGameSetting;
//	private Button mBtnGameFloatBack;
//	private Button mBtnGameFloatHelp;
//	private Button mBtnGameFloatClose;
//	private Button mBtnGameFloatSwitch;
//
//	private Button mBtnMediaSave;
//	private Button mBtnMediaBack;
//	private Button mBtnMediaClose;
//	private Button mBtnMediaSetting;
//	private Button mBtnMediaFloatSwitch;
//
//	private Button mBntWebGameRecharge;
//	private Button mBtnWebGameSetting;
//	private Button mBtnWeGameBack;
//	private Button mBtnWebGameFresh;
//	private Button mBtnWebGameClose;
//	private Button mBtnSettingMouse;
//	private Button mBtnSettingBack;
//	private Button mBtnSettingSwitch;
	private boolean mbShowVirBtn;

//	private FrameLayout mFrameMore;//左上角浮动框的菜单图片
//	private FrameLayout mFrameZoom;//左上角浮动框的放大缩小图片


	private SensorManager mSensorManager = null;
	private Sensor mSensor = null;
	private boolean mbSensorOpen = false; // 重力感应是否打开
	private WakeLock mWakeLock = null;
	private SensorEventListener mSensorListener = null;

	
	String mUAValue = null;
	String mChannelValue = null; //  渠道号
	String strUrl = null;
	public String mCpu = null;// 由jni层调用
	public String mRam = null;
	public String mDpi = null;
	
	//消息类型
	public static final int MSG_UPDATEVIEW = 1;
	public static final int MSG_LOADINGBEGIN = 2;
	public static final int MSG_LOADINGEND = 3;
	public static final int MSG_USERPROCESS = 4;
	public static final int MSG_TIMEER = 5;
	public static final int MSG_SHOWFLOATVIEW = 6;
	public static final int MSG_SDKINIT = 7;
	public static final int MSG_HIDESOFTINPUT = 8;
	public static final int MSG_OFFERWALLINIT = 9;
	public static final int MSG_PLAY_NETGAME = 10;
	public static final int MSG_PLAY_LOCALGAME = 11;
	public static final int MSG_PLAY_BOUTIQUEGAME = 12;
	public static BwPayZFB zfb = null;
	
	Timer timer = new Timer();

	private Handler mProcessUseMsg = new Handler()
	{
		public void handleMessage(Message msg) {
			if( msg.what == MSG_PLAY_LOCALGAME )
			{
				PlayFlashGame( 2 );
			}
			else if( msg.what == MSG_PLAY_BOUTIQUEGAME)
			{
				PlayFlashGame( 0 );
			}
			else if (msg.what == MSG_UPDATEVIEW) {
				mMobileView.OnIdle();
				Message msgUpdate = new Message();
				msgUpdate.what = MSG_UPDATEVIEW;
				int ideleyTime = 20;
				if (!mbRuning)
					ideleyTime = 1000;
				mProcessUseMsg.sendMessageDelayed(msgUpdate, ideleyTime);
			} else if (msg.what == MSG_LOADINGBEGIN) {
				//开始启动动画 在oncreate中调用了此方法 
				startLoadingImg();
			} else if (msg.what == MSG_LOADINGEND) {
				//结束启动动画
				endLoadingImg();
			} else if (msg.what == MSG_USERPROCESS) {
				Log.e("MobileMain", "MSG_USERPROCESS begin");
				Log.e("MobileMain", "MSG_USERPROCESS end");
			} else if (msg.what == MSG_TIMEER) {
				// Log.e("MobileMain", "MSG_TIMEER" );
				mMobileView.timeTickCpp();
				timeTickActoin();
				if (mbRuning) {
					Message msgTime = new Message();
					msgTime.what = MSG_TIMEER;
					mProcessUseMsg.sendMessageDelayed(msgTime, 50);
				}
			} else if (msg.what == MSG_SHOWFLOATVIEW) {
				showMenuFloatView();
			} else if (msg.what == MSG_SDKINIT) {
				bwSDK.initSDKPay();
			} else if (msg.what == MSG_OFFERWALLINIT) {
				bwOfferwall.initOfferWall();
			} else if (msg.what == MSG_HIDESOFTINPUT) {
				if (showSoftInput && !bCallTextEdit) {
					showSoftInput = false;
					if (inputMethod
							.equals("com.android.inputmethod.pinyin/.PinyinIME")
							|| inputMethod
									.equals("com.samsung.inputmethod/.SamsungIME")) {
						return;
					} else {
						if (!isSetXEdit) {
							mMobileView.SetXEdit("");
							mMobileView
									.SetXEdit(mEditText.getText().toString());
							if (mEditText.getText().length() > 0) {
								mMobileView.SetXEdit(mEditText.getText()
				 						.toString());
								isSetHandel = true;
							}
						}
					}
				} else if (showSoftInput && bCallTextEdit) {
					callTimeCount += 1;
					if (callTimeCount == 2) {
						bCallTextEdit = false;
						callTimeCount = 0;
					}
				}
			}
			super.handleMessage(msg);
		}
	};

	class UserProcessThread implements Runnable {
		public void run() {
			Log.e("MobileMain", "UserProcessThread begin");
			GlobalFun.GFunCopyFontFile(MobileMain.this);
			try {
				startPushService();
			} catch (Exception e) {
				Log.e("UserProcessThread", "Exception");
			}
			Log.e("MobileMain", "UserProcessThread end");
		}
	}

	// start: zhengjl 2011.09.09
	private void registerScreenActionReceiver() {
		final IntentFilter filter = new IntentFilter();
		filter.addAction(Intent.ACTION_USER_PRESENT);
		registerReceiver(mUnlockReceiver, filter);
		Log.e("mUnlockReceiver ", mUnlockReceiver.toString());
	}

	private final BroadcastReceiver mUnlockReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(final Context context, final Intent intent) {
			if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
					|| mUserStatus == UserStatus.FLASH_TYPE_GAME) {
				mMobileView.flashResume();
			} else if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
				mMobileView.setSilence(false);
			}
			mbRuning = true;
		}
	};

	// end: zhengjl 2011.09.09
	//控制home加载图片
	public void btnMenuHomeEnabled(boolean b) {
		if (b) {
			mBtnMenuHome.setEnabled(true);
			mBtnMenuHome.setBackgroundResource(R.drawable.menu_home);
		} else {
			mBtnMenuHome.setEnabled(false);
			mBtnMenuHome.setBackgroundResource(R.drawable.bar_home_touch);
		}
	}
	//控制刷新 加载图片
	public void btnMenuRefreshEnabled(boolean b) {
		if (b) {
			mBtnMenuRefresh.setEnabled(true);
			mBtnMenuRefresh.setBackgroundResource(R.drawable.menu_fresh);
		} else {
			mBtnMenuRefresh.setEnabled(false);
			mBtnMenuRefresh
					.setBackgroundResource(R.drawable.bar_refresh_touch);
		}
	}
	//控制返回 加载图片
	public void btnMenuBackEnabled(boolean b) {
		if (b) {
			mBtnMenuBack.setEnabled(true);
			mBtnMenuBack.setBackgroundResource(R.drawable.menu_back);
		} else {
			mBtnMenuBack.setEnabled(false);
			mBtnMenuBack.setBackgroundResource(R.drawable.bar_back);
		}
	}
	//控制跳转 加载图图片
	public void btnMenuForwardEnabled(boolean b) {
		if (mBtnMenuForward != null && b) {
			mBtnMenuForward.setEnabled(true);
			mBtnMenuForward.setBackgroundResource(R.drawable.menu_forward_pad);
		} else if (mBtnMenuForward != null) {
			mBtnMenuForward.setEnabled(false);
			mBtnMenuForward
					.setBackgroundResource(R.drawable.bar_forward_untouch);
		}
	}
	//显示加载进度条
	public void showProgressLoading() {
		HideMenu();
		if (!mbShowBar) {
			mMenuBrowser.setVisibility(View.INVISIBLE);
		} else {
			mMenuBrowser.setVisibility(View.VISIBLE);
			mMenuBrowser.bringToFront();
		}
		mMenuStatus = UserStatus.MENU_LOADING;
		mProgressLoading.setVisibility(View.VISIBLE);
		mProgressLoading.setProgress(5);
		mProgressLoading.bringToFront();

		btnMenuHomeEnabled(false);
		btnMenuRefreshEnabled(false);
		if (mbShowBar && !mbOnsale) 
		{
			// 未出售状态单击游戏显示工具条
			btnMenuHomeEnabled(false);
			btnMenuForwardEnabled(mMobileView.canForward());
			btnMenuBackEnabled(false);
			btnMenuRefreshEnabled(true);
		} else {
			mBtnMenuRefresh.setEnabled(true);
			mBtnMenuRefresh.setBackgroundResource(R.drawable.menu_stop);
		}
	}
	//停止加载进度条
	public void stopProgressLoading() {
		mMenuStatus = UserStatus.MENU_BROWSER;
		if (mUserStatus == UserStatus.USER_NORMAL) {
			mMobileView.onDomStop();
			btnMenuHomeEnabled(!mMobileView.isCurrentHomePage());
			btnMenuForwardEnabled(mMobileView.canForward());
			btnMenuBackEnabled(mMobileView.canBack());
			btnMenuRefreshEnabled(true);
		} else if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
				|| mUserStatus == UserStatus.FLASH_TYPE_GAME
				|| mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
			if (mMobileView.exitFlash()) {
				closeFlashView();
				mUserStatus = UserStatus.USER_NORMAL;
			} else {
				exitTimes++;
				if (exitTimes >= 2) {
					exitTimes = 0;
					AppQuit();
				} 
				else 
				{
					Toast.makeText(getApplicationContext(), "非正常退出请重试！", 1000).show();
				}
			}
		}
	}

	public void showMediaMenu() {
		HideMenu();
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		mbShowOldMenu = settings.getBoolean("showOldMenu", true);
		if (mbShowOldMenu) {
			// 显示旧菜单
//			mMobileView.setBottomPos(mScreenHeight - mHeight44);
			if (mUserStatus == UserStatus.FLASH_TYPE_GAME) {
				showMenuGameOld();
			}
			if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA) {
				showMenuMediaOld();
			}
			if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
				showMenuWebGameOld();
			}
		} else {
			showMenuFloatView();
		}
	}

	/*
	 * 修改用户状态，并且根据用户状态，修改用户菜单,被c调用
	 */
	public void startFlashView(int sta) {
		Log.e("startflashview", "success");
		
		if (getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT) {
			showProgressLoading();
		}
		switch (sta) {
		case UserStatus.USER_NORMAL:
			mUserStatus = UserStatus.USER_NORMAL;
			break;
		case UserStatus.FLASH_TYPE_MEDIA:
			mUserStatus = UserStatus.FLASH_TYPE_MEDIA;
			break;
		case UserStatus.FLASH_TYPE_GAME:
			mUserStatus = UserStatus.FLASH_TYPE_GAME;
			break;
		case UserStatus.FLASH_TYPE_WEBGAME:
			mUserStatus = UserStatus.FLASH_TYPE_WEBGAME;
			break;
		default:
			mUserStatus = UserStatus.USER_NORMAL;
		}
		if (!mbShowBar) {
		} else {
			showMediaMenu();// 此处处理显示的菜单处理
		}
		
		PowerManager manager = ((PowerManager) getSystemService(POWER_SERVICE));
		mWakeLock = manager.newWakeLock(PowerManager.SCREEN_BRIGHT_WAKE_LOCK
				| PowerManager.ON_AFTER_RELEASE, "ATAAW");
		mWakeLock.acquire();

		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		mbShowMouseSign = settings.getBoolean("USER_MOUSE", true);
		if (!mbShowMouseSign) {
//			mBtnSettingMouse
//					.setBackgroundResource(R.drawable.menu_setting_mouse_off);
		}
		if (mMobileView.hasFlashButton()) {
			mbShowVirBtn = true;
		} else {
			mbShowVirBtn = false;
		}
		
			mSensorManager.registerListener(mSensorListener, mSensor,
					SensorManager.SENSOR_DELAY_GAME);
			mbSensorOpen = true;
		return;
	}

	public void ucMultiAcountExit() {
		if (mMobileView.exitFlash()) {
			closeFlashView();
			try {
				Thread.sleep(200);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public void closeFlashView() {
		mViewLayout.MenuDirectionGone();
		if (mbSensorOpen) {
			mbSensorOpen = false;
			mSensorManager.unregisterListener(mSensorListener);
		}
		mWakeLock.release();
		
		mUserStatus = UserStatus.USER_NORMAL;
		ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER);
		btnMenuHomeEnabled(!mMobileView.isCurrentHomePage());
		btnMenuForwardEnabled(mMobileView.canForward());
		btnMenuBackEnabled(true);
		btnMenuRefreshEnabled(true);
		mMobileView.exit_setShader();
	}

	public void startLoadingImg()
	{
		GetSavedPreferences();
		ResetOrientation();
		mOrientation = mCurOrientation;
		
//		surfaceView=(SurfaceView) this.findViewById(R.id.surfaceview);
//		surfaceView.getHolder().addCallback(this);
		
		init(ITEM_DRAWABLES);
		
		//开启单机游戏
		if (!mbOnsale)
			{
				timer.schedule(new TimerTask()
				{
					@Override
					public void run() 
					{
						mMobileView.setBottomPos(mScreenHeight);
						//mStrLocalFile=D:/sdcard/byone/tante.swf
						mMobileView.connectUrl( mStrLocalFile );
					}
				}, 3000);
			}

	}

	public void endLoadingImg() 
	{
		mbFirstLoad = false;

		createView();
		HideMenu();
			
		root.setVisibility(View.GONE);
	    mLayoutMobileMain.removeView(root);
		
		ShowMobileView( true );

		if (mMenuStatus == UserStatus.MENU_BROWSER)
			ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER);
		else
			showProgressLoading();
		

		if (!mbShowBar)
		{
			HideMenu();
			HideMenuPopMore();
		}

		return;
	}

	public boolean isServiceRunning() {
		String strServiceName = getPackageName() + ".BwPushService";
		boolean isRunning = false;
		ActivityManager activityManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
		List<ActivityManager.RunningServiceInfo> serviceList = activityManager
				.getRunningServices(Integer.MAX_VALUE);
		if (!(serviceList.size() > 0)) {
			return false;
		}

		for (int i = 0; i < serviceList.size(); i++) {
			if (serviceList.get(i).service.getClassName()
					.equals(strServiceName) == true) {
				Log.e("MobileMain", "break");
				isRunning = true;
				break;
			}
		}
		return isRunning;
	}

	// test adress http://192.168.1.102:8080/testweb/testmqtt.aspx
	public void startPushService() {
		if (!isServiceRunning()) {
			String deviceID = Secure.getString(this.getContentResolver(),
					Secure.ANDROID_ID);
			String strPushCfg = GlobalFun.readPushCfg(this);

			JSONObject jbcfg = null;
			try {
				if (strPushCfg.length() == 0)
					jbcfg = new JSONObject();
				else
					jbcfg = new JSONObject(strPushCfg);
				jbcfg.put("deviceID", deviceID);
				GlobalFun.writePushCfg(this, jbcfg.toString());
			} catch (JSONException e) {
				// e.printStackTrace(System.err);
			}
			mMobileView.setDeviceId(deviceID);
			BwPushService.actionStart(getApplicationContext());
		}
		return;
	}

	boolean createShortCut() {
		Intent shortcutIntent = new Intent(
				"com.android.launcher.action.INSTALL_SHORTCUT");
		Parcelable icon = Intent.ShortcutIconResource.fromContext(this,
				R.drawable.byone_logo); // 获取快捷键的图标
		Intent myIntent = new Intent(this, MobileMain.class);
		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_NAME, "百玩");// 快捷方式的标题
		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, icon);// 快捷方式的图标
		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, myIntent);// 快捷方式的动作
		sendBroadcast(shortcutIntent);// 发送广播
		return true;
	}

	private void getOrientationHW() // 根据方向设置高宽
	{
		
		Display display = getWindowManager().getDefaultDisplay();
		mScreenWidth = display.getWidth();
		mScreenHeight = display.getHeight();
		

		System.out.println("Kindle Fire" + android.os.Build.MODEL);

		if (mCurOrientation == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT) // 为竖屏
		{
			if (mScreenWidth > mScreenHeight) 
			{
				int itmp = mScreenHeight;
				mScreenHeight = mScreenWidth;
				mScreenWidth = itmp;
			}
		}
		else if (mCurOrientation == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) // 为横屏
		{
			if (mScreenWidth < mScreenHeight)
			{
				int itmp = mScreenHeight;
				mScreenHeight = mScreenWidth;
				mScreenWidth = itmp;
			}
		}
		//huwh 判断手机型号
		if (android.os.Build.MODEL.equals("Kindle Fire")) 
		{
			mScreenHeight -= 20;
		}
	}

	/**
	 * 判断是否模拟器。如果返回TRUE，则当前是模拟器
	 * 
	 * @param context
	 * @return
	 */
	public static boolean isEmulator(Context context) {
		TelephonyManager tm = (TelephonyManager) context
				.getSystemService(Context.TELEPHONY_SERVICE);
		String imei = tm.getDeviceId();
		if (imei == null || imei.equals("000000000000000")) {
			return true;
		}
		return false;
	}

	// zhengjl 2012.06.05
	void fade_animation(View view) {
		AnimationSet animationSet = new AnimationSet(true);
		AlphaAnimation alphaAnimation = new AlphaAnimation(0.0f, 1.0f);
		alphaAnimation.setDuration(500);
		AlphaAnimation alphaAnimation1 = new AlphaAnimation(1.0f, 0.0f);
		alphaAnimation1.setDuration(500);
		alphaAnimation1.setStartOffset(2500);
		animationSet.addAnimation(alphaAnimation);
		animationSet.addAnimation(alphaAnimation1);
		view.startAnimation(animationSet);
	}
	
	protected void ShowMobileView(boolean bshow)
	{
		if( bshow )
		{
			mLayoutMobileMain.removeView((View) mMobileView);
			mLayoutMobileMain.addView((View) mMobileView);
			((View) mMobileView).setVisibility(View.VISIBLE);
			((View)mMobileView).bringToFront();
		}
		else
		{
			((View) mMobileView).setVisibility(View.GONE);
			mLayoutMobileMain.removeView((View)mMobileView);
		}
	}
	
	protected void ShowPageWebView( boolean bshow )
	{
		if( bshow )
		{
			mLayoutMobileMain.addView( mPageView);
			mPageView.setVisibility( View.VISIBLE );
			mPageView.bringToFront();
		}
		else
		{
			mPageView.setVisibility( View.GONE );
			mLayoutMobileMain.removeView( mPageView );
		}
		return;
	}

	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);
		inputMethod = Settings.Secure.getString(
				MobileMain.this.getContentResolver(),
				Settings.Secure.DEFAULT_INPUT_METHOD);
		mViewLayout = new MulTouchPhone(this);
		mViewLayout.setOrientation();
		mCurOrientation = mOrientation;
		registerScreenActionReceiver(); // zhengjl 2011.09.09
		mViewLayout.setContentView();
		
		String appId = "900002804";
		boolean isDebug = true;
		CrashReport.initCrashReport(this.getApplicationContext(), appId, isDebug );
		
		getOrientationHW();
		// cpu，ram数据
		initCPP();
		
		Intent intent = getIntent();
		Bundle bl = intent.getExtras();
		if (bl != null) 
		{
			mUAValue = bl.getString("UA_CODE");
			mChannelValue = bl.getString("CHANNEL_ID");
			strUrl = bl.getString("BW_URL");
		}
		else
		{
			mChannelValue = GlobalFun.GFunGetCID(this);//91300001
			strUrl = "http://m.jqbar.com/product/";
			// strUrl = "http://m.jqbar.com/testweb/csfk.swf";
		}
		mPageView = new PageWebView( this );
		mPageView.initView( mScreenWidth, mScreenHeight );
		mPageView.setViewZoom();
		mPageView.setJSInterface( );
		
		// SD卡是否存在判定因为其中缓存乐一些字体设置，没有可能汇出现一些问题
		if (!existSDCard()) 
		{
			menuSDCard();
		} 
		else 
		{
			// 解决无网状态无法拷贝字体进入单机无字体
			GlobalFun.getAssetsToSd(this, "aa.swf", GlobalFun.BWWORKPATH + "/tante.swf");
		}
		
		mMobileView = new MobileView(this, mScreenWidth, mScreenHeight);
//		mMobileView.connectUrl(strUrl);
		mMenuStatus = UserStatus.MENU_NONE;
		mUserStatus = UserStatus.USER_NORMAL;
		mViewStatus = UserStatus.VIEW_NORMAL;
		mViewPopStatus = UserStatus.VIEW_POP_NONE;
		mUserActionStatus = UserStatus.USER_ACTION_NONE;
		mbFirstLoad = true;

		mLayoutMobileMain = (MyFrameLayout) findViewById(R.id.layoutMobileMain);
		mLayoutMobileMain.setOnResizeListener(new MyFrameLayout.onResizeListener() 
		{
			@Override
			public void OnResize(int w, int h, int oldw, int oldh) 
			{
				//监听软件盘的隐藏和出现
				Message msg = new Message();
				msg.what = MSG_HIDESOFTINPUT;
				mProcessUseMsg.sendMessageDelayed(msg, 50);
			}
		});
		// mScrollView是手指触点小框
		((View) mMobileView).setOnTouchListener(new OnTouchListener() {
			float baseValue;
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				// Log.e("MobileMain-------------->", "in onTouchEvent fun");
				// 隐藏浏览器菜单
				HideMenuPopMore();
				mX1 = (int) event.getX(0);
				mY1 = (int) event.getY(0);
				if (showSoftInput) {// 显示软键盘时，点击的坐标位置需要调整
					mY1 += (dy - 50);
				}
				if (showSoftInput && !bCallTextEdit) {
					mLayoutMobileMain.removeView(mLayoutInput);
					InputMethodManager imm = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
					imm.hideSoftInputFromWindow(mEditText.getWindowToken(), 0);
					showSoftInput = false;

				}
				// 用于多点触摸操作
				switch (event.getAction() & MotionEvent.ACTION_MASK) {
				case MotionEvent.ACTION_DOWN:
					baseValue = 0;
					
					if (menuBackFromTouch())
						return true;

					// Log.e("MobileMain-------------->",
					// "in onTouchEvent fun ACTION_DOWN");
					if (mbSensorOpen) {
						mSensorManager.unregisterListener(mSensorListener);
					}
					
					mbSwap = false;
					// mGestureStatus = UserStatus.USER_ACTION_TOUCH_DOWN;
					mUserActionStatus = UserStatus.USER_ACTION_TOUCH_DOWN;
					mMobileView.onTouchDownCpp(mX1, mY1, 0);
					mMobileView.invalidateView1(mX1, mY1, mbShowMouseSign);
					break;

				case MotionEvent.ACTION_POINTER_DOWN:
					// Log.e("MobileMain-------------->",
					// "in onTouchEvent fun ACTION_POINTER_DOWN");
					mMobileView.invalidateView1(mX1, mY1, false);
					if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN) {
						int x2 = (int) event.getX(1);
						int y2 = (int) event.getY(1);
						if (showSoftInput) {
							y2 += (dy - 50);
						}
						// Log.e("---------------->onTouchEvent",
						// "ACTION_POINTER_DOWN" + x2 + " " + y2 );
						if (mbSwap) {
							mMobileView.onTouchDownCpp(x2, y2, 0);
						} else {
							mMobileView.onTouchDownCpp(x2, y2, 1);
						}
						// mGestureStatus =
						// UserStatus.USER_ACTION_TOUCH_POINTER_DOWN;
						mUserActionStatus = UserStatus.USER_ACTION_TOUCH_POINTER_DOWN;
						// mMobileView.invalidateView2( x2, y2, true );
					}
					break;
					// 单点和多点触控所共有的
				case MotionEvent.ACTION_MOVE:
					 if (event.getPointerCount() == 2) {
				           
				       } 
					// Log.e("MobileMain-------------->",
					// "in onTouchEvent fun ACTION_MOVE");
					if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN) {
						mMobileView.invalidateView1(mX1, mY1, mbShowMouseSign);
					}
					if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN
							|| mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_UP) {
						if (mbSwap) {
							mMobileView.onTouchMoveCpp(mX1, mY1, 1);
						} else {
							mMobileView.onTouchMoveCpp(mX1, mY1, 0);
						}
					} else if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN) {
						// Log.e("---------------->onTouchEvent",
						// "ACTION_MOVE x1:" + mX1 + ", y1:" + mY1 +", x2:" +
						// mX2 + ", y2:" + mY2 );
						mX2 = (int) event.getX(1);
						mY2 = (int) event.getY(1);
						if (showSoftInput) {
							mY2 += (dy - 50);
						}
						// mMobileView.invalidateView2( mX2, mY2, true );
						if (mbSwap) {
							mMobileView.onTouchMoveCpp(mX1, mY1, 1);
							mMobileView.onTouchMoveCpp(mX2, mY2, 0);
						} else {
							float xx = mX1 - mX2;
					           float yy = mY1 - mY2;
					           float value = (float) Math.sqrt(xx * xx + yy * yy);// 计算两点的距离
					           if (baseValue == 0) {
					                baseValue = value;
					           } 
					           else {
					               if (value - baseValue >= 10 || value - baseValue <= -10) {
					                   float scale = value / baseValue;// 当前两点间的距离除以手指落下时两点间的距离就是需要缩放的比例。
					                   if(scale<1){
					                	   mMobileView.flashZoomInOrOut(0);
					                   }else if(scale>1){
					                	   mMobileView.flashZoomInOrOut(1);
					                   }
					               }
					           }
							mMobileView.onTouchMoveCpp(mX1, mY1, 0);
							mMobileView.onTouchMoveCpp(mX2, mY2, 1);
						}
					}
					break;
				case MotionEvent.ACTION_POINTER_UP:
					// Log.e("MobileMain-------------->",
					// "in onTouchEvent fun ACTION_POINTER_UP");
					if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN) {
						// Log.e("---------------->onTouchEvent",
						// "ACTION_POINTER_UP" );
						int x2 = (int) event.getX(1);
						int y2 = (int) event.getY(1);
						if (showSoftInput) {
							y2 += (dy - 50);
						}
						if (mX2 > x2 - 5 && mX2 < x2 + 5 && mY2 > y2 - 5
								&& mY2 < y2 + 5) {
							mbSwap = false;
						} else {
							mbSwap = true;
						}
						mMobileView.onTouchUpCpp(x2, y2, 1);

						if ((mUserStatus == UserStatus.FLASH_TYPE_MEDIA
								|| mUserStatus == UserStatus.FLASH_TYPE_GAME || mUserStatus == UserStatus.FLASH_TYPE_WEBGAME)
								&& mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN) {
							// switchZoomingStatus();
						}

						// mGestureStatus = UserStatus.USER_ACTION_TOUCH_DOWN;
						mUserActionStatus = UserStatus.USER_ACTION_TOUCH_POINTER_UP;
						miTimeCount = 4;
					}
					break;
				case MotionEvent.ACTION_UP:
					// Log.e("MobileMain-------------->",
					// "in onTouchEvent fun ACTION_UP");
					// Log.e("---------------->onTouchEvent", "ACTION_UP" );
					mbSwap = false;
					mMobileView.onTouchUpCpp(mX1, mY1, 0);
					// mGestureStatus = UserStatus.USER_ACTION_NONE;
					if (mbSensorOpen) {
						mSensorManager.registerListener(mSensorListener,
								mSensor, SensorManager.SENSOR_DELAY_GAME);
					}
					mUserActionStatus = UserStatus.USER_ACTION_TOUCH_UP;
					miTimeCount = 4;
					break;

				}
				return true;
			}
		});
        //加载浏览器浮动工具栏
		mViewLayout.CreateMenuEvent();
		
		context=MobileMain.this;
		myHandler=new MyHandler();
		zfb = new BwPayZFB(this, mScreenWidth, mScreenHeight);
		bwSDK = BwSDK.DefaultSDK();
		bwSDK.createSDKAni();
		bwOfferwall = BwOfferwall.DefaultOFFERWALL();
		
		mMenuBrowser.setVisibility(View.GONE);
		mProgressLoading.setVisibility(View.GONE);
		mMenuStatus = UserStatus.MENU_NONE;
		
		Message msg = new Message();
		msg.what = MSG_LOADINGBEGIN;
		mProcessUseMsg.sendMessage(msg);
		
		// 检测系统是否支持 OpenGL ES 2.0  
	    final ActivityManager activityManager = (ActivityManager) getSystemService(Context. ACTIVITY_SERVICE);  
	    final ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();  
	    final boolean supportsEs2 = configurationInfo.reqGlEsVersion <= 0x20000; 

		mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
		mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		mSensorListener = new SensorEventListener() {
			public void onAccuracyChanged(Sensor sensor, int accuracy) {
			}

			public void onSensorChanged(SensorEvent event) {
				float x = event.values[SensorManager.DATA_X];
				float y = event.values[SensorManager.DATA_Y];
				float z = event.values[SensorManager.DATA_Z];
				if (mCurOrientation == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE )
				{
					mMobileView.senseAcc(y, -x, z);
				} 
				else if (mCurOrientation == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT ) 
				{
					mMobileView.senseAcc(-x, -y, z);
				}
			}
		};

		return;
	}

	public void switchViewHW(int mode)// 为1则为横排，为0则为竖排
	{
		if (mode == 1) {
			if (mScreenHeight > mScreenWidth)
			{
				int itmp = mScreenHeight;
				mScreenHeight = mScreenWidth;
				mScreenWidth = itmp;
			}
		} else if (mode == 0) {
			if (mScreenHeight < mScreenWidth) {
				int itmp = mScreenHeight;
				mScreenHeight = mScreenWidth;
				mScreenWidth = itmp;
			}

		}
		mMobileView.setScreenWHJava(mScreenWidth, mScreenHeight);
	}

	/*
	 * 设置屏幕显示方向 mode ： 为1则为横排，为0则为竖排
	 */
	public void SetMode(int mode) 
	{
		Log.i("tag", "setMode="+mode);
		mViewLayout.setMode(mode);
		
		return;
	}

	public void ShowMenuPopMore()  // 是否显示更多的弹出菜单
	{
		if (mMenuStatus == UserStatus.MENU_BROWSER) {
			mMenuStatus = UserStatus.MENU_BROWSER_POP;
		}
	}

	public void HideMenuPopMore() {
		if (mMenuStatus == UserStatus.MENU_BROWSER_POP) {
			mMenuStatus = UserStatus.MENU_BROWSER;
		}
	}

	public void HideMenu() {
		mMenuBrowser.setVisibility(View.GONE);
		//mMenuFloatView.setVisibility(View.GONE);
		//mMenuFloatMedia.setVisibility(View.GONE);
		//mMenuFloatGame.setVisibility(View.GONE);
		//mMenuFloatWebGame.setVisibility(View.GONE);
		mProgressLoading.setVisibility(View.GONE);
		mMenuStatus = UserStatus.MENU_NONE;
		mImageView.setVisibility(View.GONE);
	}

	public void ShowMenu(View vMenu, int menuStatus) {
		HideMenu();
		if (menuStatus != UserStatus.MENU_NONE) {
			mMobileView.bringToFront();
			mMenuStatus = menuStatus;
			if (!mbShowBar) {
				mMenuBrowser.setVisibility(View.INVISIBLE);
			} else {
				mMenuBrowser.setVisibility(View.VISIBLE);
				mMenuBrowser.bringToFront();

			}
		}
	}

	private boolean existSDCard() {
		if (android.os.Environment.getExternalStorageState().equals(
				android.os.Environment.MEDIA_MOUNTED)) {
			return true;
		} else
			return false;
		// TODO Auto-generated method stub

	}

	private void menuSDCard() {
		Dialog dlsd = new AlertDialog.Builder(MobileMain.this)
				.setIcon(R.drawable.logo_36)
				.setTitle("本机没有SD卡，运行中可能出现异常？")
				.setPositiveButton(R.string.positive_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {

							}
						}).create();
		dlsd.show();

	}

	public void menuExitFlash() {

		if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
				|| mUserStatus == UserStatus.FLASH_TYPE_GAME) {
			mMobileView.flashPause();
		} else if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
			mMobileView.setSilence(true);
		}
		
		
		Dialog dlg = new AlertDialog.Builder(MobileMain.this)
				.setIcon(R.drawable.logo_36)
				.setTitle("确定退出？")
				.setPositiveButton(R.string.positive_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
								if (mMobileView.exitFlash()) {
									closeFlashView();
								} else {
									exitTimes++;
									if (exitTimes < 3) {
										Toast.makeText(getApplicationContext(),
												"非正常退出请重试！", 1000).show();
									}
								}
							}
						})
				.setNegativeButton(R.string.negative_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int which) {
								if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
										|| mUserStatus == UserStatus.FLASH_TYPE_GAME) {
									mMobileView.flashResume();
								} else if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
									mMobileView.setSilence(false);
								}
							}
						}).create();
		dlg.setOnKeyListener(new DialogInterface.OnKeyListener() {
			public boolean onKey(DialogInterface dialog, int keyCode,
					KeyEvent event) {
				if (keyCode == KeyEvent.KEYCODE_BACK) {
					if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
							|| mUserStatus == UserStatus.FLASH_TYPE_GAME) {
						mMobileView.flashResume();
					} else if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
						mMobileView.setSilence(false);
					}
				}
				return false;
			}
		});
		dlg.show();
	}

	public void MenuDirectionVisible() {
		mMenuDirection.setVisibility(View.VISIBLE);
		mMenuDirection.bringToFront();
	}

	public void MenuDirectionGone() {
		mMenuDirection.setVisibility(View.GONE);
	}

	private void switchZooming() {
		if ((mMobileView.getZoomPecent() <= 0) && mViewStatus == UserStatus.VIEW_NORMAL) 
		{
			mViewStatus = UserStatus.VIEW_LARGE;
			mMobileView.flashPointZoom();
			mViewLayout.MenuDirectionVisible();
//			mFrameZoom.setBackgroundResource( R.drawable.menu_zoom_min );

		} else {
			
			mMobileView.flashPointZoom();
			mViewStatus = UserStatus.VIEW_NORMAL;
			mViewLayout.MenuDirectionGone();
//			mFrameZoom.setBackgroundResource( R.drawable.menu_zoom_max );

		}
	}

	/*
	 * 创建用户菜单响应
	 */
	public void CreateMenuEvent() {
		mViewLayout.CreateMenuEvent();

	} 

	private final static int TIME_GAP = 4;

	public void displayTips(String str) {
		Toast.makeText(this, str, Toast.LENGTH_SHORT).show();
	}

	private int mMenuGameOldHeight;
	private boolean mbMenuGameOldMove;
	private int mMenuGameOldMoveY;

	private boolean mbMenuWebGameOldMove;
	private int mMenuWebGameOldMoveY;

	private boolean mbMenuMediaOldMove;
	private int mMenuMediaOldMoveY;
	private int mBtnFloatImgWidth = 56;
	private int mBtnFloatImgHeight = 56;
	private int mMoveX = 0;
	private int mMoveY = 0;
	private boolean mbFloatViewMove = false;
	private int mIFloatViewCount = -1;
	public int miImgProgressShow = 0;

	public void showImgProgress() {
		miImgProgressShow = 36;
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		boolean showoldmenu = settings.getBoolean("showOldMenu", true);

	}

	public void setMenuFloatViewPos(int iposx, int iposy) {
		int iposright = mScreenWidth - 2 * mBtnFloatImgWidth;
		iposx = iposx < 0 ? 0 : iposx;
		iposx = iposx > iposright ? iposright : iposx;

		int iposbottom = mScreenHeight - mBtnFloatImgHeight;
		iposy = iposy < 0 ? 0 : iposy;
		iposy = iposy > iposbottom ? iposbottom : iposy;

		if (iposy == (mScreenHeight - mBtnFloatImgHeight)) {
			mbExchangeMenu = true;
		}
		if (!mbExchangeMenu) {
			android.widget.AbsoluteLayout.LayoutParams btnMoreParams = new AbsoluteLayout.LayoutParams(
					LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT,
					iposx, iposy);
			mLinearView.setLayoutParams(btnMoreParams);
		}
	}

	public void showMenuFloatView() {
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		int iposx = settings.getInt("USER_POS_X", mScreenWidth - 2
				* mBtnFloatImgWidth);
		int iposy = settings.getInt("USER_POS_Y", 20);
		setMenuFloatViewPos(iposx, iposy);
		mMenuStatus = UserStatus.MENU_FLOAT_VIEW;

		GlobalFun.showFloatMenu = true;// modify 11.13

	}

	public void showMenuGameOld() {
		HideMenu();
		mMenuStatus = UserStatus.MENU_GAME_OLD;
		GlobalFun.showMenu = true;
//		mMenuGameOld.setVisibility(View.VISIBLE);
//		mMenuGameOld.bringToFront();
		
		
		// huwh 每次进入游戏小球的位置都在左上角
		windowManagerParams.x = 0;
		windowManagerParams.y = 0;
		windowManager.updateViewLayout(mFrameLayout, windowManagerParams); // 刷新显示
		mArcLayout.bringToFront();
		mArcLayout.setPosition(PLACE_LEFT_TOP);
		mArcLayout.requestLayout();
		new Handler().postDelayed(new Runnable() {

			public void run() {
				mImageView.setVisibility(View.VISIBLE);
			}
		}, 10);
	}

	public void showMenuWebGameOld() {
		HideMenu();
		mMenuStatus = UserStatus.MENU_WEBGAME_OLD;
		GlobalFun.showMenu = true;


//		mMenuWebGameOld.setVisibility(View.VISIBLE);
//		mMenuWebGameOld.bringToFront();
		
		// huwh 每次进入游戏小球的位置都在左上角
		windowManagerParams.x = 0;
		windowManagerParams.y = 0;
		windowManager.updateViewLayout(mFrameLayout, windowManagerParams); // 刷新显示
		mArcLayout.bringToFront();
		mArcLayout.setPosition(PLACE_LEFT_TOP);
		mArcLayout.requestLayout();
		new Handler().postDelayed(new Runnable() {

			public void run() {
				mImageView.setVisibility(View.VISIBLE);
			}
		}, 10);
	}

	public void showMenuMediaOld() {
		HideMenu();
		mMenuStatus = UserStatus.MENU_MEDIA_OLD;
		GlobalFun.showMenu = true;
		
//		mMenuMediaOld.setVisibility(View.VISIBLE);
//		mMenuMediaOld.bringToFront();
		
		// huwh 每次进入游戏小球的位置都在左上角
		windowManagerParams.x = 0;
		windowManagerParams.y = 0;
		windowManager.updateViewLayout(mFrameLayout, windowManagerParams); // 刷新显示
		mArcLayout.bringToFront();
		mArcLayout.setPosition(PLACE_LEFT_TOP);
		mArcLayout.requestLayout();
		new Handler().postDelayed(new Runnable() {

			public void run() {
				mImageView.setVisibility(View.VISIBLE);
			}
		}, 10);
	}

	private boolean mbShowMouseSign = true;

	public void hideSettingFromBrowser() {
		AnimationParam param = new AnimationParam();
		param.mView = mMenuUserSetting;
		param.mDuration = 200;
		param.mTranFromX = 0;
		param.mTranToX = 120;
		param.mTranFromY = 0;
		param.mTranToY = 120;
		param.mScaleFromX = 1;
		param.mScaleToX = 0;
		param.mScaleFromY = 1;
		param.mScaleToY = 0;
		MenuAnimation(param);

		mMenuUserSetting.setVisibility(View.GONE);
	}

	public void showSettingFormBrowser() {
		AnimationParam param = new AnimationParam();
		param.mView = mMenuUserSetting;
		param.mDuration = 200;
		param.mTranFromX = 120;
		param.mTranToX = 0;
		param.mTranFromY = 120;
		param.mTranToY = 0;
		param.mScaleFromX = 0;
		param.mScaleToX = 1;
		param.mScaleFromY = 0;
		param.mScaleToY = 1;
		MenuAnimation(param);

		mMenuUserSetting.setVisibility(View.VISIBLE);
		mMenuUserSetting.bringToFront();
	}

//	public void showSettingFromFlash() {
//		AnimationParam param = new AnimationParam();
//		param.mView = mBtnSettingMouse;
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = 120;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//		if (mUserStatus != UserStatus.FLASH_TYPE_WEBGAME) {
//			param.mDuration = 200;
//			param.mTranFromX = 120;
//			param.mTranToX = 0;
//			param.mTranFromY = 40;
//			param.mTranToY = 0;
//			param.mScaleFromX = 0;
//			param.mScaleToX = 1;
//			param.mScaleFromY = 0;
//			param.mScaleToY = 1;
//			MenuAnimation(param);
//		}
//		param.mView = mBtnSettingBack;
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		// 去掉设置中的菜单切换
//		param.mView = mBtnSettingSwitch;
//		param.mDuration = 200;
//		param.mTranFromX = -40;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//
//		mMenuUserSetting.setVisibility(View.VISIBLE);
//		mMenuUserSetting.bringToFront();
//	}

//	public void showMediaFromSetting() {
//		AnimationParam param = new AnimationParam();
//		param.mView = mBtnMediaSave;
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = 120;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnMediaSetting;
//		param.mDuration = 200;
//		param.mTranFromX = 120;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnMediaBack;
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnMediaClose;
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = -40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//	}

//	public void showGameFromSetting() {
//		AnimationParam param = new AnimationParam();
//		param.mView = mBtnGameSave;
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = 120;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnGameSetting;
//		param.mDuration = 200;
//		param.mTranFromX = 120;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnGameFloatBack;
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnGameFloatHelp;
//		param.mDuration = 200;
//		param.mTranFromX = -80;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnGameFloatClose;
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = -40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//	}

//	public void showWebGameFromSetting() {
//		AnimationParam param = new AnimationParam();
//		param.mView = mBntWebGameRecharge; // 上
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = 120;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnWebGameSetting; // 左
//		param.mDuration = 200;
//		param.mTranFromX = 120;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnWeGameBack; // 中
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnWebGameFresh; // 右
//		param.mDuration = 200;
//		param.mTranFromX = -80;
//		param.mTranToX = 0;
//		param.mTranFromY = 40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//		param.mView = mBtnWebGameClose; // 下
//		param.mDuration = 200;
//		param.mTranFromX = 40;
//		param.mTranToX = 0;
//		param.mTranFromY = -40;
//		param.mTranToY = 0;
//		param.mScaleFromX = 0;
//		param.mScaleToX = 1;
//		param.mScaleFromY = 0;
//		param.mScaleToY = 1;
//		MenuAnimation(param);
//
//	}

	private void changeUserMenu() // 显示菜单状态
	{
		
		switch (mUserStatus) {
		case UserStatus.FLASH_TYPE_MEDIA:
			HideMenu();
			mMenuStatus = UserStatus.MENU_MEDIA;
			break;
		case UserStatus.FLASH_TYPE_GAME:
			HideMenu();
			mMenuStatus = UserStatus.MENU_GAME;
			break;
		case UserStatus.FLASH_TYPE_WEBGAME:
			HideMenu();
			mMenuStatus = UserStatus.MENU_WEBGAME;
			break;
		default:
			mUserStatus = UserStatus.USER_NORMAL;
			ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER);
		}
		return;
	}

	public void backFromUserSetting() {
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		mbShowOldMenu = settings.getBoolean("showOldMenu", true);
		switch (mUserStatus) {
		case UserStatus.FLASH_TYPE_MEDIA:
			if (mbShowOldMenu) {
//				mMobileView.setBottomPos(mScreenHeight - mHeight44);
				showMenuMediaOld();
			} else {
				mMenuStatus = UserStatus.MENU_MEDIA;
//				showMediaFromSetting();
			}
			break;
		case UserStatus.FLASH_TYPE_GAME:

			if (mbShowOldMenu) {
//				mMobileView.setBottomPos(mScreenHeight - mHeight44);
				showMenuGameOld();
			} else {
				mMenuStatus = UserStatus.MENU_GAME;
//				showGameFromSetting();
			}
			break;
		case UserStatus.FLASH_TYPE_WEBGAME:
			if (mbShowOldMenu) {
//				mMobileView.setBottomPos(mScreenHeight - mHeight44);
				showMenuWebGameOld();
			} else {
				mMenuStatus = UserStatus.MENU_WEBGAME;
//				showWebGameFromSetting();
			}
			break;
		default:
			mUserStatus = UserStatus.USER_NORMAL;
			ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER);
		}
		return;
	}

	public void MenuAnimation(AnimationParam param) {
		AnimationSet animationSet = new AnimationSet(true);

		TranslateAnimation translateAnimation = new TranslateAnimation(
				param.mTranFromX, param.mTranToX, param.mTranFromY,
				param.mTranToY);
		translateAnimation.setDuration(param.mDuration);

		ScaleAnimation scaleAnimation = new ScaleAnimation(param.mScaleFromX,
				param.mScaleToX, param.mScaleFromY, param.mScaleToY);
		scaleAnimation.setDuration(param.mDuration);

		animationSet.addAnimation(scaleAnimation);
		animationSet.addAnimation(translateAnimation);

		param.mView.setAnimation(animationSet);
		animationSet.start();
		param.mView.setVisibility(View.VISIBLE);
		param.mView.bringToFront();
	}

	public void showMenuAnimation(View view) {
		String str240 = getString(R.dimen.bwpx240);
		str240 = str240.substring(0, str240.indexOf("."));
		int len240 = Integer.parseInt(str240);
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		int iposx = settings.getInt("USER_POS_X", 20);
		int iposy = settings.getInt("USER_POS_Y", 20);
		int iposright = mScreenWidth - 2 * mBtnFloatImgWidth;
		iposx = iposx < 0 ? 0 : iposx;
		iposx = iposx > iposright ? iposright : iposx;

		int iposbottom = mScreenHeight - mBtnFloatImgHeight;
		iposy = iposy < 0 ? 0 : iposy;
		iposy = iposy > iposbottom ? iposbottom : iposy;

		AnimationParam param = new AnimationParam();
		param.mView = view;
		param.mDuration = 200;
		param.mTranFromX = iposx - mScreenWidth / 2 + len240 / 2;
		param.mTranToX = 0;
		param.mTranFromY = iposy - mScreenHeight / 2 + len240 / 2;
		param.mTranToY = 0;
		param.mScaleFromX = 0.367f;
		param.mScaleToX = 1;
		param.mScaleFromY = 0.225f;
		param.mScaleToY = 1;

		MenuAnimation(param);
		return;

	}

	public void hideMenuAnimation(View view) {
		String str240 = getString(R.dimen.bwpx240);
		str240 = str240.substring(0, str240.indexOf("."));
		int len240 = Integer.parseInt(str240);
		view.setVisibility(View.GONE);
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		int iposx = settings.getInt("USER_POS_X", 20);
		int iposy = settings.getInt("USER_POS_Y", 20);
		int iposright = mScreenWidth - 2 * mBtnFloatImgWidth;
		iposx = iposx < 0 ? 0 : iposx;
		iposx = iposx > iposright ? iposright : iposx;

		int iposbottom = mScreenHeight - mBtnFloatImgHeight;
		iposy = iposy < 0 ? 0 : iposy;
		iposy = iposy > iposbottom ? iposbottom : iposy;

		AnimationSet animationSet = new AnimationSet(true);

		TranslateAnimation translateAnimation = new TranslateAnimation(0, iposx
				- mScreenWidth / 2 + len240 / 2, 0, iposy - mScreenHeight / 2
				+ len240 / 2);
		translateAnimation.setDuration(200);

		ScaleAnimation scaleAnimation = new ScaleAnimation(1, 0.367f, 1, 0.225f);
		scaleAnimation.setDuration(200);

		animationSet.addAnimation(scaleAnimation);
		animationSet.addAnimation(translateAnimation);
		view.setAnimation(animationSet);
		animationSet.start();
	}

	public void loadPageStart() {
		if (mbFirstLoad)
			mMenuStatus = UserStatus.MENU_LOADING;
		else if (mUserStatus == UserStatus.USER_NORMAL) // zhengjl 2011.09.09
		{
			showProgressLoading();
		}
	}

	public void loadPagePercent(int per) {
		if (!mbFirstLoad) {
			if (per < 5)
				per = 5;
			mProgressLoading.setProgress(per);
			mProgressLoading.bringToFront();
		}
	}

	public void loadPageEnd()
	{
		
		if (mbFirstLoad)
			mMenuStatus = UserStatus.MENU_BROWSER;
		else if (mUserStatus == UserStatus.USER_NORMAL) {
			ShowMenu(mMenuBrowser, UserStatus.MENU_BROWSER);
			btnMenuHomeEnabled(!mMobileView.isCurrentHomePage());
			btnMenuForwardEnabled(mMobileView.canForward());
			btnMenuBackEnabled(mMobileView.canBack());
			btnMenuRefreshEnabled(true);
		} else if (mMenuStatus == UserStatus.MENU_LOADING) {
			HideMenu();
		}
	}

	// flash加载完成回调函数
	public void flashLoadingEnd() {
		if (mUserStatus == UserStatus.FLASH_TYPE_GAME) {
//			mBtnGameSave.setEnabled(true);
//			mBtnGameSave.setBackgroundResource(R.drawable.menu_save);
//			mBtnGameSaveOld.setEnabled(true);
//			mBtnGameSaveOld.setBackgroundResource(R.drawable.menu_save_old);
		} else if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA) {
//			mBtnMediaSave.setEnabled(true);
//			mBtnMediaSave.setBackgroundResource(R.drawable.menu_save);
//			mMenuMediaSaveOld.setEnabled(true);
//			mMenuMediaSaveOld.setBackgroundResource(R.drawable.menu_save_old);
		}
	}

	public void closeSubView() {
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		boolean showOldMenu = settings.getBoolean("showOldMenu", true);
		if (showOldMenu) {
			if (!GlobalFun.showMenu) {
//				mMobileView.setBottomPos(mScreenHeight - mHeight44);

				if (mUserStatus == UserStatus.FLASH_TYPE_GAME) {
					showMenuGameOld();
				} else if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA) {
					showMenuMediaOld();
				} else if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
					showMenuWebGameOld();
				}
			}
		} else {
			HideMenu();
			showMenuFloatView();
		}
		mViewPopStatus = UserStatus.VIEW_POP_NONE;
	}

	private static String format(int t) {
		String s = "" + t;
		if (s.length() == 1) {
			s = "0" + s;
		}
		return s;

	}

	
	public BwSDK bwSDK = null;
	Timer startSDKTimer = null;
	TimerTask startSDKTask = null;
	public BwOfferwall bwOfferwall = null;

	public void enterUserCenter() {
		bwSDK.enterUserCenter();
	}

	public void paySDK(String strUrl) {
		bwSDK.pay(strUrl);
	}

	public void showOfferWall(String userId) {
		bwOfferwall.showOfferWall(userId);
	}

	public void setOutLogin(int b) {
		if (!bwSDK.isCreatedPay) {
			bwSDK.createSDKPay(this, b);
			Message msg = new Message();
			msg.what = MSG_SDKINIT;
			mProcessUseMsg.sendMessageDelayed(msg, bwSDK.delayTime + 1000);
		}
		if (!bwOfferwall.isCreatedWall) {
			bwOfferwall.createOfferWall(this);
			Message msg = new Message();
			msg.what = MSG_OFFERWALLINIT;
			mProcessUseMsg.sendMessageDelayed(msg, 1500);
		}
	}

	public String encUIDStr(String str) {
		return mMobileView.encUIDStr(str);
	}

	public String getAppUA() {
		return mMobileView.getAppUA();
	}

	public String getAppHeaders() {
		return mMobileView.getAppHeaders();
	}

	public void connectUrl(String str) {
		mMobileView.connectUrl(str);
	}

	// end zhengjl 2012.03

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		boolean flag = bwSDK.result(requestCode, resultCode, data);
		if (flag)
			return;

		switch (resultCode) {
		case RESULT_OK:
			Bundle bl = data.getExtras();
			String strFile = bl.getString("strFile");
			strFile = FILE_PROTOCOL + strFile;
			mMobileView.connectUrl(strFile);
			break;

		default:
			break;
		}
		return;
	}

	protected void onStart() {
		Log.i("MobileMain", "onStart");
		super.onStart();

		Message msg = new Message();
		msg.what = MSG_UPDATEVIEW;
		mProcessUseMsg.sendMessageDelayed(msg, 1000);
	}

	 protected void onRestart()
	 {
	 Log.e("MobileMain", "onRestart");
	 super.onRestart();
	 mMobileView.onDomStop();
	 stopProgressLoading();
	 isAbout=false;
	 }
	 
	 public void refresh(){
		 mMobileView.onDomStop();
		 stopProgressLoading();
	 }
	 
	protected void onResume() {
		Log.i("MobileMain", "onResume");
		super.onResume();
		mMobileView.onResume();

		Message msgTime = new Message();
		msgTime.what = MSG_TIMEER;
		mProcessUseMsg.sendMessageDelayed(msgTime, 50);
		mbRuning = true;
		KeyguardManager mKeyguardManager = (KeyguardManager) getSystemService(KEYGUARD_SERVICE);
		if (!mKeyguardManager.inKeyguardRestrictedInputMode()) {
			if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
					|| mUserStatus == UserStatus.FLASH_TYPE_GAME) {
				mMobileView.flashResume();
			} else if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
				mMobileView.setSilence(false);
			}
			mbRuning = true;
		}
		
		if(!isShowTool){
			mImageView.setVisibility(View.VISIBLE);
			isShowTool=true;
		}
		
		if(isStop){
			mImageView.setVisibility(View.VISIBLE);
			isStop=false;
		}

	}

	@Override
	protected void onPause() {
		Log.i("MobileMain", "onPause");
		super.onPause();
		mMobileView.onPause();
		if (mbRuning) {
			if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
					|| mUserStatus == UserStatus.FLASH_TYPE_GAME) {
				mbRuning = false;
				mMobileView.flashPause();
			} else if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
				mMobileView.setSilence(true);
			} else {
				mbRuning = false;
			}
		}
	}

	protected void onStop() {
		Log.i("MobileMain", "onStop");
		super.onStop();
		
		if(mImageView.getVisibility()==View.VISIBLE){
            isStop=true;
			mImageView.setVisibility(View.GONE);
		}
		//如果不是进入关于界面，则退出游戏。即后台运行，退出游戏。
		if (!isAbout) {
			finish();
			android.os.Process
			.killProcess(android.os.Process.myPid());
		}
	}

	protected void onDestroy() {
		// Debug.stopMethodTracing();
		Log.i("MobileMain", "onDestroy");
		bwSDK.destory();
		super.onDestroy();
		mbRuning = false;
		mMobileView.Release();
		nativeFinalize();
		mWakeLock.release();
		Log.i("tag","MainActivity onDestroy");
		windowManager.removeView(mFrameLayout);
		
		try {
			Thread.sleep(200);
		} catch (InterruptedException e) {
			Thread.currentThread().interrupt();
		}
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_LANDSCAPE) {
			// 加入横屏要处理的代码

		} else if (this.getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT) {
			// 加入竖屏要处理的代码
		}
	}

	protected void onSaveInstanceState(Bundle outState) {
		super.onSaveInstanceState(outState);
	}

	private float mfOldDist = 1f;
	private int mfFlashZoom = 0;
	private int mfFlashZoomTmp = 0;

	private void ZoomFlashView(float fscale) {
		if (fscale > 1) {
			mfFlashZoomTmp = mfFlashZoom + (int) ((fscale - 1) * 40);
			if (mfFlashZoomTmp > 100)
				mfFlashZoomTmp = 100;
		} else {
			mfFlashZoomTmp = mfFlashZoom - (int) ((1 - fscale) * 4 * 40);
			if (mfFlashZoomTmp < 0)
				mfFlashZoomTmp = 0;
		}
		mMobileView.flashZoomInOut(mfFlashZoomTmp);
	}

	// 判断点是否在矩形内
	public boolean pointInRect(Rect rect, int x, int y) {
		if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom) {
			return true;
		} else {
			return false;
		}
	}

	// 点击非菜单时是否切换菜单状态
	public boolean menuBackFromTouch() {
		if (mViewPopStatus == UserStatus.VIEW_POP_PAY
				|| mViewPopStatus == UserStatus.VIEW_POP_HELP) {
			return false;
		}

		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		mbShowOldMenu = settings.getBoolean("showOldMenu", true);
		if (mMenuStatus == UserStatus.MENU_GAME) {
			mMenuStatus = UserStatus.MENU_NONE;
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
			return true;
		} else if (mMenuStatus == UserStatus.MENU_MEDIA) {
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
			return true;
		} else if (mMenuStatus == UserStatus.MENU_WEBGAME) {
			mMobileView.setSilence(false);
			hideMenuAnimation(mMenuUserSetting);
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
			return true;
		}
		else if (mMenuStatus == UserStatus.MENU_SETTING_FLASH) {
			if (Math.abs(mX1 - mScreenWidth / 2) < 120
					&& Math.abs(mY1 - mScreenHeight / 2) < 120) {
				mMenuUserSetting.setVisibility(View.GONE);
				backFromUserSetting();
			} else {
				mMobileView.setSilence(false);
				hideMenuAnimation(mMenuUserSetting);
				if (mbShowOldMenu) {
					if (mUserStatus == UserStatus.FLASH_TYPE_GAME)
						showMenuGameOld();
					if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA)
						showMenuMediaOld();
					if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME)
						showMenuWebGameOld();
				}
				if (!mbShowOldMenu) {
					showMenuFloatView();
				}
			}
			return true;
		}
		return false;
	}

	private boolean mbSwap = false; // 判断两个点是否交换
	private int mX1, mY1;
	private int mX2, mY2;

	// 返回键隐藏算键盘和Layout
	private void exitForBackButton() {
		// NoSale状态播放本地动画返回直接退出
		if( mbMakeErrorPage || !mbOnsale )
		{
			MediaGameQuit();
		} else if (mLayoutInput != null && mLayoutInput.isShown()) {
			InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
			imm.hideSoftInputFromWindow(mEditText.getWindowToken(), 0);
			mLayoutMobileMain.removeView(mLayoutInput);
		} else if (mMenuStatus == UserStatus.MENU_MEDIA) {
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
		} else if (mMenuStatus == UserStatus.MENU_GAME) {
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
		} else if (mMenuStatus == UserStatus.MENU_WEBGAME) {
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
		} else if (mMenuStatus == UserStatus.MENU_SETTING_FLASH) {
			hideMenuAnimation(mMenuUserSetting);
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
		} else if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
				|| mUserStatus == UserStatus.FLASH_TYPE_GAME
				|| mUserStatus == UserStatus.FLASH_TYPE_WEBGAME)
		{
			if (exitTimes >= 2) {
				exitTimes = 0;
				AppQuit();
			} 
			else
			{
				if (mbMakeErrorPage || !mbOnsale ) 
				{
					MediaGameQuit();
				} 
				else 
				{
					//huwh 如果小球展开  先关闭小球
					if (mArcLayout.getVisibility()==View.VISIBLE) {
						mArcLayout.switchState(true, position);
					}
					menuExitFlash();
				}
			}
		} 
		else
		{
			HideMenuPopMore();
			AppQuit();
		}

	}

	// 隐藏算键盘和Layout
	private void hideSoftKeyBord() {
		isSetXEdit = true;
		InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.hideSoftInputFromWindow(mEditText.getWindowToken(), 0);
		mMobileView.SetXEdit(mEditText.getText().toString());
		mLayoutInput.setVisibility(View.GONE);
		mLayoutMobileMain.removeView(mLayoutInput);
	}

	public boolean onTouchEvent(MotionEvent event) {
		HideMenuPopMore();
		mX1 = (int) event.getX(0);
		mY1 = (int) event.getY(0);

		switch (event.getAction() & MotionEvent.ACTION_MASK) {
		case MotionEvent.ACTION_DOWN:
			
			if (menuBackFromTouch())
				return true;
			if (mbSensorOpen) {
				mSensorManager.unregisterListener(mSensorListener);
			}
			mbSwap = false;
			mUserActionStatus = UserStatus.USER_ACTION_TOUCH_DOWN;
			mMobileView.onTouchDownCpp(mX1, mY1, 0);
			mMobileView.invalidateView1(mX1, mY1, mbShowMouseSign);
			break;

		case MotionEvent.ACTION_POINTER_DOWN:
			mMobileView.invalidateView1(mX1, mY1, false);
			if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN) {
				int x2 = (int) event.getX(1);
				int y2 = (int) event.getY(1);
				if (mbSwap) {
					mMobileView.onTouchDownCpp(x2, y2, 0);
				} else {
					mMobileView.onTouchDownCpp(x2, y2, 1);
				}
				mUserActionStatus = UserStatus.USER_ACTION_TOUCH_POINTER_DOWN;
			}
			break;
			// 单点和多点触控所共有的
		case MotionEvent.ACTION_MOVE:
			if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN) {
				mMobileView.invalidateView1(mX1, mY1, mbShowMouseSign);
			}
			if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_DOWN
					|| mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_UP) {
				if (mbSwap) {
					mMobileView.onTouchMoveCpp(mX1, mY1, 1);
				} else {
					mMobileView.onTouchMoveCpp(mX1, mY1, 0);
				}
			} else if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN) {
				mX2 = (int) event.getX(1);
				mY2 = (int) event.getY(1);
				if (mbSwap) {
					mMobileView.onTouchMoveCpp(mX1, mY1, 1);
					mMobileView.onTouchMoveCpp(mX2, mY2, 0);
				} else {
					mMobileView.onTouchMoveCpp(mX1, mY1, 0);
					mMobileView.onTouchMoveCpp(mX2, mY2, 1);
				}
			}
			break;
		case MotionEvent.ACTION_POINTER_UP:
			Log.e("MobileMain-------------->", "");
			if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN) {
				int x2 = (int) event.getX(1);
				int y2 = (int) event.getY(1);
				if (mX2 > x2 - 5 && mX2 < x2 + 5 && mY2 > y2 - 5
						&& mY2 < y2 + 5) {
					mbSwap = false;
				} else {
					mbSwap = true;
				}
				mMobileView.onTouchUpCpp(x2, y2, 1);

				if ((mUserStatus == UserStatus.FLASH_TYPE_MEDIA
						|| mUserStatus == UserStatus.FLASH_TYPE_GAME || mUserStatus == UserStatus.FLASH_TYPE_WEBGAME)
						&& mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_DOWN) {
					// switchZoomingStatus();
				}

				// mGestureStatus = UserStatus.USER_ACTION_TOUCH_DOWN;
				mUserActionStatus = UserStatus.USER_ACTION_TOUCH_POINTER_UP;
				miTimeCount = 4;
			}
			break;
		case MotionEvent.ACTION_UP:
			Log.e("MobileMain-------------->", "");
			mbSwap = false;
			mMobileView.onTouchUpCpp(mX1, mY1, 0);
			if (mbSensorOpen) {
				mSensorManager.registerListener(mSensorListener, mSensor,
						SensorManager.SENSOR_DELAY_GAME);
			}
			mUserActionStatus = UserStatus.USER_ACTION_TOUCH_UP;
			miTimeCount = 4;
			break;

		}
		return super.onTouchEvent(event);
	}

	private float spacingDist(MotionEvent event) {
		float x = event.getX(0) - event.getX(1);
		float y = event.getY(0) - event.getY(1);
		return FloatMath.sqrt(x * x + y * y);
	}

	public boolean dispatchTouchEvent(MotionEvent ev) {
		super.dispatchTouchEvent(ev);
		return false;
	}

	public boolean onKeyDown(int keyCode, KeyEvent event) {
		switch (keyCode) {
		case KeyEvent.KEYCODE_BACK:
			if (!bwSDK.onkeyBack()) {
				exitForBackButton();
			}
			return true;

		case KeyEvent.KEYCODE_DPAD_CENTER:
			mMobileView.onKeyDownCpp(0x0D, event);
			break;
		case KeyEvent.KEYCODE_DPAD_UP:
			mMobileView.onKeyDownCpp(0x26, event);
			break;
		case KeyEvent.KEYCODE_DPAD_DOWN:
			mMobileView.onKeyDownCpp(0x28, event);
			break;
		case KeyEvent.KEYCODE_DPAD_LEFT:
			mMobileView.onKeyDownCpp(0x25, event);
			break;
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			mMobileView.onKeyDownCpp(0x27, event);
			break;
		}
		return super.onKeyDown(keyCode, event);
	}

	public boolean processKeyBack() {
		InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
		boolean isOpen = imm.isActive();
		if (isOpen) {
			return false;
		}
		
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		boolean showOldMenu = settings.getBoolean("showOldMenu", true);
		
		if (showSoftInput && !bCallTextEdit) {// 处理软键盘弹出时点击Back物理按键的情况下，有的设备会调用onKeyUp,有的设备不调用的情况
			return true;
		}

		
		if ((mViewPopStatus == UserStatus.VIEW_POP_HELP || mViewPopStatus == UserStatus.VIEW_POP_PAY))

		{
			mMobileView.closePopView();
			mViewPopStatus = UserStatus.VIEW_POP_NONE;
			if (showOldMenu && !GlobalFun.showMenu) {
//				mMobileView.setBottomPos(mScreenHeight - mHeight44);
				if (mUserStatus == UserStatus.FLASH_TYPE_GAME) {
					showMenuGameOld();
				}
				if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA) {
					showMenuMediaOld();
				}
				if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
					showMenuWebGameOld();
				}
			}
			if (!showOldMenu) {
				HideMenu();
				showMenuFloatView();
			}
			if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
				mMobileView.setSilence(false);
			}

		} else if (mMenuStatus == UserStatus.MENU_BROWSER_POP) {
			HideMenuPopMore();
		} else if (mMenuStatus == UserStatus.MENU_MEDIA) {
			
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);

		} else if (mMenuStatus == UserStatus.MENU_GAME) {
			
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
		} else if (mMenuStatus == UserStatus.MENU_WEBGAME) {
			mMobileView.setSilence(false);
			Message msgTime = new Message();
			msgTime.what = MSG_SHOWFLOATVIEW;
			mProcessUseMsg.sendMessageDelayed(msgTime, 200);
		}
		
		else if (mMenuStatus == UserStatus.MENU_SETTING_FLASH) {
			mMenuUserSetting.setVisibility(View.GONE);
			backFromUserSetting();
		}
		
		else if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
				|| mUserStatus == UserStatus.FLASH_TYPE_GAME
				|| mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
			if (exitTimes >= 3) {
				exitTimes = 0;
				AppQuit();
			} else {
				menuExitFlash();
			}
		} else {
			AppQuit();
		}
		return false;
	}

	public boolean onKeyUp(int keyCode, KeyEvent event) {
		// Log.e("MobileMain","onKeyUp");
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		boolean showOldMenu = settings.getBoolean("showOldMenu", true);
		switch (keyCode) {
		case KeyEvent.KEYCODE_SEARCH:
			if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA
					|| mUserStatus == UserStatus.FLASH_TYPE_GAME
					|| mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
//				switchZooming();
			}
			return true;
		case KeyEvent.KEYCODE_BACK: {
			if (processKeyBack())
				return true;
			else
				break;
		}
		case KeyEvent.KEYCODE_DPAD_CENTER:
			mMobileView.onKeyUpCpp(0x0D, event);
			break;
		case KeyEvent.KEYCODE_DPAD_UP:
			mMobileView.onKeyUpCpp(0x26, event);
			break;
		case KeyEvent.KEYCODE_DPAD_DOWN:
			mMobileView.onKeyUpCpp(0x28, event);
			break;
		case KeyEvent.KEYCODE_DPAD_LEFT:
			mMobileView.onKeyUpCpp(0x25, event);
			break;
		case KeyEvent.KEYCODE_DPAD_RIGHT:
			mMobileView.onKeyUpCpp(0x27, event);
			break;

		case KeyEvent.KEYCODE_MENU: {
			//有的设备在点击Menu后不会自动隐藏输入软键盘
			if (showSoftInput && !bCallTextEdit) {
				mMobileView.SetXEdit(mEditText.getText().toString());
				InputMethodManager imm = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
				imm.hideSoftInputFromWindow(mEditText.getWindowToken(), 0);
				mLayoutMobileMain.removeView(mLayoutInput);
				showSoftInput = false;
				break;
			}
			// mScrollView.scrollBy(0, 50-dy);
			// showSoftInput = false;
			// 点击Menu键在相应的float菜单显示和隐藏之间切换
			if (mbMakeErrorPage || !mbOnsale ) 
			{
				break;
			} else if (mMenuStatus == UserStatus.MENU_BROWSER
					|| mMenuStatus == UserStatus.MENU_LOADING)
				break;
			else if (mMenuStatus == UserStatus.MENU_BROWSER_POP) {
				HideMenuPopMore();
			} else if ((mViewPopStatus == UserStatus.VIEW_POP_HELP || mViewPopStatus == UserStatus.VIEW_POP_PAY)) {
				mMobileView.closePopView();

				// 此时是否显示之前进入的菜单
				if (showOldMenu && !GlobalFun.showMenu) {
//					mMobileView.setBottomPos(mScreenHeight - mHeight44);
					if (mUserStatus == UserStatus.FLASH_TYPE_GAME) {
						showMenuGameOld();
					}
					if (mUserStatus == UserStatus.FLASH_TYPE_MEDIA) {
						showMenuMediaOld();
					}
					if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME) {
						showMenuWebGameOld();
					}
				} else if (!showOldMenu) {
					HideMenu();
					showMenuFloatView();
				}
				if (mUserStatus == UserStatus.FLASH_TYPE_WEBGAME)
					mMobileView.setSilence(false);
				mViewPopStatus = UserStatus.VIEW_POP_NONE;

			}
			// 显示menu菜单对当前游戏处理（设置，保存，退出）
			else if (mMenuStatus == UserStatus.MENU_FLOAT_VIEW
					|| mMenuStatus == UserStatus.MENU_NONE) {
				changeUserMenu();
			}
			// 隐藏menu菜单对当前游戏处理（设置，保存，退出）
			else if (mMenuStatus == UserStatus.MENU_MEDIA) {
				Message msgTime = new Message();
				msgTime.what = MSG_SHOWFLOATVIEW;
				mProcessUseMsg.sendMessageDelayed(msgTime, 200);
			} else if (mMenuStatus == UserStatus.MENU_GAME) {
				Message msgTime = new Message();
				msgTime.what = MSG_SHOWFLOATVIEW;
				mProcessUseMsg.sendMessageDelayed(msgTime, 200);
			} else if (mMenuStatus == UserStatus.MENU_WEBGAME) {
				Message msgTime = new Message();
				msgTime.what = MSG_SHOWFLOATVIEW;
				mProcessUseMsg.sendMessageDelayed(msgTime, 200);
			} else if (mMenuStatus == UserStatus.MENU_SETTING_FLASH) {
				mMenuUserSetting.setVisibility(View.GONE);
				backFromUserSetting();
			}

			else if (mMenuStatus == UserStatus.MENU_GAME_OLD) {
				if (GlobalFun.showMenu) {
//					mMenuGameOld.setVisibility(View.GONE);
					GlobalFun.showMenu = false;
					mMobileView.setBottomPos(mScreenHeight);
				} else {
//					mMenuGameOld.setVisibility(View.VISIBLE);
//					mMenuGameOld.bringToFront();
					GlobalFun.showMenu = true;
//					mMobileView.setBottomPos(mScreenHeight - mHeight44);
				}
			} else if (mMenuStatus == UserStatus.MENU_WEBGAME_OLD) {
				if (GlobalFun.showMenu) {
//					mMenuWebGameOld.setVisibility(View.GONE);
					GlobalFun.showMenu = false;
					mMobileView.setBottomPos(mScreenHeight);
				} else {
//					mMenuWebGameOld.setVisibility(View.VISIBLE);
//					mMenuWebGameOld.bringToFront();
					GlobalFun.showMenu = true;
//					mMobileView.setBottomPos(mScreenHeight - mHeight44);
				}
			} else if (mMenuStatus == UserStatus.MENU_MEDIA_OLD) {
				if (GlobalFun.showMenu) {
//					mMenuMediaOld.setVisibility(View.GONE);
					GlobalFun.showMenu = false;
					mMobileView.setBottomPos(mScreenHeight);
				} else {
//					mMenuMediaOld.setVisibility(View.VISIBLE);
//					mMenuMediaOld.bringToFront();
					GlobalFun.showMenu = true;
//					mMobileView.setBottomPos(mScreenHeight - mHeight44);
				}

			} else if (mMenuStatus == UserStatus.MENU_SETTING_FLASH) {
				mMenuUserSetting.setVisibility(View.GONE);
				backFromUserSetting();
			}
			break;
		} // end of KeyEvent.KEYCODE_MENU
		default:
			return super.onKeyUp(keyCode, event);

		} // endof switch
		return super.onKeyUp(keyCode, event);
	}

	public void onBackPressed() {
		super.onBackPressed();
	}

	public boolean onTrackballEvent(MotionEvent event) {
		super.onTrackballEvent(event);
		return false;
	}

	public boolean onCreateOptionsMenu(Menu menu) {
		super.onCreateOptionsMenu(menu);
		return true;
	}

	private void ClearBuffer() {
		Dialog dlg = new AlertDialog.Builder(MobileMain.this)
				.setIcon(R.drawable.logo_36)
				.setTitle("是否清除缓存？")
				.setPositiveButton(R.string.positive_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
								mMobileView.clearCache();
								Toast.makeText(MobileMain.this, "缓存清除完成",
										Toast.LENGTH_SHORT).show();
							}
						})
				.setNegativeButton(R.string.negative_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int which) {

							}
						}).create();

		dlg.show();
		return;
	}

	public void bwSDKQuit() {
		Dialog quitDlg = new AlertDialog.Builder(MobileMain.this)
				.setIcon(R.drawable.logo_36)
				.setTitle("游戏退出")
				.setPositiveButton(R.string.positive_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
								if (mMobileView.exitFlash()) {
									closeFlashView();
								}
								bwSDK.destory();
								mMobileView.onDomQuit();
								mMobileView.Release();
								nativeFinalize();
								unregisterReceiver(mUnlockReceiver);
								finish();
								android.os.Process
										.killProcess(android.os.Process.myPid());
							}
						}).setCancelable(false).create();

		quitDlg.show();

		return;
	}

	private void MediaGameQuit()
	{
		Dialog quitDlg = new AlertDialog.Builder(MobileMain.this)
				.setIcon(R.drawable.logo_36)
				.setTitle(R.string.app_exit)
				.setPositiveButton(R.string.positive_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
								if (mMobileView.exitFlash()) {
									closeFlashView();
								}
								bwSDK.destory();
								mMobileView.onDomQuit();
								mMobileView.Release();
								nativeFinalize();
								unregisterReceiver(mUnlockReceiver);
								finish();
								android.os.Process
										.killProcess(android.os.Process.myPid());
							}
						})
				.setNegativeButton(R.string.negative_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
							}
						}).create();

		quitDlg.show();

		return;
	}

	private void AppQuit() {
		Dialog quitDlg = new AlertDialog.Builder(MobileMain.this)
				.setIcon(R.drawable.logo_36)
				.setTitle(R.string.app_exit)
				.setPositiveButton(R.string.positive_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
								bwSDK.destory();
								mMobileView.onDomQuit();
								mMobileView.Release();
								nativeFinalize();
								unregisterReceiver(mUnlockReceiver);
								finish();
								android.os.Process
										.killProcess(android.os.Process.myPid());
							}
						})
				.setNegativeButton(R.string.negative_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
							}
						}).create();

		quitDlg.show();

		return;
	}

	public void MobileNative() {
		Intent intent = new Intent(this, LocalFlashAct.class);
		Bundle bl = new Bundle();
		bl.putString("LOCAL_PATH", GlobalFun.BWWORKPATH + "/Local");
		intent.putExtras(bl);
		startActivityForResult(intent, 0);
	}

	public void AboutUs() {
		Intent intent = new Intent(this, AboutActivity.class);
		
		String UA = getAppUA();
		String appHeaders = getAppHeaders();
		Bundle bl = new Bundle();
		bl.putString("UA", UA);
		String deviceID = Secure.getString(this.getContentResolver(),
				Secure.ANDROID_ID);
		bl.putString("DeviceID", deviceID);
		bl.putString("header", appHeaders);
		intent.putExtras(bl);
		startActivity(intent);
		
	}

	public void openInBrowser(String strUrl) {
		Intent intent = new Intent(this, BwWebView.class);
		Bundle bl = new Bundle();
		bl.putString("BW_URL", strUrl);
		intent.putExtras(bl);
		startActivity(intent);
	}

	public void CheckUpgrade() {
		mMobileView.checkUpgrade();
		Toast.makeText(this, "检测新版本", Toast.LENGTH_SHORT).show();
	}

	private int mbUpgradeStatus = UserStatus.UPGRADE_NOMAL;

	public void upgradeApp(String strUrl, int status) // 升级程序，由c调用
	{
		int ipos = strUrl.indexOf('|');
		if (mbUpgradeStatus == UserStatus.UPGRADE_CANCEL) {
			return;
		}
		if (ipos == -1) {
			if (status == 1) {
				mbUpgradeStatus = UserStatus.UPGRADE_CAN;
				final String url = strUrl;
				Dialog upDlg = new AlertDialog.Builder(MobileMain.this)
						.setIcon(R.drawable.logo_36)
						.setTitle("有新版本，请更新!")
						.setPositiveButton("确定",
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int whichButton) {
										openUpgradeUrl(url);
									}
								})
						.setNegativeButton("取消",
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int whichButton) {
										mbUpgradeStatus = UserStatus.UPGRADE_CANCEL;
									}
								}).create();
				upDlg.setOnKeyListener(new DialogInterface.OnKeyListener() {
					public boolean onKey(DialogInterface dialog, int keyCode,
							KeyEvent event) {
						if (keyCode == KeyEvent.KEYCODE_BACK)
							return true;
						else
							return false;
					}
				});

				upDlg.show();
			} else if (status == 2) {
				mbUpgradeStatus = UserStatus.UPGRADE_MUST;
				final String url = strUrl;
				Dialog upmustDlg = new AlertDialog.Builder(MobileMain.this)
						.setIcon(R.drawable.logo_36)
						.setTitle("有新版本，请更新!")
						.setPositiveButton("确定",
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int whichButton) {
										openUpgradeUrl(url);
									}
								}).create();

				upmustDlg.setOnKeyListener(new DialogInterface.OnKeyListener() {
					public boolean onKey(DialogInterface dialog, int keyCode,
							KeyEvent event) {
						if (keyCode == KeyEvent.KEYCODE_BACK)
							return true;
						else
							return false;
					}
				});

				upmustDlg.show();
			}
		} else {
			String updateTip = strUrl.substring(ipos + 1);
			String newtip = updateTip.replace('|', '\n');
			if (status == 1) {
				mbUpgradeStatus = UserStatus.UPGRADE_CAN;
				final String url = strUrl.substring(0, ipos);
				Dialog upDlg = new AlertDialog.Builder(MobileMain.this)
						.setIcon(R.drawable.logo_36)
						.setTitle("有新版本，请更新!")
						.setMessage(newtip)
						.setPositiveButton("确定",
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int whichButton) {
										openUpgradeUrl(url);
									}
								})
						.setNegativeButton("取消",
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int whichButton) {
										mbUpgradeStatus = UserStatus.UPGRADE_CANCEL;
									}
								}).create();
				upDlg.setOnKeyListener(new DialogInterface.OnKeyListener() {
					public boolean onKey(DialogInterface dialog, int keyCode,
							KeyEvent event) {
						if (keyCode == KeyEvent.KEYCODE_BACK)
							return true;
						else
							return false;
					}
				});

				upDlg.show();
			} else if (status == 2) {
				mbUpgradeStatus = UserStatus.UPGRADE_MUST;
				final String url = strUrl.substring(0, ipos);
				Dialog upmustDlg = new AlertDialog.Builder(MobileMain.this)
						.setIcon(R.drawable.logo_36)
						.setTitle("有新版本，请更新!")
						.setMessage(newtip)
						.setPositiveButton("确定",
								new DialogInterface.OnClickListener() {
									public void onClick(DialogInterface dialog,
											int whichButton) {
										openUpgradeUrl(url);
									}
								}).create();

				upmustDlg.setOnKeyListener(new DialogInterface.OnKeyListener() {
					public boolean onKey(DialogInterface dialog, int keyCode,
							KeyEvent event) {
						if (keyCode == KeyEvent.KEYCODE_BACK)
							return true;
						else
							return false;
					}
				});

				upmustDlg.show();
			}

		}
	}

	public void openViewUrl(String strUrl) {
		
		if (strUrl.contains("http://BW_DOMOB_OFFERWALL")) {
			String wares[] = strUrl.split("\\=");
			showDomobOfferWall(wares[1]);
			return;
		}
		final String str = strUrl;
		Dialog dlg = new AlertDialog.Builder(MobileMain.this)
				.setIcon(R.drawable.logo_36)
				.setTitle("程序将打开浏览器,并转到后台运行,是否继续")
				.setPositiveButton(R.string.positive_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
								
								Intent viewIntent = new Intent(
										"android.intent.action.VIEW", Uri
												.parse(str));
								startActivity(viewIntent);
							}
						})
				.setNegativeButton(R.string.negative_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int which) {
							}
						}).create();
		dlg.show();
	}

	public void openUpgradeUrl(String strUrl) {
		Intent intent = new Intent(MobileMain.this, UpgradeService.class);
		File file = null;
		file = new File(GlobalFun.BWWORKPATH + "/tmp.file");
		FileOutputStream fout = null;
		try {
			file.createNewFile();
			fout = new FileOutputStream(file);
			byte buf[] = strUrl.getBytes();
			fout.write(buf, 0, buf.length);
			fout.close();
		} catch (IOException e) {
			Log.e("tag", e.getMessage());
		}
		startService(intent);

		mMobileView.onDomQuit();
		mMobileView.Release();
		nativeFinalize();
		unregisterReceiver(mUnlockReceiver);
		finish();
		return;

	}

	public void HelpMsg() {
		Intent intent = new Intent(this, HelpActivity.class);
		startActivity(intent);
	}

	private ProgressDialog mDlgSaveMedia = null;// 保存flash进度条

	/*
	 * 保存多媒体文件
	 */
	private void saveMediaFile() {
		if (!mMobileView.canFlashSave())
			return;

		mDlgSaveMedia = new ProgressDialog(this);
		mDlgSaveMedia.setIcon(R.drawable.logo_36);
		mDlgSaveMedia.setTitle("百玩游戏");
		mDlgSaveMedia.setMessage("游戏保存中，请稍候...");
		mDlgSaveMedia.setIndeterminate(true);
		mDlgSaveMedia.setCancelable(true);
		ColorDrawable draw = new ColorDrawable(0xFFFF0000);
		mDlgSaveMedia.setProgressDrawable(draw);
		mDlgSaveMedia.show();
		mMobileView.flashSave();
		return;
	}

	/*
	 * flash 保存完成时，被c调用
	 */
	public void flashSaveFinish() 
	{
		mDlgSaveMedia.cancel();
		mDlgSaveMedia = null;
		Toast.makeText(this, "保存成功", Toast.LENGTH_SHORT).show();
		System.gc();
	}

	
	private Button mBtnEdit = null;
	private int dy = 0;
	private boolean showSoftInput = false;
	private boolean bCallTextEdit = false;// 辅助判断是否是本次调用
	private int callTimeCount = 0;;

	/*
	 * XEdit回调该函数，处理用户输入信息
	 */
	public void CallTextEdit(String inText, int ilength, int style) {
		if (!showSoftInput) {
			int temp = mScreenHeight * 80 / 100;
			dy = mY1;
			if (dy > temp) {
				dy = temp;
			} else if (dy <= 50) {
				dy = 50;
			}
			showSoftInput = true;
			bCallTextEdit = true;

			isSetXEdit = false;

			if (mLayoutInput == null || !mLayoutInput.isShown()) {
				mEditText = new EditText(this);
				mSure = new Button(this);
				mSure.setText("完成");
				//强制设置为非全屏
				mEditText.setImeOptions(EditorInfo.IME_FLAG_NO_EXTRACT_UI);
				// 设置编辑框为单行模式
				mEditText.setSingleLine(true);

				LinearLayout.LayoutParams layoutParamEdit = new LinearLayout.LayoutParams(
						mScreenWidth - 100,
						LinearLayout.LayoutParams.WRAP_CONTENT);
				LinearLayout.LayoutParams layoutParamButton = new LinearLayout.LayoutParams(
						100, LinearLayout.LayoutParams.WRAP_CONTENT);

				mEditText.setText(inText);

				mEditText.setLayoutParams(layoutParamEdit);
				mSure.setLayoutParams(layoutParamButton);
				mLayoutInput = new LinearLayout(this);
				mLayoutInput.addView(mEditText);
				mLayoutInput.addView(mSure);
				mLayoutMobileMain.addView(mLayoutInput);
			}

			mEditText.requestFocus();
			InputMethodManager imm = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
			imm.showSoftInput(mEditText, 0);
			mEditText
					.setOnEditorActionListener(new EditText.OnEditorActionListener() {
						@Override
						public boolean onEditorAction(TextView v, int actionId,
								KeyEvent event) {
							// TODO Auto-generated method stub

							if (actionId == EditorInfo.IME_ACTION_DONE
									|| actionId == EditorInfo.IME_ACTION_NEXT) {
								hideSoftKeyBord();
								return true;
							}
							return false;
						}
					});

			mSure.setOnClickListener(new View.OnClickListener() {

				@Override
				public void onClick(View v) {
					// TODO Auto-generated method stub
					hideSoftKeyBord();
					if (mLayoutInput != null) {
						mLayoutInput.setVisibility(View.GONE);
					}
				}
			});

		}
	}

	class AnimationParam {
		View mView;
		long mDuration;
		float mTranFromX;
		float mTranToX;
		float mTranFromY;
		float mTranToY;
		float mScaleFromX;
		float mScaleToX;
		float mScaleFromY;
		float mScaleToY;
	}

	private final static int CENTER_XL = 140;
	private final static int CENTER_XR = 160;
	private final static int CENTER_Y = 100;

	private void btnMoveScreen(int x, int y) {
		mMobileView.onTouchDownCpp(CENTER_XL, CENTER_Y, 0);
		mMobileView.onTouchDownCpp(CENTER_XR, CENTER_Y, 1);

		mMobileView.onTouchMoveCpp(CENTER_XL, CENTER_Y, 0);
		mMobileView.onTouchMoveCpp(CENTER_XR, CENTER_Y, 1);

		mMobileView.onTouchMoveCpp(CENTER_XL + x, CENTER_Y + y, 0);
		mMobileView.onTouchMoveCpp(CENTER_XR + x, CENTER_Y + y, 1);

		mMobileView.onTouchMoveCpp(CENTER_XL + x * 2, CENTER_Y + y * 2, 0);
		mMobileView.onTouchMoveCpp(CENTER_XR + x * 2, CENTER_Y + y * 2, 1);

		mMobileView.onTouchUpCpp(CENTER_XL + x * 2, CENTER_Y + y * 2, 0);
		mMobileView.onTouchUpCpp(CENTER_XR + x * 2, CENTER_Y + y * 2, 1);
	}

	private int miTimeCount = 0; // 定时器计数

	private void timeTickActoin() // 处理用户操作状态
	{
		timeCount += 1;
		if (timeCount == 20) {
			timeCount = 0;
			Time time = new Time();
			time.setToNow();
			int minute = time.minute;
			int hour = time.hour;
		
		}
		if (mViewPopStatus == UserStatus.VIEW_POP_PAY && !mMobileView.isPause()) {
			mMobileView.setSilence(false);
		}

		if (mMenuStatus == UserStatus.MENU_FLOAT_VIEW) {
			if (mIFloatViewCount > 0) {
				mIFloatViewCount--;
			} else if (mIFloatViewCount == 0) {

				mIFloatViewCount--;
			}
		}

		if (mUserActionStatus == UserStatus.USER_ACTION_NONE)
			return;
		if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_POINTER_UP) {
			if (miTimeCount > 0) {
				miTimeCount--;
			} else {
				miTimeCount = 0;
				mUserActionStatus = UserStatus.USER_ACTION_NONE;
			}
		} else if (mUserActionStatus == UserStatus.USER_ACTION_TOUCH_UP) {
			if (miTimeCount > 0) {
				miTimeCount--;
			} else {
				miTimeCount = 0;
				mMobileView.invalidateView1(mX1, mY1, false);
				mUserActionStatus = UserStatus.USER_ACTION_NONE;
			}
		} else if (mUserActionStatus == UserStatus.USER_ACTION_BTN_UP_DOWN) {
			if (miTimeCount == TIME_GAP) {
				btnMoveScreen(0, 30);
			}
			miTimeCount--;
			if (miTimeCount == 0) {
				miTimeCount = TIME_GAP;
			}

		} else if (mUserActionStatus == UserStatus.USER_ACTION_BTN_DOWN_DOWN) {
			if (miTimeCount == TIME_GAP) {
				btnMoveScreen(0, -30);
			}
			miTimeCount--;
			if (miTimeCount == 0) {
				miTimeCount = TIME_GAP;
			}
		} else if (mUserActionStatus == UserStatus.USER_ACTION_BTN_LEFT_DOWN) {
			if (miTimeCount == TIME_GAP) {
				btnMoveScreen(30, 0);
			}
			miTimeCount--;
			if (miTimeCount == 0) {
				miTimeCount = TIME_GAP;
			}
		} else if (mUserActionStatus == UserStatus.USER_ACTION_BTN_RIGHT_DOWN) {
			if (miTimeCount == TIME_GAP) {
				btnMoveScreen(-30, 0);
			}
			miTimeCount--;
			if (miTimeCount == 0) {
				miTimeCount = TIME_GAP;
			}
		}
	} // end of processUserActoin()

	private void initCPP()
	{
		String str14 = getString(R.dimen.bwpx14);// 21.0px
		str14 = str14.substring(0, str14.indexOf("."));
		miMoveShreshole = Integer.parseInt(str14);
		String str44 = getString(R.dimen.bwpx44);
		str44 = str44.substring(0, str44.indexOf("."));
		mHeight44 = Integer.parseInt(str44);

		String str64 = getString(R.dimen.bwpx64);
		str64 = str64.substring(0, str64.indexOf("."));
		GlobalFun.BWStartPoint = Integer.parseInt(str64);

		String maxCpu = this.getMaxCpuFreq();
		// cpu最高频率单位转换
		double dMaxCpu = Double.parseDouble(maxCpu);
		double temp = dMaxCpu;
		String suffix = "KHz";

		if (dMaxCpu >= 1000) {
			temp = dMaxCpu / 1000.0;
			suffix = "MHz";
			if (temp >= 1000) {
				suffix = "GHz";
				temp /= 1000.0;
			}
		}

		java.text.DecimalFormat df = new java.text.DecimalFormat("#0.#");
		StringBuilder result = new StringBuilder(df.format(temp));
		result.append(suffix);
		//CPU:1.2GHz,RAM:800M 按照这种方式
		//CPU:1.2GHz,RAM:800MB 目前是这种格式
		// String ram = this.getTotalMemory();
		mCpu = result.toString();
		mRam = this.getTotalMemory();
		mDpi = this.getMobileDpi();
		nativeInit(mScreenWidth, mScreenHeight,
				android.os.Build.VERSION.SDK_INT, mHeight44);
	}

	public void loadMenuBrowserPhone() // 浏览器菜单
	{
		mMenuBrowser = findViewById(R.id.menubrowser);
		//动态设置底部工具栏的高度
//		FrameLayout mFrameBrowser=(FrameLayout) findViewById(R.id.frame_browser);
//		LinearLayout.LayoutParams p=(android.widget.LinearLayout.LayoutParams) mFrameBrowser.getLayoutParams();
//		if(mScreenWidth>mScreenHeight){
//			p.height=(int) (mScreenHeight*0.12);
//		}else{
//			p.height=(int) (mScreenWidth*0.12);
//		}
//		mFrameBrowser.setLayoutParams(p);
		
		mBtnMenuHome = (Button) findViewById(R.id.menu_home);// home图标
		btnMenuHomeEnabled(false);
		mBtnMenuHome.setOnClickListener(new OnClickListener() {
			public void onClick(View v) 
			{
				// 隐藏pop菜单
				HideMenuPopMore();
				if( mbPageView )
				{
					mPageView.loadUrl( mStrWebburl );
					btnMenuForwardEnabled( mPageView.canGoForward() );
					btnMenuBackEnabled( mPageView.canGoBack() );
				}
				else
				{
					mMobileView.onDomHome();
					// home点击状态
					btnMenuHomeEnabled(!mMobileView.isCurrentHomePage());
					//
					btnMenuForwardEnabled(mMobileView.canForward());
					// back点击状态
					btnMenuBackEnabled(mMobileView.canBack());
				}
			}
		});

		mBtnMenuBack = (Button) findViewById(R.id.menu_back);
		btnMenuBackEnabled(false);
		mBtnMenuBack.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				HideMenuPopMore();
				if( mbPageView )
				{
					mPageView.goBack();
				}
//				else if (mMenuStatus == UserStatus.MENU_LOADING)
//				{
//					mMobileView.onDomStop();
//					stopProgressLoading();
//				} 
				else
				{
					mMobileView.onDomBack();
					btnMenuHomeEnabled(!mMobileView.isCurrentHomePage());
					btnMenuForwardEnabled(mMobileView.canForward());
					btnMenuBackEnabled(mMobileView.canBack());
				}
			}
		});

		mBtnMenuRefresh = (Button) findViewById(R.id.menu_fresh);
		mBtnMenuRefresh.setOnClickListener(new OnClickListener() 
		{
			public void onClick(View v) 
			{
				HideMenuPopMore();
				if( mbPageView )
				{
					mPageView.reload();
				}
				else if (mMenuStatus == UserStatus.MENU_LOADING)
				{
					mMobileView.onDomStop();
					stopProgressLoading();
					mBtnMenuRefresh.setEnabled(false);
				} 
				else
				{
					mMobileView.onDomUpdate();
					mBtnMenuRefresh.setEnabled(false);
				}
			}
		});
		// “清除缓存”按钮的点击事件
		Button btnMenuEliminate = (Button) findViewById(R.id.menu_elimiate);
		btnMenuEliminate.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// HideMenuPopMore();
				ClearBuffer();
			}
		});
		Button btnMenuAbout = (Button) findViewById(R.id.menu_about);
		btnMenuAbout.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// HideMenuPopMore();
				
				
				//发布时用此功能
				AboutUs();
				isAbout=true;
				//测试时用此功能
//				HideMenuPopMore();
//				MobileNative();
			}
		});
		mProgressLoading = (ProgressBar) findViewById(R.id.menu_loading_progress);
	}

	public void loadMenuDirection() // 方向建菜单
	{
		mMenuDirection = findViewById(R.id.menudirection);
		Button btnMenuDirectionUp = (Button) findViewById(R.id.menu_direction_up);
		btnMenuDirectionUp.setOnTouchListener(new OnTouchListener() {
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
					mUserActionStatus = UserStatus.USER_ACTION_BTN_UP_DOWN;
					miTimeCount = TIME_GAP;
				} else if (event.getAction() == MotionEvent.ACTION_UP) {
					mUserActionStatus = UserStatus.USER_ACTION_BTN_UP_UP;
					miTimeCount = 0;
				}
				return false;
			}
		});

		Button btnMenuDirectionDown = (Button) findViewById(R.id.menu_direction_down);
		btnMenuDirectionDown.setOnTouchListener(new OnTouchListener() {
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
					mUserActionStatus = UserStatus.USER_ACTION_BTN_DOWN_DOWN;
					miTimeCount = TIME_GAP;
				} else if (event.getAction() == MotionEvent.ACTION_UP) {
					mUserActionStatus = UserStatus.USER_ACTION_BTN_DOWN_UP;
					miTimeCount = 0;
				}
				return false;
			}
		});
		Button btnMenuDirectionLeft = (Button) findViewById(R.id.menu_direction_left);
		btnMenuDirectionLeft.setOnTouchListener(new OnTouchListener() {
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
					mUserActionStatus = UserStatus.USER_ACTION_BTN_LEFT_DOWN;
					miTimeCount = TIME_GAP;
				} else if (event.getAction() == MotionEvent.ACTION_UP) {
					mUserActionStatus = UserStatus.USER_ACTION_BTN_LEFT_UP;
					miTimeCount = 0;
				}
				return false;
			}
		});

		Button btnMenuDirectionRight = (Button) findViewById(R.id.menu_direction_right);
		btnMenuDirectionRight.setOnTouchListener(new OnTouchListener() {
			public boolean onTouch(View v, MotionEvent event) {
				if (event.getAction() == MotionEvent.ACTION_DOWN) {
					mUserActionStatus = UserStatus.USER_ACTION_BTN_RIGHT_DOWN;
					miTimeCount = TIME_GAP;
				} else if (event.getAction() == MotionEvent.ACTION_UP) {
					mUserActionStatus = UserStatus.USER_ACTION_BTN_RIGHT_UP;
					miTimeCount = 0;
				}
				return false;
			}
		});
	}

//	public void loadMenuMediaOld() {// 旧游戏菜单事件处理
//		mMenuMediaOld = findViewById(R.id.menumedia);
//		mImgProgressMediaOld = (ImageView) findViewById(R.id.img_progress_media_old);
//		mImgProgressMediaOld.setBackgroundResource(R.anim.img_progress);
//		mMenuMediaOld.setOnLongClickListener(new View.OnLongClickListener() {
//
//			public boolean onLongClick(View v) {
//				// TODO Auto-generated method stub
//				if (mbMenuMediaOldMove) {
//					return false;
//				}
//				return false;
//			}
//		});
//		mMenuMediaOld.setOnTouchListener(new OnTouchListener() {
//
//			public boolean onTouch(View v, MotionEvent event) {
//				// TODO Auto-generated method stub
//				int ea = event.getAction();
//				switch (ea) {
//				case MotionEvent.ACTION_DOWN:
//					mbMenuMediaOldMove = false;
//					mMenuMediaOldMoveY = (int) event.getRawY();
//					break;
//				case MotionEvent.ACTION_MOVE:
//					if (GlobalFun.showMenu) {
//						int dy = (int) event.getRawY() - mMenuMediaOldMoveY;
//						if (Math.abs(dy) > miMoveShreshole) {
//							mbMenuMediaOldMove = true;
//						} else {
//							break;
//						}
//					}
//					break;
//				case MotionEvent.ACTION_UP:
//					if (mbMenuMediaOldMove) {
//						mbMenuMediaOldMove = false;
//						int posx = (int) event.getRawX();
//						int posy = (int) event.getRawY();
//						SharedPreferences settings = getSharedPreferences(
//								GlobalFun.BW_CONFIGURE_FILE, 0);
//						Editor editor = settings.edit();// 获取编辑器
//						editor.putBoolean("showOldMenu", false);
//						editor.putInt("USER_POS_X", posx);
//						editor.putInt("USER_POS_Y", posy);
//						editor.commit();
//						HideMenu();
//						mMenuMediaOld.setVisibility(View.GONE);
//						// mMobileView.setTopPos( 0 );
//						mMobileView.setBottomPos(mScreenHeight);
//						showMenuFloatView();
//					}
//					break;
//				}
//				return false;
//			}
//		});
//		mMenuMediaSwitchOld = (Button) findViewById(R.id.menu_switch_media_old);
//		mMenuMediaSwitchOld.setOnClickListener(new OnClickListener() {
//
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				SharedPreferences settings = getSharedPreferences(
//						GlobalFun.BW_CONFIGURE_FILE, 0);
//				Editor editor = settings.edit();// 获取编辑器
//				editor.putBoolean("showOldMenu", false);
//				editor.putInt("USER_POS_X", mScreenWidth - 2
//						* mBtnFloatImgWidth);
//				editor.putInt("USER_POS_Y", 20);
//				editor.commit();
//				boolean firstSwitchToNew = settings.getBoolean(
//						"firstSwitchToNew", true);
//				if (firstSwitchToNew) {
//					displayTips("可通过向下滑动菜单进行切换");
//					editor.putBoolean("firstSwitchToNew", false);
//					editor.commit();
//				}
//				HideMenu();
//				mMenuMediaOld.setVisibility(View.GONE);
//				// mMobileView.setTopPos( 0 );
//				mMobileView.setBottomPos(mScreenHeight);
//				showMenuFloatView();
//			}
//		});
//		mMenuMediaZoomOld = (Button) findViewById(R.id.menu_zooming_media_old);
//		mMenuMediaZoomOld.setOnClickListener(new OnClickListener() {
//
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				switchZooming();
//			}
//		});
//		mMenuMediaSettingOld = (Button) findViewById(R.id.menu_setting_media_old);
//		mMenuMediaSettingOld.setOnClickListener(new OnClickListener() {
//
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				mMenuStatus = UserStatus.MENU_SETTING_FLASH;
//				showSettingFromFlash();
//				mMobileView.setBottomPos(mScreenHeight);
//				mMenuMediaOld.setVisibility(View.GONE);
//			}
//		});
//		mMenuMediaSaveOld = (Button) findViewById(R.id.menu_save_media_old);
//		mMenuMediaSaveOld.setEnabled(false);
//		mMenuMediaSaveOld
//				.setBackgroundResource(R.drawable.bar_save_untouch_old);
//		mMenuMediaSaveOld.setOnClickListener(new OnClickListener() {
//
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				saveMediaFile();
//			}
//		});
//		mMenuMediaCloseOld = (Button) findViewById(R.id.menu_close_media_old);
//		mMenuMediaCloseOld.setOnClickListener(new OnClickListener() {
//
//			public void onClick(View v) {
//				// TODO Auto-generated method stub
//				if (exitTimes >= 3) {
//					exitTimes = 0;
//					AppQuit();
//				} else {
//					menuExitFlash();
//				}
//			}
//		});
//	}

	private String getAvailMemory() {
		// 获取android当前可用内存大小
		ActivityManager am = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
		MemoryInfo mi = new MemoryInfo();
		am.getMemoryInfo(mi);
		// mi.avaiMem;当前系统可用内存
		return Formatter.formatFileSize(getBaseContext(), mi.availMem);
		// 将获得的内存大小规格化
	}

	// 获得dpi屏幕每英寸的点数
	private String getMobileDpi() {
		DisplayMetrics metrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metrics);
		String mobileDpi = metrics.densityDpi + "";
		return mobileDpi;

	}

	private String getTotalMemory() {
		String str1 = "/proc/meminfo";// 系统内存信息文件
		String str2;
		String[] arrayOfString;
		long initial_memory = 0;

		try {
			FileReader localFileReader = new FileReader(str1);
			BufferedReader localBufferedReader = new BufferedReader(
					localFileReader, 8192);
			str2 = localBufferedReader.readLine();// 读取meminfo第一行，系统内存大小
			arrayOfString = str2.split("\\s+");
			for (String num : arrayOfString) {
				Log.i(str2, num + "\t");
			}
			initial_memory = Integer.valueOf(arrayOfString[1]).intValue() * 1024;// 获得系统总内存，单位KB
			localBufferedReader.close();
		} catch (IOException e) {

		}
		return Formatter.formatFileSize(getBaseContext(), initial_memory);
		// Byte转位KB或MB

	}

	// 获取CPU最大频率（单位KHZ）
	// "/system/bin/cat" 命令行
	// "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq" 存储最大频率的文件的路径
	public String getMaxCpuFreq() {
		String result = " ";
		ProcessBuilder cmd;
		try {
			String[] args = { "/system/bin/cat",
					"/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq" };
			cmd = new ProcessBuilder(args);
			Process process = cmd.start();
			InputStream in = process.getInputStream();
			byte[] re = new byte[24];
			while (in.read(re) != -1) {
				result = result + new String(re);
			}
			in.close();
		} catch (IOException ex) {
			ex.printStackTrace();
			result = " ";
		}
		return result.trim();
	}

	// 获取CPU最小频率（单位KHZ）
	public String getMinCpuFreq() {
		String result = "";
		ProcessBuilder cmd;
		try {
			String[] args = { "/system/bin/cat",
					"/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_min_freq" };
			cmd = new ProcessBuilder(args);
			Process process = cmd.start();
			InputStream in = process.getInputStream();
			byte[] re = new byte[24];
			while (in.read(re) != -1) {
				result = result + new String(re);
			}
			in.close();
		} catch (IOException ex) {
			ex.printStackTrace();
			result = "N/A";
		}
		return result.trim();
	}

	// 实时获取CPU当前频率（单位KHZ）
	public String getCurCpuFreq() {
		String result = "N/A";
		try {
			FileReader fr = new FileReader(
					"/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
			BufferedReader br = new BufferedReader(fr);
			String text = br.readLine();
			result = text.trim();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return result;
	}

	// 取得系统版本
	public String getReleaseVersion() {
		return android.os.Build.VERSION.RELEASE;
	}

	// 取得设备DPI信息
	public int getDPI() {
		DisplayMetrics dm = new DisplayMetrics();
		dm = getResources().getDisplayMetrics();
		// 屏幕密度（120/160/240/320）
		return dm.densityDpi;

	}

	public void setLandscape() {
		mViewLayout.setOrientationLandscape( );
	}

	public void setPortrait() {
		mViewLayout.setOrientationPortrait( );
	}

	// 记录屏幕初始时屏幕方向
	public int getOrientation() 
	{
		if (mCurOrientation == 1)
		{
			mOrientation = 1;
		}
		else if (mCurOrientation == 0) 
		{
			mOrientation = 0;
		}
		return mOrientation;

	}
	
	
	public static Paint sPaint;
	private static int mFontSize;
	public void setFont( int nSize )
	{
		sPaint = new Paint();
		String fontType = "宋体"; 
		Typeface  typeface = Typeface.create( fontType, Typeface.NORMAL );
		sPaint.setTypeface(typeface);
		sPaint.setAntiAlias( true );
		sPaint.setColor( 0XFFFFFFFF );
		sPaint.setTextSize( nSize );
		mFontSize = nSize;
		return;
	}
	
	public void drawString( String str )
	{
		//Log.e("drawString", "str=" + str);
		if( str == null )
			return;

		float f = sPaint.getTextSize();
		FontMetrics sf = sPaint.getFontMetrics();

		int height = ((int)Math.ceil(sf.descent - sf.ascent))-4;

		int width = (int)sPaint.measureText( str );
		width = (width+3)/4*4;
		if(width > 0 && height > 0 )
		{
		    Bitmap bitmap = Bitmap.createBitmap(width, height+2, Bitmap.Config.ALPHA_8 );
		    Canvas canvas = new Canvas( bitmap );
		    canvas.drawText( str, 0, height, sPaint );
		    //final byte[] pixels = getPixels( bitmap );
		    //setTxtBitData( pixels, width, height );
		    setTxtBmpData( bitmap, width, height+2 );
		    bitmap.recycle();
		}
		return;
	}
	
	private static byte[] getPixels(final Bitmap pBitmap)
	{
		if (pBitmap != null) 
		{
			final byte[] pixels = new byte[pBitmap.getWidth()
					* pBitmap.getHeight()];
			final ByteBuffer buf = ByteBuffer.wrap(pixels);
			buf.order(ByteOrder.nativeOrder());
			pBitmap.copyPixelsToBuffer(buf);
			return pixels;
		}
		return null;
	}
	
	public native void setTxtBmpData( Bitmap bitmap, int width, int height );
	
	public native void setTxtBitData( final byte[] bits, int width, int height );

	private native final void nativeInit(int width, int height, int iversion, int browerHeight);

	private native final void nativeFinalize();

	private native final void copyFontFile(AssetManager assetManager);

	

	static {
		
		try {
			System.loadLibrary("GGlibrary");
		} catch (UnsatisfiedLinkError ule) {
			Log.e("GGlibrary error", "Could not load library GGlibrary, Model:"
					+ android.os.Build.MODEL + ", SDK:"
					+ android.os.Build.VERSION.SDK + ", Release:"
					+ android.os.Build.VERSION.RELEASE);
		}
	}

	public void changeScreenDir( int mode )
	{
	Log.i("tag", "mode="+mode);
		if( mode == 0 )
		{
			if( mOrientation == mCurOrientation )
				return;
			setRequestedOrientation(mOrientation); 
			mCurOrientation = mOrientation;
		}
		else if( mode == 1 )
		{
			if( mOrientation == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE )
			{
				setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_PORTRAIT );
				mCurOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
			}
			else if( mOrientation == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT )
			{
				setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE );
				mCurOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
			}
		}
		else if ( mode == -1 )
		{
			if( mCurOrientation == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE )
			{
				setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_PORTRAIT );
				mCurOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
			}
			else if( mCurOrientation == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT )
			{
				setRequestedOrientation( ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE );
				mCurOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
			}
		}
		getOrientationHW();
		mMobileView.setScreenWHJava(mScreenWidth,mScreenHeight);
		
		//重新设置小球位置
		mArcLayout.requestLayout();
		return;
	}

	public void showBarandsale(int type)
	{
		mbOnsale = ((type & 01) == 0) ? false : true;
		mbShowBar = ((type & 0x2) == 0 ) ? false : true;
		mbOrientation = ((type & 0x10) == 0 ) ? false : true;
		// 保存网络设置
		SharedPreferences settings = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, 0);
		Editor editor = settings.edit();


		editor.putBoolean("mbOnsale", mbOnsale);
		editor.putBoolean("mbShowBar", mbShowBar);
		editor.putBoolean("mbOrientation", mbOrientation);

		editor.commit();		
		
//		Log.e("showBarandsale", "mbOnsale       "+mbOnsale);
//		Log.e("showBarandsale", "mbShowBar      "+mbShowBar);
//		Log.e("showBarandsale", "mbOrientation  "+mbOrientation);

	}

	private void GetSavedPreferences()
	{
		SharedPreferences settings = getSharedPreferences( GlobalFun.BW_CONFIGURE_FILE, 0);
		mbOnsale = settings.getBoolean("mbOnsale", true );
		mbShowBar = settings.getBoolean("mbShowBar", true );
		mbOrientation = settings.getBoolean("mbOrientation", false);
		mCurOrientation = mbOrientation ? ActivityInfo.SCREEN_ORIENTATION_PORTRAIT:ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;

//		Log.e("GetSavedPreferences", "mbOnsale       "+mbOnsale);
//		Log.e("GetSavedPreferences", "mbShowBar      "+mbShowBar);
//		Log.e("GetSavedPreferences", "mbOrientation  "+mbOrientation);
	}

	private void ResetOrientation()
	{
		setRequestedOrientation( mCurOrientation);
		getOrientationHW();
        //添加启动动画布局
		ScaleUtil.init(context);
		initImageSize();
		addView(ITEM_DRAWABLES_BOTTOM,getRequestedOrientation());
		
		mMobileView.setScreenWHJava(mScreenWidth, mScreenHeight);
	}
	
	// type 1 表示网游， 0表示精品单机, 2  表示审核状态下的flash单机游戏
	private void PlayFlashGame(int type)
	{
		if( type == 0 )
		{
			mMobileView.onDomStop();
			mMobileView.setBottomPos(mScreenHeight);
			mMobileView.connectUrl(mStrQualityFile);
		}
		else if( type == 2 )
		{
			mMobileView.onDomStop();
			mMobileView.setBottomPos(mScreenHeight);
			mMobileView.connectUrl( mStrLocalFile );
		}
	}
	
	public void MakeErrorPage()
	{
		mbMakeErrorPage = true;

	}

	

	public void WXSendText(String pdata, int type) {
	}

	public void WXSendPhoto(byte[] pdata1, int pdata2, int type) {
		if (pdata1 != null) {
		}
	}

	public void WXSendLink(String title, String description, byte[] pdata1,
			String weburl, int type) {
	}

	public void WXSendMusic(String title, String description, byte[] pdata1,
			String musicurl, String musicdateUrl, int type) {
	}

	public void WXSendVideo(String title, String description, byte[] pdata1,
			String videourl, int type) {
	}

	public void WXSendAppMessage(String title, String description,
			byte[] pdata1, String extInfo, String url, byte[] pdata2, int type) {
	}

	// 微信是否安装
	public boolean WeiXinInStalled() {
		return false;
	}

	public void WXSendNoGif(byte[] pdata1, byte[] pdata2, int type) {
	}

	public void WXSendGif(byte[] pdata1, byte[] pdata2, int type) {
	}

	public void WXSendFile(String title, String description, byte[] pdata1,
			byte[] pdata2, String fileExt, int type) {
	}

	public void LoadPayUrl(String url) {
		final String str = url;
		Dialog dlg = new AlertDialog.Builder(MobileMain.this)
				.setIcon(R.drawable.logo_36)
				.setTitle("程序将打浏览器充值,是否继续")
				.setPositiveButton(R.string.positive_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int whichButton) {
								// openInBrowser(str);
								Intent viewIntent = new Intent(
										"android.intent.action.VIEW", Uri
												.parse(str));
								startActivity(viewIntent);
							}
						})
				.setNegativeButton(R.string.negative_button,
						new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog,
									int which) {
							}
						}).create();
		dlg.show();

	}

	public void showDomobOfferWall(String userId) {
		showOfferWall(userId);
	}

	// 读取asset目录下图片文件为Drawable
	public static Drawable getDrawableFromAssets(Context context,
			String imageFileName) {
		Drawable result = null;
		AssetManager assetManager = context.getAssets();
		InputStream is = null;
		try {
			is = assetManager.open(imageFileName);
			result = Drawable.createFromStream(is, null);
			is.close();
			is = null;
		} catch (IOException e) {
			e.printStackTrace();
		}
		return result;
	}

	public void NoticePayUrl(String url) {
		SharedPreferences settings = getSharedPreferences(
				GlobalFun.BW_CONFIGURE_FILE, 0);
		Editor editor = settings.edit();
		editor.putString("noticeurl", url);
		editor.commit();
		if (url.contains("*"))
		{
			String wares[] = url.substring(7).split("\\/");
			String temps[] = wares[0].split("\\*");
			if (temps[0].equals("")) {
				zfb.setChargeView_width(mScreenWidth);
			} else {
				zfb.setChargeView_width(Integer.parseInt(temps[0]));
			}
			if (temps[1].equals("")) {
				zfb.setChargeView_height(mScreenHeight);
			} else {
				zfb.setChargeView_height(Integer.parseInt(temps[1]));
			}
			mStrWebvurl = url.substring(0, 7) + temps[2];
		}
		else
		{
			mStrWebvurl = url;
		}
		mStrWebvurl = mStrWebvurl.replaceFirst("webv", "http");
	}

	public void WebGameRecharge()
	{
		Intent intent = new Intent(this, ChargeWebView.class);
		Bundle bl = new Bundle();
		bl.putString("BW_WEBV", mStrWebvurl );
		intent.putExtras( bl );
		startActivity(intent);
	}

	public void openWebViewUrl(String strUrl, int proto)
	{
		
		
		System.out.println("strUrl:" + strUrl);
	
		if( proto == 2 )
		{
			ShowMobileView( false );
			ShowPageWebView( true );
			mbPageView = true;
			strUrl = strUrl.replaceFirst("webb", "http");
			mPageView.connectUrl( strUrl );
		}
		else if( proto == 3 )
		{
			NoticePayUrl(strUrl);
			WebGameRecharge();
		}
		return;
		
	}
	
	public void OpenGamePage( String strUrl )
	{
		ShowPageWebView( false );
		ShowMobileView( true );
		mMobileView.connectUrl( strUrl );
		return;
	}

//	@Override
//	public void surfaceCreated(SurfaceHolder holder) {
//		// TODO Auto-generated method stub
//		
//	}
//
//	@Override
//	public void surfaceChanged(SurfaceHolder holder, int format, int width,
//			int height) {
//		// TODO Auto-generated method stub
//		Log.e("surfaceChanged", "1111");
//		nativeSetSurface(holder.getSurface(),width,height);
//	}
//
//	@Override
//	public void surfaceDestroyed(SurfaceHolder holder) {
//		// TODO Auto-generated method stub
////		Log.e("surfaceDestroyed", "2222");
//	}
//	public static native void nativeSetSurface(Surface surface,int width,int height);
	
	//huwh 以下都是启动动画的代码
	private Context context;
	private float mFromXDelta;
	private float mToXDelta;
	private float mFromYDelta;
	private float mToYDelta;
	private float mXDelta;
	private float mYDelta;
	private float imgWidth=0;//花瓣图片的宽
	private float imgHeight=0;//花瓣图片的高
	private float loadingtextSize[]={816,105};//百玩游戏图片的宽高
	private float loadingbanneSize[]={759,125};//百玩游戏精彩无限图片的宽高
	private float logounderSize[]={798,51};//阴影效果图片的宽高
	private static final int[] ITEM_DRAWABLES = { R.drawable.bw_loading_lan, 
		R.drawable.bw_loading_huang,
		R.drawable.bw_loading_hong };//上方三个花瓣图片
	private static final int[] ITEM_DRAWABLES_BOTTOM = { R.drawable.bw_loading_lv, 
		R.drawable.bw_loading_ju,
		R.drawable.bw_loading_zi };//下方三个花瓣图片
	private ImageView[] image_items;
	private ImageView image_lu,image_ju,image_zi,loadingtext,logounder,loadingbanner;
	private static final int[] itemStartX={272,1523,355};
	private static final int[] itemStartY={17,497,1247};
	private static final int[] itemCenterX={227,1133,825};
	private static final int[] itemCenterY={451,213,1119};
	private static final int[] itemEndX={679,757,835};
	private static final int[] itemEndy={634,501,636};
	private static final int[] lv_x_y={679,545};
	private static final int[] ju_x_y={835,545};
	private static final int[] zi_x_y={757,680};
	private float mToLvXDelta;//绿色花瓣移动后的x坐标
	private float mToLvYDelta;//绿色花瓣移动后的y坐标
	private float mToJuXDelta;//橘色花瓣移动后的x坐标
	private float mToJuYDelta;//橘色花瓣移动后的y坐标
	private float mToZiXDelta;//紫色花瓣移动后的x坐标
	private float mToZiYDelta;//紫色花瓣移动后的y坐标
	private static final int TIME_TRANSLATE=700;//上方三个花瓣的动画时间
	private static final int ITME_INTERVAL=50;//动画之间的间隔时间
	private static final int TIME_SCALE=600;//下方三个花瓣的动画时间
	private static final int TIME_CLIP=300;//百玩不厌精彩无限的动画时间
	
	/**
	 * 根据比例系数确定每个图片的大小
	 */
	private void initImageSize(){
		imgWidth=imgHeight=168/ScaleUtil.getCoefficient();
		loadingtextSize[0]=loadingtextSize[0]/ScaleUtil.getCoefficient();
		loadingtextSize[1]=loadingtextSize[1]/ScaleUtil.getCoefficient();
		loadingbanneSize[0]=loadingbanneSize[0]/ScaleUtil.getCoefficient();
		loadingbanneSize[1]=loadingbanneSize[1]/ScaleUtil.getCoefficient();
		logounderSize[0]=logounderSize[0]/ScaleUtil.getCoefficient();
		logounderSize[1]=logounderSize[1]/ScaleUtil.getCoefficient();
		
		
	}
	/** 
	 * 初始化上方三个花瓣 
	 * @param itemDrawables  上方三个花瓣的图片
	 */
		 private void init(int[] itemDrawables) {
		        final int itemCount = itemDrawables.length;
		        image_items=new ImageView[3];
		        for (int i = 0; i < itemCount; i++) {
		        	    image_items[i]= new ImageView(context);
		        	    image_items[i].setImageResource(itemDrawables[i]);
		            LayoutParams params =new LayoutParams((int)imgWidth, (int)imgHeight);
		            image_items[i].setLayoutParams(params);
		            mLayoutMobileMain.addView(image_items[i]);
		            mFromXDelta=ScaleUtil.getXScale(itemStartX[i]);
			 		mFromYDelta=ScaleUtil.getYScale(itemStartY[i]);
			 		mToXDelta=ScaleUtil.getXScale(itemEndX[i]);
			 		mToYDelta=ScaleUtil.getYScale(itemEndy[i]);
			 		mXDelta=ScaleUtil.getXScale(itemCenterX[i]);
			 		mYDelta=ScaleUtil.getYScale(itemCenterY[i]);
		            addAnimation_top(image_items[i],i);
		            
		        }
		        mToLvXDelta=ScaleUtil.getXScale(lv_x_y[0]);
		        mToLvYDelta=ScaleUtil.getYScale(lv_x_y[1]);
		        mToJuXDelta=ScaleUtil.getXScale(ju_x_y[0]);
		        mToJuYDelta=ScaleUtil.getYScale(ju_x_y[1]);
		        mToZiXDelta=ScaleUtil.getXScale(zi_x_y[0]);
		        mToZiYDelta=ScaleUtil.getYScale(zi_x_y[1]);
		 }
		 
    /**
     * 给上方三个花瓣添加动画
     * @param view 上方三个花瓣的view组件
     */
	public void addAnimation_top(View view,final int i) {
		AnimationSet animationset=new AnimationSet(true);
		animationset.setFillAfter(true);  
		
		Animation animation2=new ScaleAnimation(0,1,0,1,Animation.RELATIVE_TO_SELF,0.5f,Animation.RELATIVE_TO_SELF,0.5f);
		animation2.setStartOffset(0);
		animation2.setDuration(TIME_TRANSLATE);
		animation2.setFillAfter(true);
		animationset.addAnimation(animation2);
		
		Animation animation =new MyTranslateAnimation(mFromXDelta-imgWidth/2, mToXDelta-imgWidth/2, mFromYDelta-imgHeight/2,
    				mToYDelta-imgHeight/2, mXDelta-imgHeight/2, mYDelta-imgHeight/2);
		animation.setStartOffset(0);
		animation.setDuration(TIME_TRANSLATE);
		animation.setFillAfter(true);
		animationset.addAnimation(animation);
		
		animationset.setAnimationListener(new AnimationListener() {

            @Override
            public void onAnimationStart(Animation animation) {

            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {
				if (i == 2) {
					new Handler().postDelayed(new Runnable() {

						@Override
						public void run() {

							addAnimation_bottom();

						}
					}, 10);
				}
            }
        });

		
		view.startAnimation(animationset);
	}
	/**
	 * 给下方三个花瓣、百玩游戏、阴影效果 添加动画
	 */
	public void addAnimation_bottom(){
		
		image_lu.setVisibility(View.VISIBLE);
        image_ju.setVisibility(View.VISIBLE);
        image_zi.setVisibility(View.VISIBLE);
        loadingtext.setVisibility(View.VISIBLE);
        logounder.setVisibility(View.VISIBLE);
		
		Animation animation3=new ScaleAnimation(0, 1, 0, 1,Animation.RELATIVE_TO_SELF,0.5f,Animation.RELATIVE_TO_SELF,0.5f);
		animation3.setStartOffset(0);
		animation3.setDuration(TIME_SCALE);
		animation3.setFillAfter(true);
		logounder.startAnimation(animation3);
		loadingtext.startAnimation(animation3);
		
		for (int i = 0; i < 3; i++) {
			final int  j=i;
			Animation animation4 = null;
	        if(i==0){
	        	animation4=new TranslateAnimation(ScaleUtil.getCenter_x()-imgWidth/2, mToLvXDelta-imgWidth/2, ScaleUtil.getCenter_y()-imgHeight/2, mToLvYDelta-imgHeight/2);
	        }else if(i==1){
	        	animation4=new TranslateAnimation(ScaleUtil.getCenter_x()-imgWidth/2, mToJuXDelta-imgWidth/2, ScaleUtil.getCenter_y()-imgHeight/2, mToJuYDelta-imgHeight/2);
	        }else if(i==2){
	        	animation4=new TranslateAnimation(ScaleUtil.getCenter_x()-imgWidth/2, mToZiXDelta-imgWidth/2, ScaleUtil.getCenter_y()-imgHeight/2, mToZiYDelta-imgHeight/2);
	        }
			   animation4.setStartOffset(0);
			   animation4.setDuration(TIME_SCALE);
			   animation4.setFillAfter(true);
			   animation4.setAnimationListener(new AnimationListener() {

		            @Override
		            public void onAnimationStart(Animation animation) {

		            }

		            @Override
		            public void onAnimationRepeat(Animation animation) {

		            }

		            @Override
		            public void onAnimationEnd(Animation animation) {
					if (j == 2) {
						new Handler().postDelayed(new Runnable() {

							@Override
							public void run() {

								addAnimation_banner();

							}
						}, ITME_INTERVAL);
					}
		           
		            }
		        }); 
			   if(i==0){  
				   image_lu.startAnimation(animation4);  
			   }else if(i==1){
				   image_ju.startAnimation(animation4);  
			   }else if(i==2){
				   image_zi.startAnimation(animation4);  
			   }
			   
			   
			   
		}
		
		
		 
		
	}
	/**
	 * 给百玩不厌精彩无线 添加动画
	 */
	public void addAnimation_banner(){
		loadingbanner.setVisibility(View.VISIBLE);
		final ClipDrawable drawable = (ClipDrawable)loadingbanner.getDrawable();  
        final Handler handler = new Handler()  
        {  
            public void handleMessage(Message msg)  
            {  
                if(msg.what == 1)  
                {  
                    //修改ClipDrawable的level值  
                    drawable.setLevel(drawable.getLevel() +1000);  
                }  
            }  
        };  
        final Timer timer = new Timer();     
        timer.schedule(new TimerTask()  
        {  
            public void run()  
            {  
                Message msg = new Message();  
                msg.what = 1;  
                //发送消息,通知应用修改ClipDrawable对象的level值  
                handler.sendMessage(msg);  
                //取消定时器  
                if(drawable.getLevel() >= 10000)  
                {
					timer.cancel();
					// 发送启动动画结束的通知
					Message msg2 = new Message();
					msg2.what = MSG_LOADINGEND;
					mProcessUseMsg.sendMessageDelayed(msg2, 1000);
				} 
            }  
        },0,5);  
		
	}
	/**
	 * 动态向跟布局中添加组件
	 * @param itemDrawables 下方三个花瓣的图片
	 */
	public void addView(int[] itemDrawables,int orientation){
		root=(RelativeLayout) LayoutInflater.from(MobileMain.this).inflate(R.layout.activity_welcome, null);
		// 动态更换背景图片
		if (orientation == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) {
			// 横屏
			root.setBackgroundResource(R.drawable.bw_loading_background_horizontal);
		} else if (orientation == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
			// 竖屏
			root.setBackgroundResource(R.drawable.bw_loading_background_portrait);
		}
		loadingtext = new ImageView(context);
        loadingtext.setImageResource(R.drawable.bw_loadingtext);
		LayoutParams params1 = new LayoutParams((int)(loadingtextSize[0]), (int)(loadingtextSize[1]));
        params1.setMargins((int)(ScaleUtil.getCenter_x()-loadingtextSize[0]/2), (int)(ScaleUtil.getCenter_y()+20), (int)(ScaleUtil.getCenter_x()+loadingtextSize[0]), (int)(ScaleUtil.getCenter_y()+20+loadingtextSize[1]));// 通过自定义坐标来放置你的控件
        loadingtext.setLayoutParams(params1);
        root.addView(loadingtext);
        
		final int itemCount = itemDrawables.length;
        for (int i = 0; i < itemCount; i++) {
        	if(i == 0){
        		image_lu = new ImageView(context);
        		image_lu.setImageResource(itemDrawables[i]);
            LayoutParams params2 =new LayoutParams((int)imgWidth, (int)imgHeight);  
            image_lu.setLayoutParams(params2);
            root.addView(image_lu);
        	}else if(i == 1){
        		image_ju = new ImageView(context);
        		image_ju.setImageResource(itemDrawables[i]);
            LayoutParams params2 =new LayoutParams((int)imgWidth, (int)imgHeight);
            image_ju.setLayoutParams(params2);
            root.addView(image_ju);
        	}else if(i == 2){
        		image_zi = new ImageView(context);
        		image_zi.setImageResource(itemDrawables[i]);
        		LayoutParams params2 =new LayoutParams((int)imgWidth, (int)imgHeight);
            image_zi.setLayoutParams(params2);
            root.addView(image_zi);
        	}
        	
        }
        
        logounder = new ImageView(context);
		logounder.setImageResource(R.drawable.bw_loadinglogounder);
		LayoutParams params2 = new LayoutParams((int)(logounderSize[0]), (int)(logounderSize[1]));
        params2.setMargins((int)(ScaleUtil.getCenter_x()-logounderSize[0]/2), (int)(ScaleUtil.getCenter_y()+imgHeight), (int)(ScaleUtil.getCenter_x()+logounderSize[0]), (int)(ScaleUtil.getCenter_y()+imgHeight+logounderSize[1]));// 通过自定义坐标来放置你的控件
        logounder.setLayoutParams(params2);
        root.addView(logounder);
        
        loadingbanner=new ImageView(context);   
        loadingbanner.setImageResource(R.drawable.my_clip);
		LayoutParams params3 = new LayoutParams((int)(loadingbanneSize[0]), (int)(loadingbanneSize[1]));
        params3.setMargins((int)(ScaleUtil.getCenter_x()-loadingbanneSize[0]/2), ScaleUtil.getScreenHeight()*3/4, (int)(ScaleUtil.getCenter_x()+loadingbanneSize[0]), (int) (ScaleUtil.getScreenHeight()*3/4+loadingbanneSize[1]));// 通过自定义坐标来放置你的控件
        loadingbanner.setLayoutParams(params3);
        root.addView(loadingbanner);
        
        image_lu.setVisibility(View.GONE);
        image_ju.setVisibility(View.GONE);
        image_zi.setVisibility(View.GONE);
        loadingtext.setVisibility(View.GONE);
        logounder.setVisibility(View.GONE);
        loadingbanner.setVisibility(View.GONE);
        
        mLayoutMobileMain.addView(root);
        
	}
	//huwh 以下都是游戏中浮动工具栏的代码
	private WindowManager windowManager = null;
	private WindowManager.LayoutParams windowManagerParams = null;
    private ArcLeftTopLayout mArcLayout;
    private FrameLayout mFrameLayout=null;
    private ImageView mImageView;
    
    public static final int PLACE_LEFT_TOP = 1;//位置常量--左上
	public static final int PLACE_RIGHT_TOP = 2;//位置常量--右上
	public static final int PLACE_LEFT_DOWN = 3;//位置常量--左下
	public static final int PLACE_RIGHT_DOWN = 4;//位置常量--右下
	public static final int PLACE_CENTER = 5;//标志小球正在滑动 
	private int position=PLACE_LEFT_TOP;//标志小球滑动完之后应该处于四个角的那个位置
    private float mTouchX;
	private float mTouchY;
	private float x;
	private float y;
	private float mStartX;
	private float mStartY;
	private static final int TOOL_STATE_UNPRESS = 1;//小球状态--没按下
	private static final int TOOL_STATE_EXPAND_PRESS = 2;//小球转开状态--没按下
	private static final int TOOL_STATE_EXPAND_UNPRESS = 3;//小球转开状态--按下
	private static final int TOOL_STATE_FOUCS = 4;//小球状态--拖动
	private static final int[] ITEM_DRAWABLES_FLOAT = { R.drawable.bwfloattool_refreshbutton, 
											R.drawable.bwfloattool_paybutton,
											R.drawable.bwfloattool_exitbutton };
	public static MyHandler myHandler;
	public boolean isShowTool=true;//标志点击充值是否 显示隐藏小球
	public boolean isStop=false;//标志点击home键是否 显示隐藏小球
	
	private void createView() {
		mFrameLayout=(FrameLayout) LayoutInflater.from(context).inflate(R.layout.arc_lefttop_menu, null);
		mArcLayout = (ArcLeftTopLayout) LayoutInflater.from(context).inflate(
				R.layout.arc_lefttop_layout, null);
		mImageView = (ImageView) mFrameLayout.findViewById(R.id.control_hint);
		
		FrameLayout.LayoutParams params = (android.widget.FrameLayout.LayoutParams) mImageView.getLayoutParams();  
		if(mScreenWidth>mScreenHeight)
		{
			 params.height=(int) (mScreenHeight*0.12);  
			 params.width =(int) (mScreenHeight*0.12);
			 mArcLayout.setChildSize((int) (mScreenHeight*0.1));
		}else{
			 params.height=(int) (mScreenWidth*0.12);  
			 params.width =(int) (mScreenWidth*0.12);
			 mArcLayout.setChildSize((int) (mScreenWidth*0.1));
		}
	    mImageView.setLayoutParams(params); 
	    
		mArcLayout.setArc(0.0f, 90.0f);
		mArcLayout.setVisibility(View.INVISIBLE);
		mArcLayout.setContext(MobileMain.this);
		initArcMenu(ITEM_DRAWABLES_FLOAT);
        
		// 获取WindowManager
		windowManager = (WindowManager) getApplicationContext()
				.getSystemService(Context.WINDOW_SERVICE);
		// 设置LayoutParams(全局变量）相关参数
		windowManagerParams = new WindowManager.LayoutParams();

		windowManagerParams.type = WindowManager.LayoutParams.TYPE_PHONE; // 设置window type
		windowManagerParams.format = PixelFormat.RGBA_8888; // 设置图片格式，效果为背景透明
		// 设置Window flag
		windowManagerParams.flags = WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL
				| WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
		/*
		 * 注意，flag的值可以为： LayoutParams.FLAG_NOT_TOUCH_MODAL 不影响后面的事件
		 * LayoutParams.FLAG_NOT_FOCUSABLE 不可聚焦 LayoutParams.FLAG_NOT_TOUCHABLE
		 * 不可触摸
		 */
		// 调整悬浮窗口至左上角，便于调整坐标
		windowManagerParams.gravity = Gravity.LEFT | Gravity.TOP;
		// 以屏幕左上角为原点，设置x、y初始值
		windowManagerParams.x = 0;
		windowManagerParams.y = 0;
		// 设置悬浮窗口长宽数据
		windowManagerParams.width = LayoutParams.WRAP_CONTENT;
		windowManagerParams.height = LayoutParams.WRAP_CONTENT;
 		
		mFrameLayout.setOnTouchListener(this);
		windowManager.addView(mFrameLayout, windowManagerParams);
		
		mLayoutMobileMain.addView(mArcLayout);
		mArcLayout.setOnTouchListener(this);
		
	}
	
	
	
	 private void initArcMenu(int[] itemDrawables) {
	        final int itemCount = itemDrawables.length;
	        for (int i = 0; i < 3; i++) {
	            final ImageView item = new ImageView(context);
	            item.setImageResource(itemDrawables[i]);
	            final int itemposition = i;
	            mArcLayout.addView(item);
	            
	            if (!mbOnsale&&i==1)
	            {
	            	 item.setEnabled(false);
	            	 item.setImageResource(R.drawable.bwfloattool_paybutton_press);//灰色充值图片
	            }
	            
	            item.setOnClickListener(new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						// TODO Auto-generated method stub 
						mArcLayout.switchState(true,position);
					if (itemposition == 0) {
						// 刷新
						Dialog dlg = new AlertDialog.Builder(MobileMain.this)
								.setIcon(R.drawable.logo_36)
								.setTitle("是否刷新？")
								.setMessage("刷新将重新加载游戏，建议您在游戏无法正常加载时使用。")
								.setPositiveButton("确定",
										new DialogInterface.OnClickListener() {
											public void onClick(
													DialogInterface dialog,
													int whichButton) {
												mMobileView.onDomStop();
												stopProgressLoading();
												mMobileView.clearCache();
											}
										})
								.setNegativeButton("取消",
										new DialogInterface.OnClickListener() {
											public void onClick(
													DialogInterface dialog,
													int which) {
												mMobileView.setSilence(false);
											}
										}).create();
						dlg.show();

					} else if (itemposition == 1) {

						HideMenu();
						isShowTool = false;
						isAbout=true;
						// 充值
						WebGameRecharge();
						
						
					} else if (itemposition == 2) {

						// 退出
						if (exitTimes >= 3) {
							exitTimes = 0;
							AppQuit();
						} else {
							menuExitFlash();
						}

					}
	
					}
				});
	        }
	    }
	    
	    
	  /**  
	   * 改变menu的背景图片
	   * @param toolState menu的点击状态
	   */
	private void ChangeToolButtonImageView(int toolState) {
		if (toolState==TOOL_STATE_UNPRESS) {
			mImageView.setImageResource(R.drawable.bwfloattool_button_unpress);
		} else if (toolState==TOOL_STATE_EXPAND_PRESS) {
			mImageView.setImageResource(R.drawable.bwfloattool_expendbutton_press);
		} else if (toolState==TOOL_STATE_EXPAND_UNPRESS) {
			mImageView.setImageResource(R.drawable.bwfloattool_expendbutton_unpress);
		} else {
			mImageView.setImageResource(R.drawable.bwfloattool_button_focus);
		}

	}
	/**
	 * 更新windowManager的位置
	 * @param position 标志menu所在的位置 包括左上、右上、左下、右下四个位置
	 * @param type 标志是move还是up move时windowManager不更新mArcLayout up时windowManager更新mArcLayout
	 */
	private void updateViewPosition(int position) {
		switch (position) {
		case 5:
			// 小球在运动时更新浮动窗口位置参数
			windowManagerParams.x = (int) (x - mTouchX);
			windowManagerParams.y = (int) (y - mTouchY);
			break;
		case 1:
			// 更新浮动窗口到左上角
			windowManagerParams.x = 0;
			windowManagerParams.y = 0;
			break;
		case 2:
			// 更新浮动窗口到右上角
			windowManagerParams.x = mScreenWidth;
			windowManagerParams.y = 0;
			break;
		case 3:
			// 更新浮动窗口到左下角
			windowManagerParams.x = 0;
			windowManagerParams.y = mScreenHeight;
			break;
		case 4:
			// 更新浮动窗口到右下角
			windowManagerParams.x = mScreenWidth;
			windowManagerParams.y = mScreenHeight;
			break;
		default:
			break;
		}
		windowManager.updateViewLayout(mFrameLayout, windowManagerParams); // 刷新显示
		
	}
	
	/**
	 * 更新menu的位置 包括左上、右上、左下、右下四个位置
	 */
	private void updateLeftRightFlag() {
		switch(position){
		case PLACE_LEFT_TOP:
//			Log.i("tag","坐标x="+x+"宽的1/6="+mScreenWidth / 6+"宽的1/2="+mScreenWidth / 2+"坐标y="+y+"高的1/6="+mScreenHeight / 6+"高的1/2="+mScreenHeight / 2);
			if(x<=mScreenWidth / 6&&y<=mScreenHeight / 6){
				position = PLACE_LEFT_TOP;
			}else if(x>=mScreenWidth / 6&&x<=mScreenWidth / 2&&y<=mScreenHeight / 6){
				position = PLACE_RIGHT_TOP;
			}else if(x >= mScreenWidth / 2 && y <=mScreenHeight / 2){
				position = PLACE_RIGHT_TOP;
			}else if(x>=mScreenWidth / 6&&x<=mScreenWidth / 2&&y>=mScreenHeight / 6&&y<=mScreenHeight / 2){
				double distance_left=Math.sqrt(Math.pow(x, 2)+Math.pow((mScreenHeight-y), 2));
				double distance_right=Math.sqrt(Math.pow((mScreenWidth-x), 2)+Math.pow(y, 2));
				if(distance_left<=distance_right){
					position = PLACE_LEFT_DOWN;
				}else{
					position = PLACE_RIGHT_TOP;
				}
			}else if(x<=mScreenWidth / 6&&y>=mScreenHeight / 6&&y<=mScreenHeight / 2)
			{
				position = PLACE_LEFT_DOWN;
			}else if(x <= mScreenWidth / 2 && y >= mScreenHeight / 2){
				position = PLACE_LEFT_DOWN;
			}else if(x >= mScreenWidth / 2 && y >= mScreenHeight / 2) {
				position = PLACE_RIGHT_DOWN;
			}
			break;
		case PLACE_RIGHT_TOP:
			if(x>=mScreenWidth *5/ 6&&y<=mScreenHeight / 6){
				position = PLACE_RIGHT_TOP;
			}else if(x>=mScreenWidth / 2&&x<=mScreenWidth*5/ 6&&y<=mScreenHeight / 6){
				position = PLACE_LEFT_TOP;
			}else if(x <= mScreenWidth / 2 && y <= mScreenHeight / 2){
				position = PLACE_LEFT_TOP;
			}else if(x>=mScreenWidth / 2&&x<=mScreenWidth *5/ 6&&y>=mScreenHeight / 6&&y<=mScreenHeight / 2){
				double distance_left=Math.sqrt(Math.pow(x, 2)+Math.pow(y, 2));
				double distance_right=Math.sqrt(Math.pow((mScreenWidth-x), 2)+Math.pow(mScreenHeight-y, 2));
				if(distance_left<=distance_right){
					position = PLACE_LEFT_TOP;
				}else{
					position = PLACE_RIGHT_DOWN;
				}
			}else if(x>=mScreenWidth *5/ 6 &&y>=mScreenHeight / 6&&y<=mScreenHeight / 2)
			{
				position = PLACE_RIGHT_DOWN;
			}else if(x >= mScreenWidth / 2 && y >= mScreenHeight / 2){
				position = PLACE_RIGHT_DOWN;
			}else if(x <= mScreenWidth / 2 && y >= mScreenHeight / 2) {
				position = PLACE_LEFT_DOWN;
			}
			break;
		case PLACE_LEFT_DOWN:
			if(x<=mScreenWidth / 6&&y>=mScreenHeight *5/ 6){
				position = PLACE_LEFT_DOWN;
			}else if(x<=mScreenWidth / 6&&y>=mScreenHeight / 2&&y<=mScreenHeight*5 / 6){
				position = PLACE_LEFT_TOP;
			}else if(x <= mScreenWidth / 2 && y <= mScreenHeight / 2){
				position = PLACE_LEFT_TOP;
			}else if(x>=mScreenWidth / 6&&x<=mScreenWidth / 2&&y>=mScreenHeight / 2&&y<=mScreenHeight*5 / 6){
				double distance_left=Math.sqrt(Math.pow(x, 2)+Math.pow(y, 2));
				double distance_right=Math.sqrt(Math.pow((mScreenWidth-x), 2)+Math.pow(mScreenHeight-y, 2));
				if(distance_left<=distance_right){
					position = PLACE_LEFT_TOP;
				}else{
					position = PLACE_RIGHT_DOWN;
				}
			}else if(x>=mScreenWidth / 6&&x<=mScreenWidth / 2&&y>=mScreenHeight*5 / 6)
			{
				position = PLACE_RIGHT_DOWN;
			}else if(x >= mScreenWidth / 2 && y >= mScreenHeight / 2){
				position = PLACE_RIGHT_DOWN;
			}else if(x >= mScreenWidth / 2 && y <= mScreenHeight / 2) {
				position = PLACE_RIGHT_TOP;
			}
			break;
		case PLACE_RIGHT_DOWN:
			if(x>=mScreenWidth *5/ 6&&y>=mScreenHeight *5/ 6){
				position = PLACE_RIGHT_DOWN;
			}else if(x>=mScreenWidth *5/ 6&&y>=mScreenHeight / 2&&y<=mScreenHeight*5 / 6){
				position = PLACE_RIGHT_TOP;
			}else if(x >= mScreenWidth / 2 && y <= mScreenHeight / 2){
				position = PLACE_RIGHT_TOP;
			}else if(x>=mScreenWidth / 2&&x<=mScreenWidth *5/ 6&&y>=mScreenHeight / 2&&y<=mScreenHeight*5 / 6){
				double distance_left=Math.sqrt(Math.pow(x, 2)+Math.pow(mScreenHeight-y, 2));
				double distance_right=Math.sqrt(Math.pow((mScreenWidth-x), 2)+Math.pow(y, 2));
				if(distance_left<=distance_right){
					position = PLACE_LEFT_DOWN;
				}else{
					position = PLACE_RIGHT_TOP;
				}
			}else if(x>=mScreenWidth / 2&&x<=mScreenWidth *5/ 6&&y>=mScreenHeight*5 / 6)
			{
				position = PLACE_LEFT_DOWN;
			}else if(x <= mScreenWidth / 2 && y >= mScreenHeight / 2){
				position = PLACE_LEFT_DOWN;
			}else if(x <= mScreenWidth / 2 && y <= mScreenHeight / 2) {
				position = PLACE_LEFT_TOP;
			}
			break;
		}

	}

	@Override
	public boolean onTouch(final View view, MotionEvent event) {
		// TODO Auto-generated method stub
		// 获取相对屏幕的坐标，即以屏幕左上角为原点
		x = event.getRawX();
		y = event.getRawY();
		Log.i("tag", "currX" + x + "====currY" + y);
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN: // 捕获手指触摸按下动作
			// 获取相对View的坐标，即以此View左上角为原点
			mTouchX = event.getX();
			mTouchY = event.getY();
			mStartX = x;
			mStartY = y;
			Log.i("tag", "startX" + mStartX + "====startY" + mStartY);
			if(view.getId()==R.id.control_layout){
				if (mArcLayout.getVisibility()==View.VISIBLE) {
					ChangeToolButtonImageView(TOOL_STATE_EXPAND_PRESS);
				}else{
					ChangeToolButtonImageView(TOOL_STATE_FOUCS);
				}
			}else if(view.getId()==R.id.item_layout){
				//点击小球其他地方  让小球消失
				if (mArcLayout.getVisibility()==View.VISIBLE) {
					mArcLayout.switchState(true, position);
				}
			}
			break;

		case MotionEvent.ACTION_MOVE: // 捕获手指触摸移动动作
			if (mArcLayout.getVisibility()==View.INVISIBLE) {
				// 当mArcLayout不可见时才可拖动
				updateViewPosition(5);
			}
			break;

		case MotionEvent.ACTION_UP: // 捕获手指触摸离开动作
			Log.i("tag", "ACTION_UP");
			if (mArcLayout.getVisibility()==View.INVISIBLE) {
				ChangeToolButtonImageView(TOOL_STATE_UNPRESS);
				//当mArcLayout不可见时才处理手指的抬起事件
				updateLeftRightFlag();
				mArcLayout.setPosition(position);
				updateViewPosition(position);
				mArcLayout.requestLayout();
				mTouchX = mTouchY = 0;
				if (Math.abs((x - mStartX)) < 8 && Math.abs((y - mStartY)) < 8) {
					if (view.getId() == R.id.control_layout) {
						ChangeToolButtonImageView(TOOL_STATE_EXPAND_UNPRESS);
					}
					mArcLayout.setVisibility(View.VISIBLE);
					switch (position) {
					case PLACE_LEFT_TOP:
						mArcLayout.setArc(0.0f, 90.0f);
						break;
					case PLACE_RIGHT_TOP:
						mArcLayout.setArc(90.0f, 180.0f);
						break;
					case PLACE_LEFT_DOWN:
						mArcLayout.setArc(270.0f, 360.0f);
						break;
					case PLACE_RIGHT_DOWN:
						mArcLayout.setArc(180.0f, 270.0f);
						break;
					default:
						break;
					}
					new Handler().postDelayed(new Runnable() {

						public void run() {
							mArcLayout.switchState(true, position);
						}
					}, 0);
				}

			} 
			else {
				Log.i("tag", "currX" + x + "====currY" + y);
				//当mArcLayout可见的情况下  点击  让mArcLayout收缩
				mTouchX = mTouchY = 0;
				mArcLayout.switchState(true, position);
			}
			break;
		}
		return false;

	}
	
	class MyHandler extends Handler{
		@Override
		public void handleMessage(Message msg) {
			// 
			super.handleMessage(msg);
			if(msg.what==1){
				//mArcLayout收缩动画完成后更换menu的图片背景
				ChangeToolButtonImageView(TOOL_STATE_UNPRESS);
			}
		}
	}
	
} // end class of MobileMain
