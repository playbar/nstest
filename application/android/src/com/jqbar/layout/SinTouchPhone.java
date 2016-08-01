package com.jqbar.layout;

import com.jqbar.*;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.view.View;
import com.jqbar.MobileMain;
public class SinTouchPhone extends ViewLayout
{    
    public void setOrientationPortrait( )
    {
    		mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT); // 设置为竖排
    }
    
    public void setOrientationLandscape( )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); //设置为横屏
    }
    
    public void setOrientation(  )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT); 
    }
    
    public SinTouchPhone(Context context)
    {
    	super( context );
    }
    
    public void setContentView()
    {
        mMobileMain.setContentView(R.layout.sintouchphone );
    }
    
    public void setMode( int mode )
    {
        if(mMobileMain.getRequestedOrientation()==-1)
        {
            return;
        }
        if( (mMobileMain.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT) && mode == 1 )
        {
            mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);  // 转为横屏
            mMobileMain.switchViewHW( mode );
            mMobileMain.HideMenu();
        }
        else if( (mMobileMain.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) && mode == 0 )
        {
            mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);   // 转为竖屏
            mMobileMain.switchViewHW( mode );
        }
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