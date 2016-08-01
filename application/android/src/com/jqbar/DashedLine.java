package com.jqbar;
import android.view.View;
import android.content.Context;
import android.util.AttributeSet;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PathEffect;

//



public class DashedLine extends View {
	public DashedLine(Context context, AttributeSet attrs){
		super(context, attrs );
		}
	 
    protected void onDraw(Canvas canvas) { 
        // TODO Auto-generated method stub 
        super.onDraw(canvas);         
        Paint paint = new Paint(); 
       //设置paint的style空心
        paint.setStyle(Paint.Style.STROKE); 
       //　设置paint的颜色
//        paint.setColor(Color.WHITE); 
        paint.setColor(Color.GRAY);
        Path path = new Path();  
        path.moveTo(GlobalFun.BWStartPoint,1); 
        path.lineTo( MobileMain.mScreenWidth-40,1); 
        //可以使用DashPathEffect来创建一个虚线的轮廓(短横线/小圆点)，而不是使用实线。你还可以指定任意的虚/实线段的重复模式。
        PathEffect effects = new DashPathEffect(new float[]{5,5,5,5},1); 
        //数组长度必须>=2，数组的值定义了宽度，第一个实线段长5,后面的空白长5，然后又是一个长5的实线段，然后是5的空白。
        //第二个参数说是偏移量，没发现具体作用
        paint.setPathEffect(effects); 
        canvas.drawPath(path, paint); 
        
    } 

}
