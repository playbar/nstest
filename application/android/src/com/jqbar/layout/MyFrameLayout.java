package com.jqbar.layout;

import android.content.Context;
import android.util.AttributeSet;
import android.util.Log;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;

public class MyFrameLayout extends FrameLayout{
//软件盘监听
	private onResizeListener listener;
	
	public interface onResizeListener
	{
		 void OnResize(int w, int h, int oldw, int oldh);
	}
	
	 public void setOnResizeListener(onResizeListener l) { 
		 listener = l;
	 }
	
	public MyFrameLayout(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
//		Log.e("onMeasure ", "=>onMeasure called! widthMeasureSpec=" + widthMeasureSpec + ", heightMeasureSpec=" + heightMeasureSpec); 
	}
	@Override
	protected void onLayout(boolean changed, int left, int top, int right,
			int bottom) {
		// TODO Auto-generated method stub
		super.onLayout(changed, left, top, right, bottom);
		//Log.e("onLayout ", "=>OnLayout called! changed="+ changed+",l=" + left + ", t=" + top + ",r=" + right + ",b="+bottom);   
		if(listener!=null)
		{
			listener.OnResize(left,top,right,bottom);
		}
	}
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		// TODO Auto-generated method stub
		super.onSizeChanged(w, h, oldw, oldh);
//		 Log.e("onSizeChanged ", "=>onResize called! w="+w + ",h="+h+",oldw="+oldw+",oldh="+oldh);   
	}

}
