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

import android.view.animation.Animation;
import android.view.animation.Transformation;

/**
 * An animation that controls the position of an object, and make the object
 * rotate on its center at the same time.
 * 
 */
public class MyTranslateAnimation extends Animation {
	private int mFromXType = ABSOLUTE;

	private int mToXType = ABSOLUTE;

	private int mFromYType = ABSOLUTE;

	private int mToYType = ABSOLUTE;

	private float mFromXValue = 0.0f;

	private float mToXValue = 0.0f;

	private float mFromYValue = 0.0f;

	private float mToYValue = 0.0f;

	private float mFromXDelta;

	private float mToXDelta;

	private float mFromYDelta;

	private float mToYDelta;

	private float mXDelta;

	private float mYDelta;
	
	private int mPivotXType = ABSOLUTE;

    private int mPivotYType = ABSOLUTE;

    private float mPivotXValue = 0.0f;

    private float mPivotYValue = 0.0f;

    private float mPivotX;

    private float mPivotY;

	
	public MyTranslateAnimation(float fromXDelta, float toXDelta,
			float fromYDelta, float toYDelta, float xDelta, float yDelta) {
		mFromXValue = fromXDelta;
		mToXValue = toXDelta;
		mFromYValue = fromYDelta;
		mToYValue = toYDelta;
		mXDelta = xDelta;
		mYDelta = yDelta;

		mFromXType = ABSOLUTE;
		mToXType = ABSOLUTE;
		mFromYType = ABSOLUTE;
		mToYType = ABSOLUTE;
		
		mPivotXValue = 0.5f;
        mPivotXType = RELATIVE_TO_SELF;
        mPivotYValue = 0.5f;
        mPivotYType = RELATIVE_TO_SELF;

	}

	@Override
	protected void applyTransformation(float interpolatedTime, Transformation t) {
//		float sx = mFromXDelta;
//        float sy = mFromYDelta;
//        if (mFromXDelta != mToXDelta) {
//            sx = mFromXDelta + ((mToXDelta - mFromXDelta) * interpolatedTime);
//        }
//        if (mFromYDelta != mToYDelta) {
//            sy = mFromYDelta + ((mToYDelta - mFromYDelta) * interpolatedTime);
//        }
//		if (mPivotX == 0.0f && mPivotY == 0.0f) {
//            t.getMatrix().setScale(1, 1);
//        } else {
//            t.getMatrix().setScale(sx, sy, mPivotX, mPivotY);
//        }
//		t.getMatrix().postScale(2, 2);
		
		float dx = mFromXDelta;
		float dy = mFromYDelta;
		if (mFromXDelta != mToXDelta) {
			dx = calcBezier(interpolatedTime, mFromXDelta, mXDelta, mToXDelta);
		}
		if (mFromYDelta != mToYDelta) {
			dy = calcBezier(interpolatedTime, mFromYDelta, mYDelta, mToYDelta);
		}

		t.getMatrix().postTranslate(dx, dy);
	}

	@Override
	public void initialize(int width, int height, int parentWidth,
			int parentHeight) {
		super.initialize(width, height, parentWidth, parentHeight);
		mFromXDelta = resolveSize(mFromXType, mFromXValue, width, parentWidth);
		mToXDelta = resolveSize(mToXType, mToXValue, width, parentWidth);
		mFromYDelta = resolveSize(mFromYType, mFromYValue, height, parentHeight);
		mToYDelta = resolveSize(mToYType, mToYValue, height, parentHeight);
		
		mPivotX = resolveSize(mPivotXType, mPivotXValue, width, parentWidth);
        mPivotY = resolveSize(mPivotYType, mPivotYValue, height, parentHeight);
	}

	public long calcBezier(float interpolatedTime, float p0, float p1, float p2) {
		return Math.round((Math.pow((1 - interpolatedTime), 2) * p0)
				+ (2 * (1 - interpolatedTime) * interpolatedTime * p1)
				+ (Math.pow(interpolatedTime, 2) * p2));
	}
}