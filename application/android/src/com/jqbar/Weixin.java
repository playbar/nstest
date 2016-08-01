package com.jqbar;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import com.tencent.mm.sdk.openapi.IWXAPI;
import com.tencent.mm.sdk.openapi.SendMessageToWX;
import com.tencent.mm.sdk.openapi.WXAPIFactory;
import com.tencent.mm.sdk.openapi.WXAppExtendObject;
import com.tencent.mm.sdk.openapi.WXEmojiObject;
import com.tencent.mm.sdk.openapi.WXFileObject;
import com.tencent.mm.sdk.openapi.WXImageObject;
import com.tencent.mm.sdk.openapi.WXMediaMessage;
import com.tencent.mm.sdk.openapi.WXMusicObject;
import com.tencent.mm.sdk.openapi.WXTextObject;
import com.tencent.mm.sdk.openapi.WXVideoObject;
import com.tencent.mm.sdk.openapi.WXWebpageObject;
import com.tencent.mm.sdk.platformtools.Util;
import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Environment;
import android.widget.EditText;
import android.widget.LinearLayout;

public class Weixin {
	String sdpath =Environment.getExternalStorageDirectory().getAbsolutePath()+"/byone/Local/";
	private Activity mActivity;
	private static final int THUMB_SIZE = 150;
	private static final String SDCARD_ROOT = Environment
			.getExternalStorageDirectory().getAbsolutePath();

	IWXAPI api = null;

	public Weixin(Activity act) {
		this.mActivity = act;
		initWx();
	}

