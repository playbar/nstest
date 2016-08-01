package com.jqbar.android.spsgyx;

import android.app.Activity;
import android.net.Uri;
import android.os.Bundle;
import android.widget.Button;
import android.view.View.OnClickListener;
import android.view.View;
import android.content.ComponentName;
import android.content.Intent;
import android.content.pm.PackageManager.NameNotFoundException;
//for receiver
import android.content.IntentFilter;
//import jqbar.bw.R;
import java.io.*;

import android.os.Build;
import android.os.Handler;
import android.os.Message;
import android.os.Bundle;
//for gallery
//import jqbar.bw.R.drawable;
//import jqbar.bw.R.id;
//import jqbar.bw.R.layout;
//import jqbar.bw.R.string;
import android.widget.Gallery;
//for image width
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
//for device width
import android.telephony.TelephonyManager;
import android.util.DisplayMetrics;
//for imei
import android.content.Context;
//device id
import android.os.Build;

//添加桌面图标用
import android.content.pm.PackageInfo;
import java.util.List;
import android.graphics.drawable.BitmapDrawable;
//gif动画用
import java.util.TimerTask;
import java.util.Timer;
import my.global.*;
import android.widget.ImageView;

public class GhostActivity extends Activity implements OnClickListener , MSocketObserver
{
	protected Button		m_button;
	protected boolean		m_canStart=false;		//major application have own
	protected boolean		m_canInstall=false;		//apk can install
	protected ZSocket		m_socket=null;			//socket
	protected boolean		m_canDownload = false;	//application can be download
	protected String		m_Ure;					//application ure;
	protected boolean		m_isApk = false;		//apk operation;
	private   String        m_mobileType;			//moblie type;
	long						m_length=0L;
	long						m_currentLen=0L;
	protected Gallery		m_gallery;
	protected imageAdapter	m_adapter;
	
//	protected TimerTask		m_task = new TimerTask(){
//		public void run(){
//    		Message m = m_handler.obtainMessage();
//    		m.what = 6;			//发送gif换图消息
//    		m_handler.sendMessage(m);
//		}
//	};
//	protected Timer			m_timer;
//	protected GifDecoder	m_decoder;
//	protected PackageReceiver m_receiver;
	public Handler m_handler = new Handler()
	{
		@Override
		public void handleMessage(Message msg)
		{
			switch(msg.what)
			{
			case 1:		//start download
				{
					m_button.setEnabled(false);
					m_button.setText(R.string.linking);
//					m_button.setVisibility(View.INVISIBLE);
				}
				break;
			case 2:		//loading
				{
					m_currentLen+=msg.arg1;
					if(m_currentLen>m_length)
						m_button.setText(R.string.derror);
					else
					{
						String str = getString(R.string.loaded);
						int percent;
						percent = (int)(m_currentLen * 1.0 / m_length * 100.0);
						m_button.setText(str+percent+"% "+"("+m_length/1024/1024+"MB"+")");
					}
				}
				break;
			case 3:		//major application is ready
				{
					m_button.setEnabled(true);
					m_button.setText(R.string.install);
//					m_button.setVisibility(View.VISIBLE);
					m_canInstall = true;
				}
				break;
			case 4:		//data has error
				{
					m_button.setText(R.string.lerror);
					//断点记录
					String currentlen = "Content-Length: ";
					currentlen+=m_currentLen;
					currentlen+="\r\n";
					String datalen = "Data-Length: ";
					datalen+=m_length;
					datalen+="\r\n";
			    	File fl = new File("sdcard/jqbar/msg.dat");
			    	FileOutputStream fos;
			    	try
			    	{
			    		fos= new FileOutputStream(fl,false);
//			        	String tempStr = new String(str,0,len);
			        	fos.write(currentlen.getBytes(),0,currentlen.length());
			        	fos.write(datalen.getBytes(),0,datalen.length());
			        	fos.close();
			    	}
			    	catch(IOException e)
			    	{
			    		e.printStackTrace();
			    		return;
			    	}
				}
				break;
			case 5:		//Redirect 
				{
					m_socket.Connect("m.jqbar.com", GhostActivity.this);
				}
				break;
//			case 6:		//m_gallery中的gif动画
//				{
//					ImageView m = (ImageView)m_gallery.getItemAtPosition(1);
//					if(m_gallery.findFocus() == m)
//					{
//						m.setImageBitmap(m_decoder.nextFrame());
//					}
//				}
			}
		}
	};
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        if(m_socket != null) return;
        m_gallery = (my.global.OneGallery)findViewById(R.id.oneGallery1);
        //image width
        int imgW;
        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inJustDecodeBounds = true;
        Bitmap bmp = BitmapFactory.decodeResource(this.getResources(), R.drawable.img_a, options);
        imgW = options.outWidth;
        //device width
        int dvcW,dvcH;
        DisplayMetrics dm = new DisplayMetrics();
        dm = this.getApplicationContext().getResources().getDisplayMetrics();
        dvcW = dm.widthPixels;
        dvcH = dm.heightPixels;
        //mobile type
    	Build bd = new Build();
    	m_mobileType = bd.MODEL;
    	m_mobileType+="/";
    	m_mobileType+=dvcW;
    	m_mobileType+="*";
    	m_mobileType+=dvcH;
    	m_mobileType+="/2G";
        
