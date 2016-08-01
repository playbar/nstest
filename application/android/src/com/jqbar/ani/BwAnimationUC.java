package com.jqbar.ani;

import android.widget.ImageView;

import com.jqbar.R;

public class BwAnimationUC extends BwAnimation {

	public BwAnimationUC() {
		// TODO Auto-generated constructor stub
	}

	public int startAnimation(final ImageView view) {
		view.setImageResource(R.drawable.app_loading_logo_other);
		view.setBackgroundResource(R.drawable.app_loading_land_other);
		
		return ani_fade(view);
	}

}