	private void initWx() {
		// TODO Auto-generated method stub
		api = WXAPIFactory.createWXAPI(mActivity, GlobalFun.APP_ID, true);
		api.registerApp(GlobalFun.APP_ID);

	}
	private String buildTransaction(final String type) {
		return (type == null) ? String.valueOf(System.currentTimeMillis())
				: type + System.currentTimeMillis();
	}
	public static boolean saveMyBitmap(Bitmap bmp, String path) {
		File f = new File(path);
		try {
			f.createNewFile();
			FileOutputStream fOut = new FileOutputStream(f);
			bmp.compress(Bitmap.CompressFormat.PNG, 100, fOut);
			fOut.flush();
			fOut.close();
			return true;
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
		}
		return false;
	}
//	  String filePath =path+"pic2.png";
//	  //保存在本地png
//			  if (pdata1.length!=0){				
//			  saveMyBitmap(BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length), filePath
//			  );				 
//				  }
	public void sendText(String text,int type) {
		if (text == null || text.length() == 0) {
			return;
		}
		WXTextObject textObj = new WXTextObject();
		textObj.text = text;
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = textObj;
		msg.description = text;
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("text"); // transaction字段用于唯一标识一个请求
		req.message = msg;
		System.out.println("type "+type);
		if(type ==1){
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}else{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		
		api.sendReq(req);
	}

	public void sendPhoto ( byte[] pdata1,int pdata2,int type) {
	
		//发送图片到微信	  
		Bitmap bmp=BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length);
		    WXImageObject imgObj = new WXImageObject(bmp);		     
		    WXMediaMessage msg = new WXMediaMessage();
		    msg.mediaObject = imgObj;
		    msg.description="图片描述";		 
		    Bitmap thumbBmp = Bitmap.createScaledBitmap(bmp, THUMB_SIZE, THUMB_SIZE, true);
			bmp.recycle();
			msg.thumbData = Util.bmpToByteArray(thumbBmp, true); 
		    msg.title="abc-title";			
		    SendMessageToWX.Req req = new SendMessageToWX.Req();
		    req.transaction = buildTransaction("img");
		    req.message = msg;   
			if(type ==1){
				req.scene = SendMessageToWX.Req.WXSceneTimeline;
			}else{
				req.scene = SendMessageToWX.Req.WXSceneSession;
			}
		    api.sendReq(req);
		    
		    }
	public void sendLink ( String title, String description, byte[] pdata1,
			String weburl,int type){
		Bitmap thumb=null;
		WXWebpageObject webpage = new WXWebpageObject();
		webpage.webpageUrl = weburl;
		WXMediaMessage msg = new WXMediaMessage(webpage);
		msg.title = title;
		msg.description = description;
		if (pdata1.length!=0){
		 thumb=BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length);
		    Bitmap thumbBmp = Bitmap.createScaledBitmap(thumb, THUMB_SIZE, THUMB_SIZE, true);
			msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
		}
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("webpage");
		req.message = msg;
		if(type ==1){
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}else{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}
		api.sendReq(req);
		thumb.recycle();
	}

	public void sendMusic(String title, String description, byte[] pdata1,
			String musicurl, String musicdateUrl, int type) {
		Bitmap thumb=null;
		WXMusicObject music = new WXMusicObject();
		music.musicUrl=musicdateUrl;
		WXMediaMessage msg = new WXMediaMessage();
		msg.mediaObject = music;
		msg.title = title;
		msg.description = description;
		
		if (pdata1.length!=0){
			 thumb=BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length);
			 Bitmap thumbBmp = Bitmap.createScaledBitmap(thumb, THUMB_SIZE, THUMB_SIZE, true);
				msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
			}

		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("music");
		req.message = msg;
		if(type == 1){
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}else{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}		
		api.sendReq(req);
		thumb.recycle();
		
	}

	public void sendVideo(String title, String description, byte[] pdata1,
			String videourl, int type) {
		Bitmap thumb=null;
		WXVideoObject video = new WXVideoObject();
		video.videoUrl = videourl;
		WXMediaMessage msg = new WXMediaMessage(video);
		msg.title =title;
		msg.description = description;
		if (pdata1.length!=0){
			 thumb=BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length);
			 Bitmap thumbBmp = Bitmap.createScaledBitmap(thumb, THUMB_SIZE, THUMB_SIZE, true);
				msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
			}
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("video");
		req.message = msg;
		if(type ==1){
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}else{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}	
		api.sendReq(req);	
		thumb.recycle();
	}

	public void sendAppMessage(String title, String description, byte[] pdata1,
			String extInfo, String url,byte[] pdata2, int type) {

		final WXAppExtendObject appdata = new WXAppExtendObject();
	//	final String path = sdpath + "pic.png";
	//	appdata.fileData = Util.readFromFile(path, 0, -1);
		appdata.fileData = pdata2;
		appdata.extInfo = extInfo;
		
		final WXMediaMessage msg = new WXMediaMessage();
		//msg.setThumbImage(Util.extractThumbNail(path, 150, 150, true));
		Bitmap thumb = BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length);
	    Bitmap thumbBmp = Bitmap.createScaledBitmap(thumb, THUMB_SIZE, THUMB_SIZE, true);

		msg.setThumbImage(thumbBmp);
		msg.title = title;
		msg.description =description;
		msg.mediaObject = appdata;
		
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("appdata");
		req.message = msg;
		if(type ==1){
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}else{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}			
		api.sendReq(req);
		thumb.recycle();
	}

	public void sendNoGif(byte[] pdata1, byte[] pdata2, int type) {
		// TODO Auto-generated method stub
		WXEmojiObject emoji = new WXEmojiObject();	
		//gif转为bitmap在转为data
		emoji.emojiData  = pdata2;
		
		WXMediaMessage msg = new WXMediaMessage(emoji);
		
		Bitmap thumb = BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length);
	    Bitmap thumbBmp = Bitmap.createScaledBitmap(thumb, THUMB_SIZE, THUMB_SIZE, true);
		msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("emoji");
		req.message = msg;
		if(type ==1){
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}else{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}					
		api.sendReq(req);
		thumb.recycle();
	}

	public void sendFile(String title, String description, byte[] pdata1,
			byte[] pdata2, String fileExt, int type) {
		// TODO Auto-generated method stub
		WXFileObject file = new WXFileObject();		
		//file.fileData = Util.readFromFile(path, 0, (int) new File(path).length());
		file.fileData = pdata2;

		WXMediaMessage msg = new WXMediaMessage(file);
		
		msg.title = title;
		msg.description = description;
		Bitmap thumb = BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length);
	    Bitmap thumbBmp = Bitmap.createScaledBitmap(thumb, THUMB_SIZE, THUMB_SIZE, true);
		msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("file");
		req.message = msg;
		if(type ==1){
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}else{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}					
		api.sendReq(req);
		thumb.recycle();
	}

	public void sendGif(byte[] pdata1, byte[] pdata2, int type) {
		// TODO Auto-generated method stub
		WXEmojiObject emoji = new WXEmojiObject();	
		//gif转为bitmap在转为data
		emoji.emojiData  = pdata2;
		
		WXMediaMessage msg = new WXMediaMessage(emoji);
		
		Bitmap thumb = BitmapFactory.decodeByteArray(pdata1, 0, pdata1.length);
	    Bitmap thumbBmp = Bitmap.createScaledBitmap(thumb, THUMB_SIZE, THUMB_SIZE, true);
		msg.thumbData = Util.bmpToByteArray(thumbBmp, true);
		SendMessageToWX.Req req = new SendMessageToWX.Req();
		req.transaction = buildTransaction("emoji");
		req.message = msg;
		if(type ==1){
			req.scene = SendMessageToWX.Req.WXSceneTimeline;
		}else{
			req.scene = SendMessageToWX.Req.WXSceneSession;
		}					
		api.sendReq(req);
		thumb.recycle();
	}
}
