package com.jqbar.pay;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;

import com.jqbar.MobileMain;

public class BwPay {
	Activity mActivity = null;
	MobileMain mMobileMain = null;
	Context appContext = null;

	int byAniTime = 0;
	Timer byAniTimer = null;
	TimerTask byAniTask = null;

	public BwPay(Activity context) {
		mActivity = context;
		mMobileMain = (MobileMain) context;
		appContext = context.getApplicationContext();
	}

	public void initSDK() {
		return;
	}

	public int pay(String strUrl) {
		return 0;
	}
	//进入SDK用户管理中心
	public void enterUserCenter()
	{
		return;
	}
	public boolean result(int requestCode, int resultCode, Intent data) {
		return false;
	}

	public void destory() {

	}
	public boolean onkeyBack(){
		return false;
	}
}
