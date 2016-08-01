package com.jqbar.offerwall;

import com.jqbar.MobileMain;

import android.app.Activity;
import android.content.Context;

public class BwWall {
	Activity mActivity = null;
	MobileMain mMobileMain = null;
	Context appContext = null;

	public BwWall(Activity context) {
		mActivity = context;
		mMobileMain = (MobileMain) context;
		appContext = context.getApplicationContext();
	}

	public void initWall() {
		return;
	}

	public void showOfferWall(String userId) {
		
	}
}
