package com.jqbar;



import android.content.Context;
import android.graphics.Canvas;
import android.view.View;

public class FillView extends View {

	public FillView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}
	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
	}
	
	
	//来设置本View的宽和高，这样就会显示。注意宽度和高度必须大于设备的宽和高，此时才会滚动。
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		// TODO Auto-generated method stub
//		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		//setMeasuredDimension(GlobalFun.BWScreenWidth, GlobalFun.BWScreenHeight);
	}

}
