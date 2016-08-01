package com.jqbar;

import org.json.JSONException;
import org.json.JSONObject;
import com.jqbar.MobileMain;
import com.ibm.mqtt.IMqttClient;
import com.ibm.mqtt.MqttClient;
import com.ibm.mqtt.MqttException;
import com.ibm.mqtt.MqttPersistence;
import com.ibm.mqtt.MqttPersistenceException;
import com.ibm.mqtt.MqttSimpleCallback;
import com.jqbar.R;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

public class BwPushService extends Service
{
	private static final String		mHostName = "proxy.jqbar.com";
	private static int				mPortNum = 2583;
	//private static int				mPortNum = 2583;
	public static String			mClientID = GlobalFun.BWPRODUCTNAME;
	
	//private static final String		mHostName ="192.168.1.122";
	//private static int				mPortNum = 3883;
	
	// Let's not use the MQTT persistence.
	private static MqttPersistence	MQTT_PERSISTENCE          = null;
	// Let's set the internal keep alive for MQTT to 15 mins. I haven't tested this value much. It could probably be increased.
	private static short			MQTT_KEEP_ALIVE           = 60 * 15;
	// Set quality of services to 0 (at most once delivery), since we don't want push notifications 
	// arrive more than once. However, this means that some messages might get lost (delivery is not guaranteed)
	private static int[]			MQTT_QUALITIES_OF_SERVICE = { 0 } ;
	private static int				MQTT_QUALITY_OF_SERVICE   = 0;
	// The broker should not retain any messages.
	private static boolean			MQTT_RETAINED_PUBLISH     = false;
		
	// MQTT client ID, which is given the broker. In this example, I also use this for the topic header. 
	// You can use this to run push notifications for multiple apps with one MQTT broker. 

	// These are the actions for the service (name are descriptive enough)
	public static final String		ACTION_START = mClientID + ".START";
	private static final String		ACTION_STOP = mClientID + ".STOP";
	private static final String		ACTION_KEEPALIVE = mClientID + ".KEEP_ALIVE";
	private static final String		ACTION_RECONNECT = mClientID + ".RECONNECT";
	
	// Connectivity manager to determining, when the phone loses connection
	private ConnectivityManager		mConnMan;
	// Notification manager to displaying arrived push notifications 
	private NotificationManager		mNotifMan;

	// Whether or not the service has been started.	
	private boolean 				mStarted;

	// This the application level keep-alive interval, that is used by the AlarmManager
	// to keep the connection active, even when the device goes to sleep.
	private static final long		KEEP_ALIVE_INTERVAL = 1000 * 60 * 28;

	// Retry intervals, when the connection is lost.
	private static final long		INITIAL_RETRY_INTERVAL = 1000 * 10;
	private static final long		MAXIMUM_RETRY_INTERVAL = 1000 * 60 * 30;

	private String mPushCfg = null;
	
	// This is the instance of an MQTT connection.
	private MQTTConnection			mConnection;
	private long					mStartTime;
	private PushBroadcastReceiver mBroadReceiverChanged = new PushBroadcastReceiver();

	// Static method to start the service
	public static void actionStart(Context ctx) 
	{
		Intent i = new Intent(ctx, BwPushService.class);
		i.setAction(ACTION_START);
		ctx.startService(i);
	}

	// Static method to stop the service
	public static void actionStop(Context ctx)
	{
		Intent i = new Intent(ctx, BwPushService.class);
		i.setAction(ACTION_STOP);
		ctx.startService(i);
	}
	
	// Static method to send a keep alive message
	public static void actionPing(Context ctx) 
	{
		Intent i = new Intent(ctx, BwPushService.class);
		i.setAction(ACTION_KEEPALIVE);
		ctx.startService(i);
	}

