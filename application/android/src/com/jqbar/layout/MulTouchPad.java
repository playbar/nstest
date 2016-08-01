package com.jqbar.layout;

import com.jqbar.*;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.view.View;
import com.jqbar.MobileMain;
public class MulTouchPad extends ViewLayout
{
    public void setOrientationPortrait( )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT); // 设置为竖排
    }
    
    public void setOrientationLandscape( )
    {
    	mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); 
    }
    
    public void setOrientation( )
    {
    		mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE); 
    		mMobileMain.mOrientation = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
    }
    
    public MulTouchPad(Context context)
    {
    	super( context );
    }
    
    public void setContentView()
    {
        mMobileMain.setContentView(R.layout.multouchpad );
    }
    
    public void setMode( int mode )
    {
    	//mMobileMain.switchViewHW( 1 );
       // mMobileMain.screenModeVal( 1 );
    	  if(mMobileMain.getRequestedOrientation()==-1)
          {
              return;
          }
          if( (mMobileMain.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_PORTRAIT) && mode == 1 )
          {
              mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);  // 转为横屏
              mMobileMain.switchViewHW( mode );
              System.out.println("setMode MulTouchPad LANDSCAPE");
          }
          else if( (mMobileMain.getRequestedOrientation() == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) && mode == 0 )
          {
              mMobileMain.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);   // 转为竖屏
              mMobileMain.switchViewHW( mode );
              System.out.println("setMode MulTouchPad PORTRAIT");

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