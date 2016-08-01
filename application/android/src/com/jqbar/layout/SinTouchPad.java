package com.jqbar.layout;

import com.jqbar.*;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.view.View;
import com.jqbar.MobileMain;
public class SinTouchPad extends ViewLayout
{
    public void setOrientationPortrait(  )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT); // 设置为竖排
    }
    
    public void setOrientationLandscape(  )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); 
    }
    
    public void setOrientation( )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); 
    }
    
    public SinTouchPad(Context context)
    {
    	super( context );
    }
    
    public void setContentView()
    {
        mMobileMain.setContentView(R.layout.sintouchpad );
    }
    
    public void setMode( int mode )
    {
        return;
    }
    
    public void CreateMenuEvent()
    {
    	 mMobileMain.loadMenuBrowserPhone();
     mMobileMain.loadMenuDirection();
    }
    
    public void MenuDirectionVisible()
    {
        mMobileMain.MenuDirectionVisible();
    }
    
    public void MenuDirectionGone()
    {
        mMobileMain.MenuDirectionGone();
    }
    
    
}