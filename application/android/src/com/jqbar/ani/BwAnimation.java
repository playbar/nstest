package com.jqbar.ani;

import com.jqbar.R;

import android.view.View;
import android.view.animation.AlphaAnimation;
import android.view.animation.AnimationSet;
import android.widget.ImageView;

public class BwAnimation {

	public BwAnimation() {
		// TODO Auto-generated constructor stub
	}

	public int ani_fade(View view) {
		AnimationSet animationSet = new AnimationSet(true);
		AlphaAnimation alphaAnimation = new AlphaAnimation(0.0f, 1.0f);
		alphaAnimation.setDuration(500);
		AlphaAnimation alphaAnimation1 = new AlphaAnimation(1.0f, 0.0f);
		alphaAnimation1.setDuration(500);
		alphaAnimation1.setStartOffset(2500);
		animationSet.addAnimation(alphaAnimation);
		animationSet.addAnimation(alphaAnimation1);
		view.startAnimation(animationSet);
		return 3000;
	}

	public int startAnimation(final ImageView view) {
		return 500;
	}

	public void setAboutImage(final ImageView view) {
		view.setImageResource(R.drawable.aboutus_logo);
		view.setBackgroundResource(R.drawable.aboutus_land);
	}

}
