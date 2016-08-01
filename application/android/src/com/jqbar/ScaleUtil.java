package com.jqbar;

import android.content.Context;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.WindowManager;

public class ScaleUtil {
	private static WindowManager windowManager = null;
	private static int screenWidth;// 屏宽
	private static int screenHeight;// 屏高
	private static float coefficient_X;//x比例系数
	private static float coefficient_Y;//y比例系数
	private static float coefficient;//比例系数（原图/实际图）
	private static float centerX=757;//原图中心点x坐标
	private static float centerY=589;//原图中心点y坐标
	private static float radius=750;//原图最大圆的半径
	private static float center_x;//手机中中心点的x坐标
	private static float center_y;//手机中中心点的y坐标
	public static float XScale;
	public static float YScale;
    /**
     * 初始化一些变量 
     * @param context
     */
	public static void init(Context context) {
		// 获取屏幕宽高
		DisplayMetrics dm = new DisplayMetrics();
		windowManager = (WindowManager) context.getApplicationContext()
				.getSystemService(Context.WINDOW_SERVICE);
		windowManager.getDefaultDisplay().getMetrics(dm);
		screenWidth = dm.widthPixels;// 宽度
		screenHeight = dm.heightPixels;// 高度
		Log.i("tag", screenWidth+"---"+screenHeight);
		coefficient_Y=radius/(screenHeight/2);
		coefficient_X=radius/(screenWidth/2);
		if(coefficient_X>coefficient_Y){
			coefficient=coefficient_X;
		}else{
			coefficient=coefficient_Y;
		}
		center_x=screenWidth / 2;
		center_y=screenHeight / 3;
		
	}
     /**
      * 根据原图中运动点的x坐标得出在手机上运动点的x坐标
      * @param x 原图中运动点的x坐标
      * @return 手机上运动点的x坐标
      */
	public static float getXScale(float x) {
//		return XScale=(float) (screenWidth / 2-757+(757-x)*(1.6-1)+x);
		return XScale=(float) (center_x-(centerX-x)/coefficient);
		
	}
	/**
     * 根据原图中运动点的y坐标得出在手机上运动点的y坐标
     * @param x 原图中运动点的y坐标
     * @return 手机上运动点的y坐标
     */
	public static float getYScale(float y) {
//		return YScale= (float) (screenHeight / 4-589+(589-y)*(1.6-1)+y);
		return YScale= (float) (center_y-(centerY-y)/coefficient);
	}

	public static int getScreenWidth() {
		return screenWidth;
	}


	public static int getScreenHeight() {
		return screenHeight;
	}
	
	
	public static void setScreenWidth(int screenWidth) {
		ScaleUtil.screenWidth = screenWidth;
	}
	public static void setScreenHeight(int screenHeight) {
		ScaleUtil.screenHeight = screenHeight;
	}
	public static float getCoefficient(){
		return coefficient;
	}

	public static float getCenter_x() {
		return center_x;
	}


	public static float getCenter_y() {
		return center_y;
	}
	
}
