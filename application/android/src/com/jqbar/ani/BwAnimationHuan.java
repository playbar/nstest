package com.jqbar.ani;

import android.widget.ImageView;

import com.jqbar.R;

public class BwAnimationHuan extends BwAnimation {

	public BwAnimationHuan() {
		// TODO Auto-generated constructor stub
	}

	public int startAnimation(final ImageView view) {
		view.setImageResource(R.drawable.app_loading_logo_other);
		view.setBackgroundResource(R.drawable.app_loading_land_other);
		
		return ani_fade(view);
	}

	public void setAboutImage(final ImageView view) {
		view.setImageResource(R.drawable.aboutus_logo_other);
		view.setBackgroundResource(R.drawable.aboutus_land_other);
	}

}