	@Override
	public void onCreate()
	{
		super.onCreate();
		mStartTime = System.currentTimeMillis();
		
		// Get instances of preferences, connectivity manager and notification manager
		//mPrefs = getSharedPreferences(GlobalFun.BW_CONFIGURE_FILE, MODE_PRIVATE);
		mPushCfg = GlobalFun.readPushCfg( this );
		mConnMan = (ConnectivityManager)getSystemService(CONNECTIVITY_SERVICE);
		mNotifMan = (NotificationManager)getSystemService(NOTIFICATION_SERVICE);
	
		/* If our process was reaped by the system for any reason we need
		 * to restore our state with merely a call to onCreate.  We record
		 * the last "started" value and restore it here if necessary. */
		handleCrashedService();
	}
	
	// This method does any necessary clean-up need in case the server has been destroyed by the system
	// and then restarted
	private void handleCrashedService()
	{
		if (wasStarted() == true) 
		{
			 // stop the keep alives
			stopKeepAlives(); 		
			// Do a clean start
			start();
		}
	}
	
	@Override
	public void onDestroy()
	{
		// Stop the services, if it has been started
		if (mStarted == true)
		{
			stop();
		}
	}
	
	@Override
	public void onStart(Intent intent, int startId)
	{
		if( intent == null )
		{
			return;
		}
		super.onStart(intent, startId);

		// Do an appropriate action based on the intent.
		if (intent.getAction().equals(ACTION_STOP) == true) 
		{
			stop();
			stopSelf();
		} else if (intent.getAction().equals(ACTION_START) == true) 
		{
			start();
		}
		else if (intent.getAction().equals(ACTION_KEEPALIVE) == true)
		{
			keepAlive();
		} 
		else if (intent.getAction().equals(ACTION_RECONNECT) == true)
		{
			if (isNetworkAvailable()) 
			{
				reconnectIfNecessary();
			}
		}
	}
	
	@Override
	public IBinder onBind(Intent intent)
	{
		return null;
	}
	
	// Reads whether or not the service has been started from the preferences
	private boolean wasStarted()
	{
		//return mPrefs.getBoolean(PREF_STARTED, false);
		boolean bstart = false;
		JSONObject jb = null;
		try 
		{
			jb = new JSONObject(mPushCfg);
			bstart = jb.getBoolean("bStart");
		} 
		catch (JSONException e)
		{
			//e.printStackTrace(System.err);
		}
		return bstart;
	}

	// Sets whether or not the services has been started in the preferences.
	private void setStarted(boolean started)
	{
		//mPrefs.edit().putBoolean(PREF_STARTED, started).commit();		
		JSONObject jb = null;
		try 
		{
			jb = new JSONObject(mPushCfg);
			jb.put("bStart", started);
			GlobalFun.writePushCfg(this,jb.toString() );
			mPushCfg = jb.toString();
		} 
		catch (JSONException e)
		{
			//e.printStackTrace(System.err);
		}
		mStarted = started;
	}

