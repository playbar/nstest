package com.jqbar.offerwall;

import com.jqbar.pay.BwSDK;

import android.app.Activity;

public class BwOfferwall {
	static BwOfferwall m_BwOfferwall = null;
	public static BwWall m_BwWall = null;
	public boolean isInitWall = false;
	public boolean isCreatedWall = false;

	public BwWall createOfferWall(Activity context) {
		if (m_BwWall == null) {
			m_BwWall = new BwWall(context);
//			m_BwWall=new Domob_wall(context);
		}
		isInitWall = false;
		isCreatedWall = true;
		return m_BwWall;
	}

	public void initOfferWall() {
		if (m_BwWall == null)
			return;
		if (!isInitWall) {
			m_BwWall.initWall();
		}
	}

	public static BwOfferwall DefaultOFFERWALL()
	{
		if (m_BwOfferwall == null)
		{
			m_BwOfferwall = new BwOfferwall();
		}
		return m_BwOfferwall;
	}
	
	
	public void showOfferWall(String userId) {
		if (m_BwWall == null)
			return;
		m_BwWall.showOfferWall(userId);
	}
}