        int bound = (dvcW-imgW)/2;
        if(bound<0) bound=0;
        m_gallery.setSpacing(bound);
        m_adapter = new imageAdapter(this);
        m_gallery.setAdapter(m_adapter);
        m_gallery.setSelection(300);
//        m_gallery.getPersistentDrawingCache();
//        m_decoder = new GifDecoder(GlobalUtil.OpenApkFile(this, "walking.gif"));
//        m_decoder.decodeImage();
//        m_timer = new Timer(true);
//        m_timer.schedule(m_task, 500 , 10000);
        m_button = (Button)findViewById(R.id.button1);
        m_button.setOnClickListener(this);
        
    	File dir = new File("sdcard/jqbar");
    	if(!dir.exists())
    	{
    		dir.mkdir();
    	}
        m_canStart = checkAPK();
        if(m_canStart)
        {
        	m_button.setText(R.string.start);
        	onClick(m_button);
        }
        else
        {
        	m_button.setText(R.string.download);
        }

    }
    
    @Override
    public void onStart()
    {
//    	m_receiver = new PackageReceiver();
//		IntentFilter filter = new IntentFilter();
//		//向过滤器中添加action
//		filter.addAction("android.intent.action.PACKAGE_ADDED");
//		//注册广播
//		registerReceiver(m_receiver, filter);
    	super.onStart();
    }
    
    @Override
    public void onRestart()
    {
//    	unregisterReceiver(m_receiver);
    	super.onRestart();
    }
    @Override
    public void onResume()
    {
    	
    	System.out.println("on resume");
        m_canStart = checkAPK();
        if(m_canStart)
        {
        	m_button.setText(R.string.start);
        }
//        desktop();

    	super.onResume();
    }

    @Override
    public void onPause()
    {
    	System.out.println("on pause");

    	super.onPause();
    }
    
    @Override
    public void onStop()
    {
    	super.onStop();
    }
    
    @Override
    public void onDestroy()
    {
//    	unregisterReceiver(m_receiver);
//    	m_timer.cancel();
//    	m_timer.purge();
    	m_adapter.cancel();
    	super.onDestroy();
    }


    
    protected boolean checkAPK()
    {
    	try{
    	this.getPackageManager().getPackageInfo("com.jqbar.android.bw", 0);
    	return true;
    	}
    	catch(NameNotFoundException e)
    	{
    		return false;
    	}
    }
    
    //come from OnClickListener
    public void onClick(View v)
    {
    	if(v == m_button)
    	{
    		if(m_canStart)
    		{
    	        Intent temp = new Intent();
    	        temp.setComponent(new ComponentName("com.jqbar.android.bw","com.jqbar.android.bw.MobileMain"));
    	        //����Ϣ
//    	        temp.putExtra(name, value);
    	        Bundle msgBundle = new Bundle();
    	        String spiritname = this.getString(R.string.spirit_code_name);
    	        String spiritvalue = this.getString(R.string.spirit_code_value);
    	        msgBundle.putString(spiritname, spiritvalue);
    	        String channelname = getString(R.string.channel_id_name);
    	        String channelvalue = getString(R.string.channel_id_value);
    	        msgBundle.putString(channelname, channelvalue);
    	        temp.putExtras(msgBundle);
    	        temp.setAction(Intent.ACTION_MAIN);
    	        startActivity(temp); 
    	        System.exit(0);
    		}
    		else if(m_canInstall)
    		{
    			OnFinish();
    		}
    		else
    		{
    			//特种版本，本地内有百玩安装包
    			m_isApk = movefile("ggclient.apk","sdcard/jqbar/ggclient.apk");
    			if(m_isApk)
    			{
    				OnFinish();
    			}
    			else
    			{
	    			m_socket = new ZSocket();
	    			m_socket.Connect("m.jqbar.com", this);
    			}
    			m_button.setEnabled(false);					//2012/2/9=======连续点击出现消息冲突
    		}
    	}
    }
    
    //come from MSocketObserver
    public void OnConnect()
    {
    	File dir = new File("sdcard/jqbar");
    	if(!dir.exists())
    	{
    		dir.mkdir();
    	}
    	File fl = new File("sdcard/jqbar/ggclient.apk");
    	if(fl.exists())
    	{
    		fl.delete();
    	}
    	HttpHead head=new HttpHead();
//    	head.Create("/Download/soft/bw_v2.2.1_android2.2_86660266.apk", "GET");
    	if(m_canDownload)
    		head.Create(m_Ure, "GET" );
    	else
//    		head.Create("/Download/goust.aspx", "GET");
    		head.Create("/spiritpack/", "GET");
    	head.addHead("host", "m.jqbar.com");
    	head.addHead("Accept", "*/*");
    	head.addHead("MOBILE_TYPE", getMobileType());
//    	head.addHead("Connection", "keep-alive");
    	head.addHead("Accept-Language", "chinese;ch;chinese;zh;");
    	head.addHead("Accept-Charse", "utf-8;q=0.80");
//    	m_socket.Send(getString(R.string.http_head).toString());
    	try
    	{
    	m_socket.Send(new String(head.getUtf8(),"UTF-8"));
    	m_socket.StartReceive();
    	Message m = m_handler.obtainMessage();
    	m.what = 1;
    	m.arg1 = 0;
    	m_handler.sendMessage(m);
    	}
    	catch(IOException e)
    	{
    		e.printStackTrace();
    		return;
    	}
    }
  //come from MSocketObserver
    public void OnStart(byte[] str,int len)
    {
    	if(!m_canDownload)
    	{
    		m_canDownload=findlink(str,len);
    		return;
    	}
    	m_isApk = true;
		String cutup = "\r\n\r\n";
		String nameend = "\r\n";
		String cententlen = "Content-Length: ";
    	String tempStr = new String(str,0,len);
    	int offset;
    	int endoffset;
    	//get content-length
    	offset=tempStr.indexOf(cententlen);
    	if(offset!=-1)
    	{
	    	offset+=cententlen.length();
	    	endoffset = tempStr.indexOf(nameend,offset);
	    	endoffset-=offset;
	    	if(endoffset!=0)
	    	{
	    		String datalen = new String(str,offset,endoffset);
	    		m_length = Long.parseLong(datalen);
	    		
	    	}
    	
    	}
    	offset = tempStr.indexOf(cutup);
    	offset+=cutup.length();
    	if(len<=offset) return;
    	File dir = new File("sdcard/jqbar");
    	if(!dir.exists())
    	{
    		dir.mkdir();
    	}
    	File fl = new File("sdcard/jqbar/ggclient.apk");
    	if(!fl.exists())
    	{
    		try{
    		fl.createNewFile();
    		}
    		catch(IOException e)
    		{
        		e.printStackTrace();
        		return;
    		}
    	}
    	FileOutputStream fos;
    	try
    	{
    		fos= new FileOutputStream(fl,true);
//        	tempStr = new String(str,offset,len-offset);
        	fos.write(str,offset,len-offset);
        	fos.close();
        	Message m = m_handler.obtainMessage();
        	m.what = 2;
        	m.arg1 = len-offset;
        	m_handler.sendMessage(m);
    	}
    	catch(IOException e)
    	{
    		e.printStackTrace();
    		return;
    	}
    }
    //come from MSocketObserver
    public void OnReceive(byte[] str,int len)
    {
    	if(!m_canDownload)
    	{
    		m_canDownload=findlink(str,len);
    		return;
    	}
    	File fl = new File("sdcard/jqbar/ggclient.apk");
    	FileOutputStream fos;
    	try
    	{
    		fos= new FileOutputStream(fl,true);
//        	String tempStr = new String(str,0,len);
        	fos.write(str,0,len);
        	fos.close();
        	Message m = m_handler.obtainMessage();
        	m.what = 2;
        	m.arg1 = len;
        	m_handler.sendMessage(m);
    	}
    	catch(IOException e)
    	{
    		e.printStackTrace();
    		return;
    	}
    }
    
    public void OnFinish()
    {
    	if(m_isApk)
    	{
	    	Uri uri = Uri.fromFile(new File("sdcard/jqbar/ggclient.apk"));
	    	Intent intent = new Intent(Intent.ACTION_VIEW);
	    	intent.setDataAndType(uri, "application/vnd.android.package-archive");
	    	startActivity(intent);
	    	Message m = m_handler.obtainMessage();
	    	m.what = 3;
	    	m.arg1 = 0;
	    	m_handler.sendMessage(m);
    	}
    	else
    	{
    		if(m_canDownload)
    		{
    			Message m = m_handler.obtainMessage();
    	    	m.what = 5;
    	    	m.arg1 = 0;
    	    	m_handler.sendMessage(m);
    		}
    		else
    		{
    	    	Message m = m_handler.obtainMessage();
    	    	m.what = 4;
    	    	m.arg1 = 0;
    	    	m_handler.sendMessage(m);
    		}
    	}
    }
    
    public void OnTimeOut()
    {
    	if(m_currentLen == m_length)
    		OnFinish();
    	else
    	{
        	Message m = m_handler.obtainMessage();
        	m.what = 4;
        	m.arg1 = 0;
        	m_handler.sendMessage(m);
    	}
    }
    private boolean findlink(byte[] str,int len)
    {
    	String temp;

    	temp = new String(str,0,len);

    	String test;
    	String a = "<a";
    	String end = ">";
    	String over = "</a";
    	String link = "href";
    	String dqm = "\"";
    	String head = "http://";
    	String sc = "/";
    	
    	int offset;
    	int endoffset;
    	
    	
    	
    	//---------------------test--------------------------
//    	String test1 = "</form>";
//    	offset = temp.indexOf(test1);
//    	offset += test1.length();
//    	try{
//    	test = new String(temp.getBytes("UNICODE"),offset*2,(len-offset)*2);
//    	}
//    	catch(IOException e)
//    	{
//    		e.printStackTrace();
//    		return false;
//    	}
//    	test = new String(str,offset,len-offset);
    	//---------------------end test----------------------
    	
    	offset = temp.indexOf(a);
    	if(offset < 0) return false;
    	offset += a.length();
//    	test = new String(str,offset,len-offset);	//test
    	
    	offset = temp.indexOf(link,offset);
    	if(offset < 0) return false;
    	offset += link.length();
//    	test = new String(str,offset,len-offset);	//test
    	
    	offset = temp.indexOf(dqm,offset);
    	if(offset < 0) return false;
    	offset += dqm.length();
//    	test = new String(str,offset,len-offset);	//test
    	
    	endoffset = temp.indexOf(dqm,offset);
    	if(endoffset < 0) return false;
    	if(endoffset-offset <= 1) return false;
    	
    	offset = temp.indexOf(head,offset);
    	offset += head.length();
//    	test = new String(str,offset,len-offset);	//test
    	
    	offset = temp.indexOf(sc,offset);
    	if(offset < 0) return false;
//    	offset += sc.length();
//    	test = new String(str,offset,len-offset);	//test
    	
    	if(endoffset-offset<=0) return false;
    	offset+=4;endoffset+=4;			//what is fuck
    	
    	m_Ure = new String(str,offset,endoffset-offset);
    	
    	return true;
    }
    private String getMobileType()
    {
//    	String mt;
//    	Context context = getApplicationContext();
//    	TelephonyManager tm = (TelephonyManager)context.getSystemService(Context.TELEPHONY_SERVICE);
//    	mt = tm.getDeviceId();
//    	Build bd = new Build();
//    	mt = bd.MODEL;
//    	return mt;
    	return m_mobileType;
    }
    
    //桌面添加图标  2012年2月27日 修改，这个方式变成了一个可笑的方式
    private void desktop()
    {
    	File fl = new File("sdcard/jqbar/top.dat");
    	if(!fl.exists())
    	{
    		try{
    		fl.createNewFile();
    		}
    		catch(IOException e)
    		{
        		e.printStackTrace();
        		return;
    		}
    	}
    	else
    	{
    		return;
    	}
    	List<PackageInfo> packages = getPackageManager().getInstalledPackages(0);
    	AppInfo tmpInfo = new AppInfo();
    	int i;
    	for(i=0;i<packages.size();i++)
    	{
    		PackageInfo packageInfo = packages.get(i);
    		tmpInfo.packagename = packageInfo.packageName;
    		if(!tmpInfo.packagename.equals("com.jqbar.android.bw")) continue;
    		tmpInfo.appname = packageInfo.applicationInfo.loadLabel(getPackageManager()).toString();    		
    		tmpInfo.versionName = packageInfo.versionName;
    		tmpInfo.versionCode = packageInfo.versionCode;
    		tmpInfo.appicon = packageInfo.applicationInfo.loadIcon(getPackageManager());
    		break;
    	}
    	if(i>=packages.size()) return;
    	
    	//有包
    	Intent intent = new Intent("com.android.launcher.action.INSTALL_SHORTCUT");
    	intent.putExtra(Intent.EXTRA_SHORTCUT_NAME, tmpInfo.appname);
    	intent.putExtra("duplicate", false);
    	
    	Intent intent2 = new Intent(Intent.ACTION_MAIN);
    	intent2.addCategory(Intent.CATEGORY_LAUNCHER);
    	// 删除的应用程序的ComponentName，即应用程序包名+activity的名字
    	intent2.setComponent(new ComponentName(tmpInfo.packagename, tmpInfo.packagename + ".MobileMain"));
    	
    	intent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, intent2);
    	BitmapDrawable bpd = (BitmapDrawable)tmpInfo.appicon;
    	Bitmap bp = bpd.getBitmap();
    	intent.putExtra(Intent.EXTRA_SHORTCUT_ICON,bp);
    	sendBroadcast(intent);
    	
    	
    }
    
    public boolean movefile(String sfname,String dfname)
    {
    	InputStream input = null;
    	input = GlobalUtil.OpenApkFile(this, sfname);
    	if(input == null) return false;
    	File fl = new File(dfname);
    	if(fl.exists())
    	{
    		return true;
    	}
    	else
    	{
    		try
    		{
	    		fl.createNewFile();
	    		OutputStream output = null;
	    		output = new FileOutputStream(fl);
	    		int len;
	    		byte buf[] = new byte[1024];
	    		while((len = input.read(buf, 0, 1024)) != -1)
	    		{
	    			output.write(buf,0,len);
	    		}
	    		input.close();
	    		output.close();
    		}
    		catch(IOException e)
    		{
        		e.printStackTrace();
        		return false;
    		}
    	}
    	return true;
    	
    }
    
    
}