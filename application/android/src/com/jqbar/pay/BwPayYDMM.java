package com.jqbar.pay;

import android.app.Activity;
import android.content.Context;
import android.widget.Toast;

public class BwPayYDMM extends BwPay{
	private Context context;
	final String checkStr = "|UCSDKPAY";
	int cpid_uc=760 ;
	int gameid_uc=527542 ;
	int serverid_uc =2338;
	public BwPayYDMM(Activity c) {
		super(c);
		context=c;
	}

	public void initSDK() {
		
	}

	
	
	private void login() {
		
	}
	
	
	public int pay(String strUrl) {
 	if (!strUrl.endsWith(checkStr))
	{
		Toast.makeText(mActivity, "错误参数", Toast.LENGTH_LONG).show();
		return -1;
	}
	if (context == null) {
		return -1;
	}	
	String wares[] = strUrl.split("\\|");

	return 0;
	}

	
	public void destory() {
	};

}