	private void start()
	{
		// Do nothing, if the service is already running.
		if (mStarted == true)
		{
			return;
		}
		
		// Establish an MQTT connection
		connect();
		// Register a connectivity listener
		registerReceiver(mBroadReceiverChanged, new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION));		
	}

	private void stop()
	{
		// Do nothing, if the service is not running.
		if (mStarted == false)
		{
			return;
		}

		// Save stopped state in the preferences
		setStarted(false);

		// Remove the connectivity receiver
		unregisterReceiver(mBroadReceiverChanged);
		// Any existing reconnect timers should be removed, since we explicitly stopping the service.
		cancelReconnect();

		// Destroy the MQTT connection if there is one
		if (mConnection != null) 
		{
			mConnection.disconnect();
			mConnection = null;
		}
	}
	
	// 
	private void connect()
	{		
//		// fetch the device ID from the preferences.
//		//String deviceID = "b779a48ab47726d";
//		String deviceID = null;// mPrefs.getString(PREF_DEVICE_ID, null);
//		JSONObject jb = null;
//		try 
//		{
//			jb = new JSONObject(mPushCfg);
//			deviceID = jb.getString("deviceID");
//		} 
//		catch (JSONException e)
//		{
//			//e.printStackTrace(System.err);
//		}
//		// Create a new connection only if the device id is not NULL
//		//Log.e("BwPushService connect", deviceID );
//		if (deviceID != null)
//		{
//			try
//			{
//				//Log.e("BwPushService connect", mHostName );
//				mConnection = new MQTTConnection(mHostName, deviceID);
//				setStarted(true);
//			} 
//			catch (MqttException e) 
//			{
//				setStarted(false);
//				// Schedule a reconnect, if we failed to connect
////	        	if (isNetworkAvailable())
////	        	{
////	        		scheduleReconnect(mStartTime);
////	        	}
//			}
//			
//		}
		
		// zhengjl 2012.06
		Thread connectThread = new Thread( new ConnectThread() );
		connectThread.start();
		
	}
	
	// 使用子线程，解决MQTTConnection卡死问题。	// zhengjl 2012.06
    class ConnectThread implements Runnable
    {
    	public void run()
    	{
    		String deviceID = null;// mPrefs.getString(PREF_DEVICE_ID, null);
    		JSONObject jb = null;
    		try 
    		{
    			jb = new JSONObject(mPushCfg);
    			deviceID = jb.getString("deviceID");
    		} 
    		catch (JSONException e)
    		{
    		}

    		if (deviceID != null)
    		{
    			try
    			{
    				mConnection = new MQTTConnection(mHostName, deviceID);
    				setStarted(true);
        			// Register a connectivity listener
        			//registerReceiver(mBroadReceiverChanged, new IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION));		

    			} 
    			catch (MqttException e) 
    			{
    				Log.e("MQTTConnection","MqttException");
    				setStarted(false);
    			}
    		} 
    	}
    }


	private void keepAlive()
	{
		try 
		{
			// Send a keep alive, if there is a connection.
			if (mStarted == true && mConnection != null)
			{
				mConnection.sendKeepAlive();
			}
		}
		catch (MqttException e)
		{
			
			mConnection.disconnect();
			mConnection = null;
			cancelReconnect();
		}
	}

	// Schedule application level keep-alives using the AlarmManager
	private void startKeepAlives() 
	{
		Intent i = new Intent();
		i.setClass(this, BwPushService.class);
		i.setAction(ACTION_KEEPALIVE);
		PendingIntent pi = PendingIntent.getService(this, 0, i, 0);
		AlarmManager alarmMgr = (AlarmManager)getSystemService(ALARM_SERVICE);
		alarmMgr.setRepeating(AlarmManager.RTC_WAKEUP,
		  System.currentTimeMillis() + KEEP_ALIVE_INTERVAL,
		  KEEP_ALIVE_INTERVAL, pi);
	}

	// Remove all scheduled keep alives
	private void stopKeepAlives()
	{
		Intent i = new Intent();
		i.setClass(this, BwPushService.class);
		i.setAction(ACTION_KEEPALIVE);
		PendingIntent pi = PendingIntent.getService(this, 0, i, 0);
		AlarmManager alarmMgr = (AlarmManager)getSystemService(ALARM_SERVICE);
		alarmMgr.cancel(pi);
	}

	// We schedule a reconnect based on the starttime of the service
	public void scheduleReconnect(long startTime)
	{
		// the last keep-alive interval
		long interval = 0;
		JSONObject jb = null;
		try 
		{
			jb = new JSONObject(mPushCfg);
			interval = jb.getLong("retry");
		} 
		catch (JSONException e)
		{
			//e.printStackTrace(System.err);
		}
		
		//long interval = mPrefs.getLong(PREF_RETRY, INITIAL_RETRY_INTERVAL);
		
		// Calculate the elapsed time since the start
		long now = System.currentTimeMillis();
		long elapsed = now - startTime;


		// Set an appropriate interval based on the elapsed time since start 
		if (elapsed < interval)
		{
			interval = Math.min(interval * 4, MAXIMUM_RETRY_INTERVAL);
		} 
		else
		{
			interval = INITIAL_RETRY_INTERVAL;
		}
	
		try
		{
			jb.put("retry", interval);
			GlobalFun.writePushCfg( this, jb.toString() );
			mPushCfg = jb.toString();
		}
		catch (JSONException e)
		{
			//e.printStackTrace(System.err);
		}

		// Save the new internval
		//mPrefs.edit().putLong(PREF_RETRY, interval).commit();

		// Schedule a reconnect using the alarm manager.
		Intent i = new Intent();
		i.setClass(this, BwPushService.class);
		i.setAction(ACTION_RECONNECT);
		PendingIntent pi = PendingIntent.getService(this, 0, i, 0);
		AlarmManager alarmMgr = (AlarmManager)getSystemService(ALARM_SERVICE);
		alarmMgr.set(AlarmManager.RTC_WAKEUP, now + interval, pi);
	}
	
	// Remove the scheduled reconnect
	public void cancelReconnect()
	{
		Intent i = new Intent();
		i.setClass(this, BwPushService.class);
		i.setAction(ACTION_RECONNECT);
		PendingIntent pi = PendingIntent.getService(this, 0, i, 0);
		AlarmManager alarmMgr = (AlarmManager)getSystemService(ALARM_SERVICE);
		alarmMgr.cancel(pi);
	}
	
	private void reconnectIfNecessary()
	{		
		if (mStarted == true && mConnection == null)
		{
			connect();
		}
	}

	// This receiver listeners for network changes and updates the MQTT connection
	// accordingly
	
	// Display the topbar notification
	private void showNotification(String strAlert, String strUrl, int type ) 
	{
		Notification n = new Notification();
		n.flags |= Notification.FLAG_SHOW_LIGHTS;
      	n.flags |= Notification.FLAG_AUTO_CANCEL;
        n.defaults = Notification.DEFAULT_ALL;
		n.icon = R.drawable.logo_36;
		n.when = System.currentTimeMillis();

		// Simply open the parent activity
		//type = 2;
		//strUrl = "http://www.baidu.com";
		PendingIntent pi = null;
    	if( type == 1 )
    	{
    		Intent intent = new Intent(this, MobileMain.class);
        	Bundle bl = new Bundle();
        	bl.putString("BW_URL", strUrl );
        	intent.putExtras(bl );
			pi = PendingIntent.getActivity(this, 0, intent, 0);
	    }
    	else if( type == 2 || type == 3 )
    	{
    		//Intent intent = new Intent(this, BwWebView.class);
        	//Bundle bl = new Bundle();
        	//bl.putString("BW_URL", strUrl );
        	//intent.putExtras(bl );
    		 Intent intent = new Intent("android.intent.action.VIEW", Uri.parse( strUrl ));
             //startActivity(viewIntent);
        	 pi = PendingIntent.getActivity(this, 0, intent, 0);
    	}
    	else
    	{
    		Intent intent = new Intent(this, MobileMain.class);
    		pi = PendingIntent.getActivity(this, 0, intent, 0);
    	}

		// Change the name of the notification here
		n.setLatestEventInfo(this, "百玩", strAlert, pi);

		mNotifMan.notify( 0, n);
	}
	
	// Check if we are online
	private boolean isNetworkAvailable()
	{
		NetworkInfo info = mConnMan.getActiveNetworkInfo();
		if (info == null) {
			return false;
		}
		return info.isConnected();
	}
	
	public class PushBroadcastReceiver extends BroadcastReceiver 
	{
		@Override
		public void onReceive(Context context, Intent intent) 
		{
			if( intent != null && intent.getAction().equals("android.intent.action.BOOT_COMPLETED"))
			{
				BwPushService.actionStart(getApplicationContext());	
			}
			else
			{
				// Get network info
				NetworkInfo info = (NetworkInfo)intent.getParcelableExtra (ConnectivityManager.EXTRA_NETWORK_INFO);
				
				// Is there connectivity?
				boolean hasConnectivity = (info != null && info.isConnected()) ? true : false;
	
				if (hasConnectivity) 
				{
					reconnectIfNecessary();
				} 
				else if (mConnection != null)
				{
					// if there no connectivity, make sure MQTT connection is destroyed
					mConnection.disconnect();
					cancelReconnect();
					mConnection = null;
				}
			}
			return;
		}	// end of onReceive
	};
	
	// This inner class is a wrapper on top of MQTT client.
	private class MQTTConnection implements MqttSimpleCallback
	{
		IMqttClient mqttClient = null;
		
		// Creates a new connection given the broker address and initial topic
		public MQTTConnection(String brokerHostName, String deviceID) throws MqttException
		{
			Log.e("PushService", "MQTTConnection begin" );
	    	String mqttConnSpec = "tcp://" + brokerHostName + "@" + mPortNum;
			mqttClient = MqttClient.createMqttClient(mqttConnSpec, MQTT_PERSISTENCE);
			String strTopic = mClientID + "/"+ deviceID;
			mqttClient.connect(strTopic, false, MQTT_KEEP_ALIVE);
			mqttClient.registerSimpleHandler(this);
			subscribeToTopic(strTopic);
			mStartTime = System.currentTimeMillis();
			startKeepAlives();	
			Log.e("PushService", "MQTTConnection end" );
		}
		
		// Disconnect
		public void disconnect()
		{
			try
			{			
				stopKeepAlives();
				mqttClient.disconnect();
			} 
			catch (MqttPersistenceException e)
			{
				
			}
		}
		/*
		 * Send a request to the message broker to be sent messages published with 
		 *  the specified topic name. Wildcards are allowed.	
		 */
		private void subscribeToTopic(String topicName) throws MqttException
		{
			if ((mqttClient == null) || (mqttClient.isConnected() == false))
			{
				// quick sanity check - don't try and subscribe if we don't have
				//  a connection
			} 
			else
			{									
				String[] topics = { topicName };
				mqttClient.subscribe(topics, MQTT_QUALITIES_OF_SERVICE);
			}
		}	
		/*
		 * Sends a message to the message broker, requesting that it be published
		 *  to the specified topic.
		 */
		private void publishToTopic(String topicName, String message) throws MqttException
		{		
			if ((mqttClient == null) || (mqttClient.isConnected() == false)) 
			{
				// quick sanity check - don't try and publish if we don't have
				//  a connection				
			} 
			else 
			{
				mqttClient.publish(topicName, 
								   message.getBytes(),
								   MQTT_QUALITY_OF_SERVICE, 
								   MQTT_RETAINED_PUBLISH);
			}
		}		
		
		/*
		 * Called if the application loses it's connection to the message broker.
		 */
		public void connectionLost() throws Exception
		{
			stopKeepAlives();
			// null itself
			mConnection = null;
			if (isNetworkAvailable() == true) 
			{
				reconnectIfNecessary();	
			}
		}		
		
		/*
		 * Called when we receive a message from the message broker. 
		 */
		public void publishArrived(String topicName, byte[] payload, int qos, boolean retained)
		{
			// Show a notification
			String strJson = new String(payload);
		   JSONObject jb =null;
		   try 
		   {
			   jb = new JSONObject(strJson);
			   JSONObject jbAps = jb.getJSONObject("aps");
			   String strAlert = jbAps.getString("alert");
			   JSONObject jbBw = jb.getJSONObject("bw");
			   String strUrl = jbBw.getString("url");
			   int iurltype = jbBw.getInt("urltype");
			   showNotification(strAlert, strUrl, iurltype );
		   } 
		   catch (JSONException e)
		   {
	           //e.printStackTrace(System.err);
	       }
		}   
		
		public void sendKeepAlive() throws MqttException
		{
			// publish to a keep-alive topic
			String deviceID = null;
			JSONObject jb = null;
			try 
			{
				jb = new JSONObject(mPushCfg);
				deviceID = jb.getString("deviceID");
			} 
			catch (JSONException e)
			{
				//e.printStackTrace(System.err);
			}
			publishToTopic(mClientID + "/keepalive", deviceID );
		}		
	} // end of class MQTTConnection
	
}