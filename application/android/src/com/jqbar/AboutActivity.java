package com.jqbar;

import com.jqbar.R;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import com.jqbar.MobileMain;

public class AboutActivity extends Activity
{

	private ImageView imgAbout;
	private Button backBtn;
	private TextView versionText;
	LinearLayout linear;
	private ImageView image;
    Intent mIntent = null;
	Bundle mBundle = null;
	private String UA_value=null;
	private String header=null;
	private String strDeviceID = null;
	
    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        super.onCreateOptionsMenu(menu);
        return true;
    }
    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        return onOptionsItemSelected( item );
    }
    @Override
    public void onConfigurationChanged(Configuration newConfig)
    {
       super.onConfigurationChanged(newConfig);
    }
    /**
     * Standard window-focus override. Notice focus lost so we can pause on
     * focus lost. e.g. user switches to take a call.
     */
    @Override
    public void onWindowFocusChanged(boolean hasWindowFocus)
    {
    	super.onWindowFocusChanged(hasWindowFocus);
    	return;
    }

    @Override
    public void onCreate(Bundle savedInstanceState)
    {
    	super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		GlobalFun.setOrientation( this );
		MobileMain mobileMain=new MobileMain();
		if(mobileMain.getOrientation()==1){
    	setContentView(R.layout.aboutactivity_land);
		}else{
		setContentView(R.layout.aboutactivity);	
		}
    	mIntent = this.getIntent();
    	mBundle = mIntent.getExtras();
    	UA_value = mBundle.getString("UA");
    	strDeviceID = mBundle.getString("DeviceID");
    	header=mBundle.getString("header");
    	header=header.replace('#', '\t');
    	header=header.replace('$', '\n');
    	
    	imgAbout = (ImageView)findViewById(R.id.imageView1);
//    	imgAbout.setImageResource(R.drawable.aboutus_logo);
    	imgAbout.setBackgroundResource(R.drawable.aboutus_land);
//    	mImgVLoading.setImageResource(R.drawable.app_loading_logo );
//		mImgVLoading.setBackgroundResource(R.drawable.app_loading_land );
//    	if( MobileMain.screenModeVal() == 0 )
//    	{
//    		imgAbout.setImageResource( R.drawable.aboutus_poheaderrt );
//    	}
//    	else
//    	{
//    		imgAbout.setImageResource( R.drawable.aboutus_land );
//    	}
//    	
    	imgAbout.setOnClickListener(new OnClickListener()
        {
            public void onClick(View v)
            {
            	AboutActivity.this.finish();
            }
        });
//    	title=(TextView)findViewById(R.id.title);
//    	backBtn=(Button)findViewById(R.id.about_back_btn);
//    	int screenWidth=GlobalFun.BWScreenWidth;
//    	String str20 = getString( R.dimen.bwpx20 );
//    	str20 = str20.substring(0, str20.indexOf(".") );
//    	int buttonWidth = Integer.parseInt( str20 );
//    	title.setWidth(screenWidth-buttonWidth);
//    	backBtn.setWidth(buttonWidth);
    	backBtn=(Button)findViewById(R.id.about_back_btn);
    	backBtn.setOnClickListener(new OnClickListener() {
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				AboutActivity.this.finish();
			}
		});
    	//取得版本号
    	versionText=(TextView)findViewById(R.id.versionContext);
    	try {
    		 PackageManager pm = getPackageManager();
    		 PackageInfo pinfo = pm.getPackageInfo(AboutActivity.this.getPackageName(),PackageManager.GET_CONFIGURATIONS);
    		 String versionCode = pinfo.versionName;
    		 versionText.setText(versionCode);
    	 } 
    	catch (NameNotFoundException e)
    	{
    		e.printStackTrace(System.err);
    	}
    	

    	linear=(LinearLayout)findViewById(R.id.words);
    	linear.setPadding(0, 0, 0, MobileMain.mScreenHeight/10);
    	
    	image=(ImageView)findViewById(R.id.imageView1);
    	image.setOnLongClickListener(new ImageView.OnLongClickListener() 
    	{
			public boolean onLongClick(View v) {
				// TODO Auto-generated method stub
				
				Dialog dlg = new AlertDialog.Builder(AboutActivity.this)
		        .setIcon(R.drawable.logo_36)
		        .setTitle("配置信息")
		        .setMessage(UA_value+"\n" + strDeviceID + "\n" +header)
		        .setNegativeButton("确定", new DialogInterface.OnClickListener() {
		            public void onClick(DialogInterface dialog, int whichButton) {
		            }
		        }).create();

		        dlg.show();
		        return true;
			}
		});

    }
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		return super.onKeyDown( keyCode, event);
	}

	public boolean onKeyUp(int keyCode, KeyEvent event)
	{
		switch (keyCode)
		{
		case KeyEvent. KEYCODE_BACK:
			finish();
			return true;
		default:
			return super.onKeyUp(keyCode, event);
		}
	}
}
