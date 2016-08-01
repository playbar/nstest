/*
 * Copyright (C) 2012 Capricorn
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jqbar;



import android.app.Activity;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.animation.AccelerateInterpolator;
import android.view.animation.Animation;
import android.view.animation.AnimationSet;
import android.view.animation.Interpolator;
import android.view.animation.LinearInterpolator;
import android.view.animation.OvershootInterpolator;
import android.view.animation.RotateAnimation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.TranslateAnimation;
import android.widget.Toast;

/**
 * A Layout that arranges its children around its center. The arc can be set by
 * calling {@link #setArc(float, float) setArc()}. You can override the method
 * {@link #onMeasure(int, int) onMeasure()}, otherwise it is always
 * WRAP_CONTENT.
 * 
 * @author Capricorn
 * 
 */
public class ArcLeftTopLayout extends ViewGroup {
    /**
     * children will be set the same size.
     */
    private static int mChildSize=0;

    private int mChildPadding = 5;

    private int mLayoutPadding = 10;
    
    private static final float OFFSET_DEGREES=15.0F;

    public static final float DEFAULT_FROM_DEGREES = 270.0f;

    public static final float DEFAULT_TO_DEGREES = 360.0f;

    private float mFromDegrees = DEFAULT_FROM_DEGREES;

    private float mToDegrees = DEFAULT_TO_DEGREES;

    private static final int MIN_RADIUS = 100;

    private int mRadius;

    private boolean mExpanded = false;
    private int position=1;
    public static final int PLACE_LEFT_TOP = 1;//位置常量--左上
	public static final int PLACE_RIGHT_TOP = 2;//位置常量--右上
	public static final int PLACE_LEFT_DOWN = 3;//位置常量--左下
	public static final int PLACE_RIGHT_DOWN = 4;//位置常量--右下
	private MobileMain context;
	private int Width;
	private int Height;
    public ArcLeftTopLayout(Context context) {
        super(context);
    }
    
    public void setContext(MobileMain context) {
		this.context = context;
	}

    public ArcLeftTopLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
    }
   /**
    * 通过各个参数来计算半径大小
    * @param arcDegrees 扇形的度数
    * @param childCount 子组件的个数
    * @param childSize 子组件的大小
    * @param childPadding 子组件之间的间隙
    * @param minRadius 最小的半径
    * @return
    */
    private static int computeRadius(final float arcDegrees, final int childCount, final int childSize,
            final int childPadding, final int minRadius) {
        if (childCount < 2) {
            return minRadius;
        }
        final float perDegrees = arcDegrees / (childCount - 1);
        final float perHalfDegrees = perDegrees / 2;
        final int perSize = childSize + childPadding;

        final int radius = (int) ((perSize / 2) / Math.sin(Math.toRadians(perHalfDegrees)));

        return Math.max(radius, minRadius);
    }
/**
 * 通过各参数计算子组件所在位置的矩形区域
 * @param centerX 扇形的中心点X坐标 
 * @param centerY 扇形的中心点Y坐标 
 * @param radius 扇形半径
 * @param degrees 子组件偏离的度数
 * @param size 子组件的大小
 * @return
 */
    private Rect computeChildFrame(final int centerX, final int centerY, final int radius, final float degrees,
            final int size) {
        double childCenterX = centerX + radius * Math.cos(Math.toRadians(degrees));
        double childCenterY = centerY + radius * Math.sin(Math.toRadians(degrees));
		// 子组件的起始点
		if (radius == 0) {
			switch (position) {
			case PLACE_LEFT_TOP:
				childCenterX = centerX + size*2/3;
				childCenterY = centerY + size*2/3;
				break;
			case PLACE_RIGHT_TOP:
				childCenterX = centerX - size*2/3;
				childCenterY = centerY + size*2/3;
				break;
			case PLACE_LEFT_DOWN:
				childCenterX = centerX + size*2/3;
				childCenterY = centerY - size*2/3;
				break;
			case PLACE_RIGHT_DOWN:
				childCenterX = centerX - size*2/3;
				childCenterY = centerY - size*2/3;
				break;
			default:
				break;
			}
		}
        return new Rect((int) (childCenterX - size / 2), (int) (childCenterY - size / 2),
                (int) (childCenterX + size / 2), (int) (childCenterY + size / 2));
    }
   
    
/**
 * 计算自定义组件ArcLayout的大小
 */
    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
