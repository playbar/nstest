package my.global;

import android.widget.Gallery;
import android.content.Context;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.widget.SpinnerAdapter;
import android.view.MotionEvent;

public class OneGallery extends Gallery
{
	
	private imageAdapter m_imageAdapter;
	
	public OneGallery(Context context, AttributeSet attrs)
	{
		super(context,attrs);
	}
	
	@Override
	public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY)
	{
		return false;
	}
	
	@Override
	public void setAdapter (SpinnerAdapter adapter)
	{
		m_imageAdapter = (imageAdapter)adapter;
		super.setAdapter(adapter);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event)
	{
		if(event.getAction() == MotionEvent.ACTION_DOWN)
		{
			m_imageAdapter.stop();
		}
		else if(event.getAction() == MotionEvent.ACTION_UP)
		{
			m_imageAdapter.start();
		}
		return super.onTouchEvent(event);
	}
	
}
