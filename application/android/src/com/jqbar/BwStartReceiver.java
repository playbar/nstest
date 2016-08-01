package com.jqbar;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class BwStartReceiver extends BroadcastReceiver 
{ 
    public void onReceive(Context context, Intent intent) 
    {
    	if( intent.getAction().equals("android.intent.action.BOOT_COMPLETED") )
    	{
    		BwPushService.actionStart( context );	
    	}
    	return;
    }

} 