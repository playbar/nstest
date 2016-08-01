package com.jqbar.pay;

import android.app.Activity;
import android.content.Intent;
import android.widget.ImageView;

import com.jqbar.ani.*;
import com.jqbar.pay.BwPay;

public class BwSDK
{
	static BwSDK m_BwSDK = null;
	public static BwAnimation m_BwAni = null;
	public static BwPay m_BwPay = null;
	public int delayTime = 500;
	public boolean isCreatedPay = false;
	public boolean isInitPay = false;
	
	public BwPay createSDKPay(Activity context, int enable)
	{
		if (m_BwPay == null)
		{
			if (enable == 1)
			{
				// TODO: 付费修改
//				m_BwPay = new BwPay360(context);
//				m_BwPay = new BwPayUC(context);
				m_BwPay=new BwPay(context);
//				m_BwPay=new BwPayYYH(context);
//				m_BwPay=new BwPayYDMM(context);
//				m_BwPay=new BwPayZFB(context);
			}
			else
			{
			//	m_BwPay = new BwPay(context);
			}
			isInitPay = false;
			isCreatedPay = true;
		}
		return m_BwPay;
	}
	
	public BwAnimation createSDKAni()
	{
		if (m_BwAni == null)
		{
			// TODO: 双启动动画修改
			m_BwAni = new BwAnimation();
//			m_BwAni =new  BwAnimation9You();
//			m_BwAni = new BwAnimationUC();
		}
		return m_BwAni;
	}
	
	// 以下不需要修改
	public static BwSDK DefaultSDK()
	{
		if (m_BwSDK == null)
		{
			m_BwSDK = new BwSDK();
		}
		return m_BwSDK;
	}
	

	
	
	// 动画资源，关于画面
	public int setStartAnimation(final ImageView view) {
		if (m_BwAni != null)
		{
			delayTime = m_BwAni.startAnimation(view);
		}
		return delayTime;
	}

	public void startFadeAni(final ImageView view) {
		if (m_BwAni == null) return;
		m_BwAni.ani_fade(view);
	}

	public static void setAboutImage(final ImageView view) {
		if (m_BwAni == null) return;
		m_BwAni.setAboutImage(view);
	}
	
	// 付费相关
	public void initSDKPay() {
		if (m_BwPay == null) return;
		if (!isInitPay)
		{
			m_BwPay.initSDK();
			isInitPay = true;
		}
	}

	public int pay(String strUrl) {
		if (m_BwPay == null) return 0;
		return m_BwPay.pay(strUrl);
	}
	public void enterUserCenter()
	{
		if(m_BwPay != null)
		{
		 m_BwPay.enterUserCenter();
		}
	}
	public boolean result(int requestCode, int resultCode, Intent data) {
		if (m_BwPay == null) return false;
		return m_BwPay.result(requestCode,resultCode,data);
	}

	public void destory() {
		if (m_BwPay == null) return;
		m_BwPay.destory();
	}
	public boolean onkeyBack(){
		if (m_BwPay != null){
			return m_BwPay.onkeyBack();
		}
		return false;
	}
}
