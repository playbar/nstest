package com.jqbar;

import com.jqbar.R;

import android.app.Activity;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;


public class HelpActivity extends Activity {
	private Button backButton;
	

    
    public void onCreate(Bundle savedInstanceState)
    {
    	super.onCreate(savedInstanceState);

		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		
		GlobalFun.setOrientation( this );
		
    	setContentView(R.layout.helpactivity);
    	backButton=(Button)findViewById(R.id.help_back_btn);
//    	title=(TextView)findViewById(R.id.title);
//    	int screenWidth=GlobalFun.BWScreenWidth;
//    	String str20 = getString( R.dimen.bwpx20 );
//    	str20 = str20.substring(0, str20.indexOf(".") );
//    	int buttonWidth = Integer.parseInt( str20 );
//    	backButton.setWidth(buttonWidth);
//    	title.setWidth(screenWidth-buttonWidth);
    	backButton.setOnClickListener(new Button.OnClickListener() {
			
			public void onClick(View v) {
				// TODO Auto-generated method stub
				HelpActivity.this.finish();
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
