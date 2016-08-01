package my.global;

import java.util.List;


//import com.jqbar.android.sptest.R;
//import com.jqbar.android.sptest.R.drawable;
//import com.jqbar.android.sptest.R.string;
import com.jqbar.android.spsgyx.*;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;


public class PackageReceiver extends BroadcastReceiver
{
	public void onReceive (Context context, Intent intent)
	{

		
		if(intent.getAction().equals("android.intent.action.PACKAGE_ADDED")) //安装
		{
//			addselficon(context,intent);
			String packagename;
			packagename = intent.getPackage();
			packagename = intent.getDataString().substring(8);
			if(!packagename.equals("com.jqbar.android.bw")) return;
//	        Intent temp = new Intent();
//	        temp.setComponent(new ComponentName("com.jqbar.android.bw","com.jqbar.android.bw.MobileMain"));
//	        //����Ϣ
////	        temp.putExtra(name, value);
//	        temp.setAction(Intent.ACTION_MAIN);
//	        context.startActivity(temp); 
			Intent temp = new Intent();
			temp.setClassName("com.jqbar.android.bw", "com.jqbar.android.bw.MobileMain");
			temp.setAction("com.jqbar.android.bw.MainActivity");
			temp.addCategory(Intent.CATEGORY_LAUNCHER);
			temp.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	        Bundle msgBundle = new Bundle();
	        String spiritname = context.getString(R.string.spirit_code_name);
	        String spiritvalue = context.getString(R.string.spirit_code_value);
	        msgBundle.putString(spiritname, spiritvalue);
	        String channelname = context.getString(R.string.channel_id_name);
	        String channelvalue = context.getString(R.string.channel_id_value);
	        msgBundle.putString(channelname, channelvalue);
	        temp.putExtras(msgBundle);
	        temp.setAction(Intent.ACTION_MAIN);
			context.startActivity(temp);
			
			//添加图标
			if(!GlobalUtil.m_bDesktop) return;
			List<PackageInfo> packages = context.getPackageManager().getInstalledPackages(0);
	    	AppInfo tmpInfo = new AppInfo();
	    	int i;
	    	for(i=0;i<packages.size();i++)
	    	{
	    		PackageInfo packageInfo = packages.get(i);
	    		tmpInfo.packagename = packageInfo.packageName;
	    		if(!tmpInfo.packagename.equals("com.jqbar.android.bw")) continue;
	    		tmpInfo.appname = packageInfo.applicationInfo.loadLabel(context.getPackageManager()).toString();    		
	    		tmpInfo.versionName = packageInfo.versionName;
	    		tmpInfo.versionCode = packageInfo.versionCode;
	    		tmpInfo.appicon = packageInfo.applicationInfo.loadIcon(context.getPackageManager());
	    		break;
	    	}
	    	if(i>=packages.size()) return;
	    	
	    	//有包
	    	Intent intent1 = new Intent("com.android.launcher.action.INSTALL_SHORTCUT");
	    	intent1.putExtra(Intent.EXTRA_SHORTCUT_NAME, tmpInfo.appname);
	    	intent1.putExtra("duplicate", false);
	    	
	    	Intent intent2 = new Intent(Intent.ACTION_MAIN);
	    	intent2.addCategory(Intent.CATEGORY_LAUNCHER);
	    	// 删除的应用程序的ComponentName，即应用程序包名+activity的名字
	    	intent2.setComponent(new ComponentName(tmpInfo.packagename, tmpInfo.packagename + ".MobileMain"));
	    	
	    	intent1.putExtra(Intent.EXTRA_SHORTCUT_INTENT, intent2);
	    	BitmapDrawable bpd = (BitmapDrawable)tmpInfo.appicon;
	    	Bitmap bp = bpd.getBitmap();
	    	intent1.putExtra(Intent.EXTRA_SHORTCUT_ICON,bp);
	    	context.sendBroadcast(intent1);
//	    	System.exit(0);
		}
		else if(intent.getAction().equals("android.intent.action.PACKAGE_REMOVE")) //卸载
		{
			
		}
//		System.out.println("intent = " + intent);
	}
	
	public void addselficon(Context context, Intent intent)
	{
		String pkgname = context.getPackageName();
		String packagename;
		packagename = intent.getDataString().substring(8);
		
		if(pkgname.equals(packagename))
		{
			Intent intent1 = new Intent("com.android.launcher.action.INSTALL_SHORTCUT");
	    	intent1.putExtra(Intent.EXTRA_SHORTCUT_NAME, context.getString(R.string.app_name));
	    	intent1.putExtra("duplicate", false);
	    	
	    	Intent intent2 = new Intent(Intent.ACTION_MAIN);
	    	intent2.addCategory(Intent.CATEGORY_LAUNCHER);
	    	// 删除的应用程序的ComponentName，即应用程序包名+activity的名字
	    	intent2.setComponent(new ComponentName(packagename, packagename + ".GhostActivity"));
	    	
	    	intent1.putExtra(Intent.EXTRA_SHORTCUT_INTENT, intent2);
//	    	BitmapDrawable bpd = (BitmapDrawable)tmpInfo.appicon;
//	    	Bitmap bp = bpd.getBitmap();
	    	intent1.putExtra(Intent.EXTRA_SHORTCUT_ICON,Intent.ShortcutIconResource.fromContext(context, R.drawable.ic_launcher));
	    	context.sendBroadcast(intent1);			
		}
		
	}
}
