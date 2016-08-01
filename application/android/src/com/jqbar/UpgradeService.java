package com.jqbar;

import org.apache.http.HttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import com.jqbar.R;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Binder;
import android.os.Bundle;
import android.os.ConditionVariable;
import android.os.Environment;
import android.os.IBinder;
import android.os.Parcel;
import android.os.RemoteException;
import android.util.Log;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

/**
 * This is an example of service that will update its status bar balloon
 * every 5 seconds for a minute.
 *
 */
public class UpgradeService extends Service
{
    private NotificationManager mNotificationManager;

    // Use a layout id for a unique identifier
    private static int MOOD_NOTIFICATIONS = 3923;
    // variable which controls the notification thread
    //private ConditionVariable mCondition;

    private String mUpgradeUrl = null;
    private String mStrDownloadPath = null;

    
    public void onCreate()
    {
        mNotificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

        File file = null;
        file = new File( GlobalFun.BWWORKPATH + "/tmp.file");
        try
        {
            file.createNewFile();
            FileInputStream fin = new FileInputStream( file );
            byte buf[] = new byte[512];
            int ilen = fin.read(buf);
            mUpgradeUrl = new String( buf, buf.length );
            mUpgradeUrl = mUpgradeUrl.substring(0, ilen );
            fin.close();
        }
        catch (IOException e)
        {
            mUpgradeUrl = "http://m.jqbar.com";
            Log.e("tag", e.getMessage());
        }
        Thread notifyingThread = new Thread(null, mTask, "NotifyingService");
        //mCondition = new ConditionVariable(false);
        notifyingThread.start();
    }

    
    public void onDestroy()
    {
        super.onDestroy();
        mNotificationManager.cancel(MOOD_NOTIFICATIONS);
        System.exit(0);
    }

    private Runnable mTask = new Runnable()
    {
        public void run()
        {
            showNotification( "开始下载...");
            downApkFile( mUpgradeUrl );
            // Done with our work...  stop the service!
            installApk(mStrDownloadPath + "/baiwan.apk");
            UpgradeService.this.stopSelf();
            System.exit(0);

        }
    };

    public void downApkFile(String strUrl)
    {
      mStrDownloadPath = Environment.getExternalStorageDirectory().getPath()+ "/bwdown";
      File file = new File(mStrDownloadPath);
      if (!file.exists())
      {
          file.mkdir();
      }
      HttpGet httpGet = new HttpGet(strUrl);
      try
      {
          HttpResponse httpResponse = new DefaultHttpClient().execute(httpGet);
          if (httpResponse.getStatusLine().getStatusCode() == 200)
          {
              InputStream is = httpResponse.getEntity().getContent();
              long tolLen = httpResponse.getEntity().getContentLength();
              FileOutputStream fos = new FileOutputStream(mStrDownloadPath + "/baiwan.apk");
              byte[] buffer = new byte[8192];
              int count = 0;
              int tolcount = 0;
              while ((count = is.read(buffer)) != -1)
              {
                  fos.write(buffer, 0, count);
                  tolcount += count;
                  showNotification( "下载进度:" + tolcount / 1024 + "kb/" + tolLen / 1024 +"kb" );
                  //showNotification(R.drawable.logo_36, R.string.upgrade_downing);
              }
              fos.close();
              is.close();

              showNotification( "下载完成，存放sdcard/bwdown目录下" );

          }
      }
      catch (Exception e)
      {
      }
    }

    private void installApk(String filename)
    {
        File file = new File(filename);
        Intent intent = new Intent();
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setAction(Intent.ACTION_VIEW);
        String type = "application/vnd.android.package-archive";
        intent.setDataAndType(Uri.fromFile(file), type);
        startActivity(intent);
    }

    
    public IBinder onBind(Intent intent)
    {
        return mBinder;
    }

    private void showNotification(String strMsg )
    {
      //创建一个NotificationManager的引用

        String ns = Context.NOTIFICATION_SERVICE;

        NotificationManager mNotificationManager = (NotificationManager)getSystemService(ns);

        //定义Notification的各种属性
        int icon = R.drawable.logo_36; //通知图标
        CharSequence tickerText = "正在更新，请稍候..."; //状态栏显示的通知文本提示
        long when = System.currentTimeMillis(); //通知产生的时间，会在通知信息里显示
        Notification notification = new Notification(icon,tickerText,when);

        Context context = getApplicationContext(); //上下文
        CharSequence contentTitle = "正在更新，请稍候..."; //通知栏标题
        CharSequence contentText = strMsg; //通知栏内容
        Intent notificationIntent = new Intent(this,AboutActivity.class); //点击该通知后要跳转的Activity
        PendingIntent contentIntent = PendingIntent.getActivity(this,0,notificationIntent,0);

        notification.setLatestEventInfo(context, contentTitle, contentText, contentIntent);

        //把Notification传递给NotificationManager

        mNotificationManager.notify(0,notification);
    }

    // This is the object that receives interactions from clients.  See
    // RemoteService for a more complete example.
    private final IBinder mBinder = new Binder()
    {
        protected boolean onTransact(int code, Parcel data, Parcel reply,
                int flags) throws RemoteException
        {
            return super.onTransact(code, data, reply, flags);
        }
    };

}
