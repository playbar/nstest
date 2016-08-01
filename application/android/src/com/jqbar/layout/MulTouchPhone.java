package com.jqbar.layout;

import com.jqbar.*;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Button;
import com.jqbar.MobileMain;
public class MulTouchPhone extends ViewLayout
{
    public void setOrientationPortrait( )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT); 
    }
    
    public void setOrientationLandscape( )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); 
    }
    public void setOrientation( )
    {
    		mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); // 设置为竖排
    		mMobileMain.mOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
    }
    
    public MulTouchPhone(Context context)
    {
        super( context );
        //mMobileMain = (MobileMain)context;
    }
    
    public void setContentView()
    {
        mMobileMain.setContentView(R.layout.multouchphone );
    }
    
    public void setMode(int mode )
    {
        if(mMobileMain.getRequestedOrientation()==-1)
        {
            return;
        }
        if( (mMobileMain.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT) && mode == 1 )
        {
            mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);  // 转为横屏
            mMobileMain.switchViewHW( mode );
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
//        mMobileMain.loadMenuMediaOld();
    }
    
    public void MenuDirectionVisible()
    {
        //mMobileMain.MenuDirectionVisible();
    }
    
    public void MenuDirectionGone()
    {
        //mMobileMain.MenuDirectionGone();
    }
   
}