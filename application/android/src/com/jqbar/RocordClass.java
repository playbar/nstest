package com.jqbar;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import android.app.Activity;
import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Toast;

//音频视频
public class RocordClass  {
	 private String strImgPath = "";//照片文件绝对路径
	   private String strVideoPath = "";//视频文件绝对路径
	   private String strRecorderPath = "";//录音文件绝对路径
	   private static final int RESULT_CAPTURE_IMAGE = 1;//照相的requestCode
	   private static final int REQUEST_CODE_TAKE_VIDEO = 2;//摄像的
	   private static final int RESULT_CAPTURE_RECORDER_SOUND = 3;//录音的
	   Activity mContext;
	   
	   public RocordClass(Activity context) {
		// TODO Auto-generated constructor stub
		   this.mContext=context;
	}
	  //照相功能
	public void cameraMethod(){
		  Intent imageCaptureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		  strImgPath = Environment.getExternalStorageDirectory().toString()+"/CONSDCGMPIC";//存放照片的文件夹
		  String fileName = new SimpleDateFormat("yyyyMMddHHmmss").format(new Date())+".jpg";
		  File out = new File(strImgPath);
		  if(!out.exists()){
			  out.mkdirs();
		  }
		  out = new File(strImgPath,fileName);
		  strImgPath = strImgPath + fileName;//该照片的绝对路径
		  Uri uri = Uri.fromFile(out);
		  imageCaptureIntent.putExtra(MediaStore.EXTRA_OUTPUT, uri);
		  imageCaptureIntent.putExtra(MediaStore.EXTRA_VIDEO_QUALITY, 1);
		  mContext.startActivityForResult(imageCaptureIntent,RESULT_CAPTURE_IMAGE);
	  }
	  
	  //拍摄视频
	  public void videoMethod(){
		  Intent intent = new Intent(MediaStore.ACTION_VIDEO_CAPTURE);
		  intent.putExtra(MediaStore.EXTRA_VIDEO_QUALITY, 0);
		  mContext.startActivityForResult(intent,REQUEST_CODE_TAKE_VIDEO);
	  }
	  
	  //录音功能
	  public void soundRecorderMethod(){
		  Intent intent = new Intent(Intent.ACTION_GET_CONTENT);
		  intent.setType("audio/amr");
		  mContext.startActivityForResult(intent,RESULT_CAPTURE_RECORDER_SOUND);
	  }
	
}