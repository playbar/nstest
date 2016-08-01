package my.global;

import java.util.Timer;
import java.util.TimerTask;

//import my.test.GifDecoder;

//import com.jqbar.android.sptest.R;
//import com.jqbar.android.sptest.R.drawable;
//import com.jqbar.android.sptest.R.styleable;
import com.jqbar.android.spsgyx.*;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.content.res.TypedArray;
import my.global.*;

public class imageAdapter extends BaseAdapter 
{
	private Integer m_rImgId[] = {R.drawable.img_a,R.drawable.img_b}; 
	private Context m_Context;
	private int m_nItemBg;
	private ImageView m_rView[];
	private GifDecoder m_decoder;
	private boolean	m_isStop;
    private Handler m_handler= new Handler(){
    	public void handleMessage(Message msg)
    	{
    		switch(msg.what)
    		{
    		case 1:
    			m_rView[1].setImageBitmap(m_decoder.nextFrame());
    			break;
    		}
    	}
    };
    private TimerTask m_task = new TimerTask(){
    	public void run(){
    		if(!m_isStop)
    		{
	    		Message m = m_handler.obtainMessage();
	    		m.what = 1;
	    		m_handler.sendMessage(m);
    		}
    	}
    };

    private Timer m_timer;
	
	
	public imageAdapter(Context context)
	{
		m_Context = context;
		TypedArray a = context.obtainStyledAttributes(R.styleable.Gallery);
		m_nItemBg = a.getResourceId(R.styleable.Gallery_android_galleryItemBackground, 0);
		a.recycle();
		m_rView = new ImageView[3];

			m_rView[0] = new ImageView(m_Context);
			m_rView[0].setImageResource(m_rImgId[0]);
			m_rView[0].setScaleType(ImageView.ScaleType.FIT_CENTER);
			m_rView[2] = new ImageView(m_Context);
			m_rView[2].setImageResource(m_rImgId[1]);
			m_rView[2].setScaleType(ImageView.ScaleType.FIT_CENTER);

		m_decoder = new GifDecoder(GlobalUtil.OpenApkFile(m_Context, "game.gif"));
		m_decoder.decodeImage();
			m_rView[1] = new ImageView(m_Context);
			m_rView[1].setImageBitmap(m_decoder.curFrame());
			m_rView[1].setScaleType(ImageView.ScaleType.FIT_CENTER);
		m_timer = new Timer(true);
		m_timer.schedule(m_task, m_decoder.getDelay(),1000);
		
	}
	
	public int getCount() {
		// TODO Auto-generated method stub
//		return m_rView.length;
		return 600;
	}

	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return m_rView[position%3];
	}

	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position%3;
	}

	public View getView(int position, View convertView, ViewGroup parent) 
	{
		// TODO Auto-generated method stub
//		ImageView v = new ImageView(m_Context);
//		v.setImageResource(m_rImgId[position]);
//		v.setScaleType(ImageView.ScaleType.FIT_CENTER);
//		return v;
		return m_rView[position%3];
	}
	public void start()
	{
		m_isStop = false;
	}
	public void stop()
	{
		m_isStop = true;
	}
	
	public void cancel()
	{
		if(m_timer != null)
		{
			m_timer.cancel();
			m_timer = null;
		}
	}

}
