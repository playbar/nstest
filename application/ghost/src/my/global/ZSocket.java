package my.global;

import java.lang.System;
import java.lang.Thread;
import java.net.*;
import java.lang.String;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.OutputStream;
import java.io.InputStream;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;



import android.os.Message;


public class ZSocket extends Thread 
{
	protected Socket 				m_socket;	//client socket
	protected InputStream			m_inStream;
	protected OutputStream			m_outStream;
	protected MSocketObserver		m_Observer;	//listener for output data when call StartReceive
	protected enum Type{ENone,EConnect,EReceive};
	protected Type					m_type[];
	protected String				m_url;
	protected int					m_pushevent=0;
	protected int					m_popevent=0;
	private   Event					m_event;
	private   Event					m_stop;
	
	public ZSocket() 
	{
		m_socket = null;
		m_inStream = null;
		m_outStream = null;
		m_type = new Type[16];
		m_event = new Event();
		m_stop = new Event();
	}
	
	public void Connect(String Url,MSocketObserver observer)
	{
		try{
		close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		m_Observer = observer;
//		m_type[m_eventcount] = Type.EConnect;
		push(Type.EConnect);
		m_event.setEvent();
		m_url = Url;
		
		if(!isAlive())
		{
			start();
		}
	}
	
	public String Receive(int len)
	{
		
		byte[] ch;
		ch = new byte[len];
		try
		{
			m_inStream.read(ch,0,len);
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		//-------------------------------
		return ch.toString();
	}
	
	public void StartReceive()
	{
		push(Type.EReceive);
		m_event.setEvent();
	}
	
	public void Send(String data)
	{
		if(!m_socket.isConnected())
			return;
		try
		{
//			String utf8 = new String(data.getBytes("UNICODE"),"UTF-8");
//			m_outStream.write(data);
			m_outStream.write(data.getBytes("UTF-8"));
			m_outStream.flush();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
	}
	
	public void close() throws Exception
	{
		if(m_inStream!=null)
		{
			m_inStream.close();
			m_inStream = null;
		}
		if(m_outStream!=null)
		{
			m_outStream.close();
			m_outStream = null;
		}
		if(m_socket != null)
		{
			if(!m_socket.isClosed())
				m_socket.close();
			m_socket = null;
		}

//		if(isAlive())
//		{
//			this.destroy();
//		}
	}
	
	
	//come from Thread
	public void run()
	{
		while(true)
		{
			if(m_event.isEvent(1000000))
			{
				switch(pop())
				{
				case EConnect:
					try
					{
						m_socket = new Socket();
						InetAddress addr = InetAddress.getByName(m_url);
						InetSocketAddress address = new InetSocketAddress(addr,80);
						m_socket.connect(address,5000);
						m_socket.setSoTimeout(60000);
	//					m_outStream = new BufferedWriter(new OutputStreamWriter(m_socket.getOutputStream()));
						m_outStream = m_socket.getOutputStream();
	//					m_inStream = new BufferedReader(new InputStreamReader(m_socket.getInputStream()));
						m_inStream = m_socket.getInputStream();
						m_Observer.OnConnect();
					}
					catch(Exception e)
					{
						m_Observer.OnTimeOut();
						Release();
						e.printStackTrace();
						return;
					}
					break;
				case EReceive:
					byte[] str;
					str = new byte[256001];
					try
					{
						int len;
						if((len = m_inStream.read(str,0,1024))!=-1)
						{
							//-------------------test---------------------
							//-------------------massage be copy---------------
//					    	File fl = new File("sdcard/ghostlog.txt");
//					    	if(!fl.exists())
//					    	{
//					    		try{
//					    		fl.createNewFile();
//					    		}
//					    		catch(IOException e)
//					    		{
//					        		e.printStackTrace();
//					        		return;
//					    		}
//					    	}
//					    	FileOutputStream fos;
//					    	try
//					    	{
//					    		fos= new FileOutputStream(fl,true);
//	//				        	tempStr = new String(str,offset,len-offset);
//					        	fos.write(str,0,len);
//					        	fos.close();
//					    	}
//					    	catch(IOException e)
//					    	{
//					    		e.printStackTrace();
//					    		return;
//					    	}
					    	//--------------------test over-----------------
							m_Observer.OnStart(str,len);
						}
						while((len = m_inStream.read(str,0,256000)) != -1)
						{
							m_Observer.OnReceive(str,len);
						}
						m_Observer.OnFinish();
					}
					catch(Exception e)
					{
						m_Observer.OnTimeOut();
						e.printStackTrace();
					}
					break;
				case ENone:
					m_event.resetEvent();
					break;
				}
				if(m_stop.isEvent(0))
					return;
			}
		}
	}
	
	public void Release()
	{

		try
		{
			close();
		}
		catch(Exception e)
		{
			e.printStackTrace();
		}
		m_event.setEvent();
		m_stop.setEvent();
		System.gc();
	}
	
	protected void finalize()
	{
		Release();
	}
	private void push(Type t)
	{
		if(m_popevent-m_pushevent == 1 || m_pushevent-m_popevent == 15) return;
		m_type[m_pushevent] = t;
		if(m_pushevent>=16)
			m_pushevent = 0;
		else
			m_pushevent++;
		m_type[m_pushevent] = t;
	}
	private Type pop()
	{
		if(m_popevent == m_pushevent)
			return Type.ENone;
		Type temp;
		temp = m_type[m_popevent];
		
		if(m_popevent>=16)
			m_popevent = 0;
		else
			m_popevent++;

		return temp;
	}
	
	private class Event
	{
		private   Semaphore		m_semaphore;
		
		public Event()
		{
			m_semaphore = new Semaphore(1);
			try{
			m_semaphore.acquire();
			}
	    	catch(InterruptedException e)
	    	{
	    		e.printStackTrace();
	    	}
			
		}
		public void setEvent()
		{
//			if(m_semaphore.tryAcquire()) return;
			m_semaphore.release();

		}
		public void resetEvent()
		{
			m_semaphore.tryAcquire();
		}
		public boolean isEvent(long time)
		{
			try{
			if(m_semaphore.tryAcquire(time,TimeUnit.MICROSECONDS))
			{
				m_semaphore.release();
				return true;
			}
			return false;
			}
	    	catch(InterruptedException e)
	    	{
	    		e.printStackTrace();
	    		return false;
	    	}
		}
	}
}
