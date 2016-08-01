package com.jqbar.layout;

import com.jqbar.*;

import android.app.Activity;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.view.View;
import android.widget.EditText;
import com.jqbar.MobileMain;
public abstract class ViewLayout
{
    protected MobileMain mMobileMain = null;
    
    public ViewLayout(Context context)
    {
        mMobileMain = (MobileMain)context;
    }
    
    public void setOrientationPortrait( ){ }
    
    public void setOrientationLandscape( ) {}
    
    public void setOrientation( ){ }
    
    public void setContentView() { }
    
    public void setMode( int mode ){ }
    
    public void CreateMenuEvent(){}
    
    public void MenuDirectionVisible(){}
    
    public void MenuDirectionGone() { }


    
}