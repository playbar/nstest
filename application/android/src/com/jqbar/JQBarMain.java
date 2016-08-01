package com.jqbar;

import com.jqbar.R;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import com.jqbar.MobileMain;
public class JQBarMain extends Activity
{

    private String LOCAL_PATH = "/sdcard/byone/cs/Local";

    void createLocalView()
    {
    	Intent intent = new Intent(this, LocalFlashAct.class);
    	Bundle bl = new Bundle();
    	bl.putString("LOCAL_PATH", LOCAL_PATH );
    	intent.putExtras(bl );
    	startActivityForResult( intent, 0 );
    	//finish();
    }
    void createMobileView()
    {
    	Intent intent = new Intent(this, MobileMain.class);
    	startActivity(intent);
    	finish();
    }

    private ImageView mImgVLoading;
    private Animation mAniLoadingEnter;
    private Animation mAniLoadingExit;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.jqbarmain);

//        mAniLoadingExit = AnimationUtils.loadAnimation(this,R.anim.loading_exit); //动画效果
//        mAniLoadingEnter = AnimationUtils.loadAnimation(this,R.anim.loading_enter); //动画效果
//        mImgVLoading = (ImageView) findViewById(R.id.app_loading);
//        mImgVLoading.startAnimation(mAniLoadingEnter);
//        Message msg = new Message();
//        splashHandler.sendMessageDelayed(msg, SPLASHTIME);

        mImgVLoading = (ImageView)findViewById(R.id.app_loading);
        mImgVLoading.setOnClickListener(new OnClickListener()
        {
            public void onClick(View v)
            {
                createMobileView();
            }
        });
    }

    private static final long SPLASHTIME = 1000;

    private Handler splashHandler = new Handler()
    {
        public void handleMessage(Message msg)
        {
            mImgVLoading.startAnimation(mAniLoadingExit);
            mImgVLoading.setVisibility(View.GONE);
            createMobileView();
             //sendEmptyMessageDelayed(STOPSPLASH, SPLASHTIME);
             super.handleMessage(msg);
        }
    };

//    static {
//    	try{
//    		System.loadLibrary("GGlibrary");
//    	}catch(UnsatisfiedLinkError ule){
//    		System.err.println("WARNING: Could not load library GGlibrary!");
//    	}
//    }

}