//      final int radius = mRadius = computeRadius(Math.abs(mToDegrees - mFromDegrees), getChildCount(), mChildSize,
//                mChildPadding, MIN_RADIUS);
    	    //把半径固定200
    		final int radius;
      	if(ScaleUtil.getScreenWidth()<ScaleUtil.getScreenHeight()){
      		radius = mRadius=(int) (ScaleUtil.getScreenWidth()*0.23);
      	}else{
      		radius = mRadius=(int) (ScaleUtil.getScreenHeight()*0.23);
      	}
    	
        final int size = radius * 2 + mChildSize + mChildPadding + mLayoutPadding * 2;
        //计算了一个圆形区域，只显示圆形区域的1/4  宽为1/2  高为1/2
//      setMeasuredDimension(size/2, size/2); 
        
        DisplayMetrics dm = new DisplayMetrics();
        WindowManager windowManager = (WindowManager) context.getApplicationContext()
				.getSystemService(Context.WINDOW_SERVICE);
		windowManager.getDefaultDisplay().getMetrics(dm);
		int measureWidth = dm.widthPixels;// 宽度
		int measureHeight = dm.heightPixels;// 高度
        Log.i("tag", "小球父类的宽和高："+measureWidth+measureHeight);
        setMeasuredDimension(measureWidth, measureHeight); 
        
        final int count = getChildCount();
        for (int i = 0; i < count; i++) {
            getChildAt(i).measure(MeasureSpec.makeMeasureSpec(mChildSize, MeasureSpec.EXACTLY),
                    MeasureSpec.makeMeasureSpec(mChildSize, MeasureSpec.EXACTLY));
        }
        
    }
    /**
     * 计算自定义组件ArcLayout的显示位置
     */
    @Override
    protected void onLayout(boolean changed, int l, int t, int r, int b) {
    		int centerX = 0;
    		int centerY = 0;
    		switch (position) {
    		case PLACE_LEFT_TOP:
    			centerX = 0;
    			centerY = 0;
    			break;
    		case PLACE_RIGHT_TOP:
    			centerX = getWidth();
    			centerY = 0;
    			break;
    		case PLACE_LEFT_DOWN:
    			centerX = 0;
    			centerY = getHeight();
    			break;
    		case PLACE_RIGHT_DOWN:
    			centerX = getWidth();
    			centerY = getHeight();
    			break;
    		default:
    			centerX = 0;
    			centerY = 0;
    			break;
    		}
            final int radius = mExpanded ? mRadius : 0;

            final int childCount = getChildCount();
             //让扇形区域的度数减少30度
            final float perDegrees = (mToDegrees - mFromDegrees-OFFSET_DEGREES*2) / (childCount - 1);
            float degrees ;
            if(position==PLACE_RIGHT_TOP||position==PLACE_RIGHT_DOWN){
            	 //第一个偏移75度
              degrees = mToDegrees-OFFSET_DEGREES;
            }else{
            	 //第一个偏移15度
              degrees = mFromDegrees+OFFSET_DEGREES;
            }
            
            for (int i = 0; i < childCount; i++) {
                Rect frame = computeChildFrame(centerX, centerY, radius, degrees, mChildSize);
                
              	Log.i("tag", position+","+centerX+","+centerY+","+frame.left+","+frame.top+","+frame.right+","+frame.bottom+","+degrees+"度"+",半径为"+radius+",孩子大小为"+mChildSize+"-----");
              	if(position==PLACE_RIGHT_TOP||position==PLACE_RIGHT_DOWN){
              		//子组件递归减perDegrees
              		degrees -= perDegrees;
                }else{
                	    //子组件递归增perDegrees
            	        degrees += perDegrees;
                }
                	getChildAt(i).layout(frame.left, frame.top, frame.right, frame.bottom);
                
            }
        
        
    }
    
    //点击菜单 每一项展开时候的动画
    private static Animation createExpandAnimation(float fromXDelta, float toXDelta, float fromYDelta, float toYDelta,
            long startOffset, long duration, Interpolator interpolator) {
    	    Animation expandAnimation = new TranslateAnimation(0, toXDelta, 0, toYDelta);
    	    expandAnimation.setStartOffset(startOffset);
    	    expandAnimation.setDuration(duration);
    	    expandAnimation.setInterpolator(interpolator);
    	    expandAnimation.setFillAfter(true);

        return expandAnimation;
    }
  //点击菜单 每一项收回时候的动画
    private static Animation createShrinkAnimation(float fromXDelta, float toXDelta, float fromYDelta, float toYDelta,
            long startOffset, long duration, Interpolator interpolator) {
        Animation shrinkAnimation = new TranslateAnimation(0, toXDelta, 0, toYDelta);
        shrinkAnimation.setStartOffset(startOffset);
        shrinkAnimation.setDuration(duration);
        shrinkAnimation.setInterpolator(interpolator);
        shrinkAnimation.setFillAfter(true);
        return shrinkAnimation;
    }

    private void bindChildAnimation(final View child, final int index, final long duration) {
        int centerX = 0;
		int centerY = 0;
		switch (position) {
		case PLACE_LEFT_TOP:
			centerX = 0;
			centerY = 0;
			break;
		case PLACE_RIGHT_TOP:
			centerX = getWidth();
			centerY = 0;
			break;
		case PLACE_LEFT_DOWN:
			centerX = 0;
			centerY = getHeight();
			break;
		case PLACE_RIGHT_DOWN:
			centerX = getWidth();
			centerY = getHeight();
			break;
		default:
			break;
		}
        final int radius = mExpanded ? 0 : mRadius;

        final int childCount = getChildCount();
        final float perDegrees = (mToDegrees - mFromDegrees-OFFSET_DEGREES*2) / (childCount - 1);
        Rect frame ;
        if(position==PLACE_RIGHT_TOP||position==PLACE_RIGHT_DOWN){
          frame = computeChildFrame(centerX, centerY, radius, mToDegrees-OFFSET_DEGREES - index * perDegrees, mChildSize);
        }else{
          frame = computeChildFrame(centerX, centerY, radius, OFFSET_DEGREES+mFromDegrees + index * perDegrees, mChildSize);
        }
        
        final int toXDelta = frame.left - child.getLeft();
        final int toYDelta = frame.top - child.getTop();
        Interpolator interpolator = mExpanded ? new AccelerateInterpolator() : new OvershootInterpolator(1.5f);

//        Animation animation = mExpanded ? createShrinkAnimation(0, toXDelta, 0, toYDelta, 0, duration,
//                interpolator) : createExpandAnimation(0, toXDelta, 0, toYDelta, 0, duration, interpolator);
        Animation animation =createExpandAnimation(0, toXDelta, 0, toYDelta, 0, duration, interpolator);
        animation.setAnimationListener(new AnimationListener() {

            @Override
            public void onAnimationStart(Animation animation) {
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }

            @Override
            public void onAnimationEnd(Animation animation) {
             	
				postDelayed(new Runnable() {

					@Override
					public void run() {
						if (index == 2) {
							onAllAnimationsEnd();
						}
					}
				}, 0);
            }
        });

        child.setAnimation(animation);
    }

    public boolean isExpanded() {
        return mExpanded;
    }
    /**
     * 设置扇形弧度  
     * @param fromDegrees 起始角度
     * @param toDegrees 总之角度
     */
    public void setArc(float fromDegrees, float toDegrees) {
        if (mFromDegrees == fromDegrees && mToDegrees == toDegrees) {
            return;
        }

        mFromDegrees = fromDegrees;
        mToDegrees = toDegrees;
//      requestLayout();
    }
    /**
     * 设置子组件大小啊
     * @param size 组件大小
     */

    public void setChildSize(int size) {
        if (mChildSize == size || size < 0) {
            return;
        }

        mChildSize = size;

//      requestLayout();
    }

    public int getChildSize() {
        return mChildSize;
    }

    /**
     * 给自定义ViewGroup绑定展开收缩动画 
     * @param showAnimation 是否绑定动画
     * @param position 扇形的中心点位置
     */
    public void switchState(final boolean showAnimation,int position) {
    	     
       
        if (showAnimation) {
            final int childCount = getChildCount();
            for (int i = 0; i < childCount; i++) {
                bindChildAnimation(getChildAt(i), i, 250);
            }
        }
        
        this.position=position;
        mExpanded = !mExpanded;
        
      invalidate();
        
        Log.i("tag", "刷新界面");
    }
    
    public void setPosition(int position){
    	 this.position=position;
    }
    
    
   /**
    * 清除子组件的动画效果
    */
    private void onAllAnimationsEnd() {
        final int childCount = getChildCount();
        for (int i = 0; i < childCount; i++) {
            getChildAt(i).clearAnimation();
        }
        Log.i("tag", "动画消失");
        if(!mExpanded){
            setVisibility(View.INVISIBLE);
            context.myHandler.sendEmptyMessage(1);
        }
        
        requestLayout();
        
    }
   

}
