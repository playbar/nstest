package com.jqbar;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import com.jqbar.R;
import com.jqbar.MobileMain;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.content.res.AssetManager;
import android.os.Parcelable;
import android.util.Log;

public class GlobalFun {
//	public static int BWScreenWidth = 0;// 屛幕宽度
//	public static int BWScreenHeight = 0;// 屛幕高度
	public static int BWStartPoint = 0;// 帮助虛线起始位置
	public static boolean showMenu = true;
	public static boolean showFloatMenu = true;// modify 11.13
	public static final String BWPRODUCTNAME = "BW";
	public static final String BWWORKPATH = "/sdcard/byone"; // 程序工作目录

	public static final String BW_CONFIGURE_FILE = "BWConfigureFile"; // 保存配置文件信息
	// APP_ID必须为提交审核通过的id
	public static final String APP_ID = "wx11d867e281866d33";

	public static void setOrientation(Activity context)
	{
		if (MobileMain.mCurOrientation == ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE) 
		{
			context.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
		}
		else
		{
			context.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
		}
	}

	/*
	 * 复制assets目录中fonts.fnt文件到/sdcard/byone/目录下
	 */
	public static boolean GFunCopyFontFile(Context act) {
		File filedir = new File(BWWORKPATH + "/");
		if (!filedir.exists()) {
			filedir.mkdirs();
		}
		CopyFontFile(act, "fonts.fnt");
		CopyFontFile(act, "hz8.dot");
		CopyFontFile(act, "hz12.dot");
		CopyFontFile(act, "hz16.dot");
		filedir = null;
		System.gc();
		return true;
	}

	static boolean CopyFontFile(Context act, String fileName) {
		InputStream inputStream = null;
		File file = null;
		file = new File(BWWORKPATH + "/" + fileName);
		if (file.exists())
			return true;
		try {
			file.createNewFile();
			AssetManager assetManager = act.getAssets();
			inputStream = assetManager.open(fileName);
			int ilen = inputStream.available();
			byte buf[] = new byte[ilen];
			inputStream.read(buf);
			inputStream.close();
			FileOutputStream fout = new FileOutputStream(file);
			fout.write(buf, 0, ilen);
			fout.flush();
			fout.close();
			buf = null;
		} catch (IOException e) {
			Log.e("tag", e.getMessage());
		}
		file = null;
		return true;
	}

	static boolean createShortCut(Context cont) {
		Intent shortcutIntent = new Intent(
				"com.android.launcher.action.INSTALL_SHORTCUT");
		Parcelable icon = Intent.ShortcutIconResource.fromContext(cont,
				R.drawable.byone_logo); // 获取快捷键的图标
		Intent myIntent = new Intent(cont, MobileMain.class);
		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_NAME, "百玩");// 快捷方式的标题
		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, icon);// 快捷方式的图标
		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, myIntent);// 快捷方式的动作
		// shortcutIntent(Intent.EXTRA_SHORTCUT_DUPLICATE, false);
		cont.sendBroadcast(shortcutIntent);// 发送广播
		return true;
	}

	public static String GFunGetCID(Context act) {
		String strCID = null;
		try {
			AssetManager assetManager = act.getAssets();
			InputStream inputStream = assetManager.open("cid");
			;
			byte buf[] = new byte[9];
			int ilen = inputStream.read(buf);
			strCID = new String(buf, "UTF-8");
			strCID = strCID.substring(0, 8);
			inputStream.close();
		} catch (IOException e) {
			strCID = "86660268";
			Log.e("tag", e.getMessage());
		}
		return strCID;
	}

	static void WriteFile(String str) {
		try {
			File file = null;
			file = new File(BWWORKPATH + "/tmp.file");
			file.createNewFile();
			FileOutputStream fout = new FileOutputStream(file);
			byte buf[] = str.getBytes();
			fout.write(buf, 0, buf.length);
			fout.close();
		} catch (IOException e) {
			Log.e("tag", e.getMessage());
		}
	}

	// 根据机型判断Search按键是否存在
	static boolean checkExistSearch() {
		String strModle = android.os.Build.MODEL;
		if (strModle.equals("GT-I9000"))
			return false;
		else if (strModle.equals("MI-ONE Plus"))
			return false;
		else
			return true;
	}

	/**
	 * 保存文件
	 * 
	 * @param fileName
	 *            文件名
	 * @param content
	 *            文件内容
	 * @throws Exception
	 */
	public static void writePushCfg(Context act, String strcontent) {
		// Context.MODE_PRIVATE：为默认操作模式，代表该文件是私有数据，只能被应用本身访问，在该模式下，写入的内容会覆盖原文件的内容，如果想把新写入的内容追加到原文件中。可以使用Context.MODE_APPEND
		// Context.MODE_APPEND：模式会检查文件是否存在，存在就往文件追加内容，否则就创建新文件。
		// Context.MODE_WORLD_READABLE和Context.MODE_WORLD_WRITEABLE用来控制其他应用是否有权限读写该文件。
		// MODE_WORLD_READABLE：表示当前文件可以被其他应用读取；MODE_WORLD_WRITEABLE：表示当前文件可以被其他应用写入。
		// 如果希望文件被其他应用读和写，可以传入：
		// openFileOutput("output.txt", Context.MODE_WORLD_READABLE +
		// Context.MODE_WORLD_WRITEABLE);
		String fileName = BW_CONFIGURE_FILE + ".cfg";
		try {
			FileOutputStream fos = act.openFileOutput(fileName,
					act.MODE_PRIVATE);
			fos.write(strcontent.getBytes());
			fos.close();
		} catch (Exception err) {

		}
	}

	/**
	 * 读取文件内容
	 * 
	 * @param fileName
	 *            文件名
	 * @return 文件内容
	 * @throws Exception
	 */
	public static String readPushCfg(Context act) {
		String fileName = BW_CONFIGURE_FILE + ".cfg";
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
		try {
			FileInputStream fis = act.openFileInput(fileName);
			int ilen = fis.available();
			byte[] buf = new byte[ilen];
			fis.read(buf);
			fis.close();
			baos.write(buf, 0, ilen);
			baos.close();
		} catch (Exception err) {

		}
		// 返回内存中存储的数据
		return baos.toString();

	}
	
	// assets下读取文件并拷贝到SD卡
		public static void getAssetsToSd(Context cont, String from, String to)
		{
			File toFile = new File(to);
			if (toFile.isFile() && toFile.exists()) {
				return;
			}
			if (!toFile.getParentFile().exists()) {
				toFile.getParentFile().mkdirs();
			}

			try {
				InputStream is = cont.getResources().getAssets().open(from);
				// FileInputStream fosfrom =(FileInputStream)is;
				FileOutputStream fosto = new FileOutputStream(toFile);

				byte[] bt = new byte[1024];
				int c;
				while ((c = is.read(bt)) > 0) {
					fosto.write(bt, 0, c);
				}
				// 关闭输入、输出流
				is.close();
				fosto.close();
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}
	
	public static void unZip(Context context, String assetName,
			String outputDirectory,boolean isReWrite) throws IOException
	{
		File file = new File(outputDirectory);
		
		if (!file.exists()) {
			file.mkdirs();
		}
		
		InputStream inputStream = context.getAssets().open(assetName);
		ZipInputStream zipInputStream = new ZipInputStream(inputStream);
		
		ZipEntry zipEntry = zipInputStream.getNextEntry();
		
		byte[] buffer = new byte[1024 * 1024];
		
		int count = 0;
		
		while (zipEntry != null) {
			
			if (zipEntry.isDirectory()) {
				file = new File(outputDirectory + File.separator + zipEntry.getName());
				
				if(isReWrite || !file.exists()){
					file.mkdir();
				}
			} else {
				
				file = new File(outputDirectory + File.separator + zipEntry.getName());
				if(isReWrite || !file.exists()){
					file.createNewFile();
					FileOutputStream fileOutputStream = new FileOutputStream(file);
					while ((count = zipInputStream.read(buffer)) > 0) {
						fileOutputStream.write(buffer, 0, count);
					}
					fileOutputStream.close();
				}
			}
			zipEntry = zipInputStream.getNextEntry();
		}
		zipInputStream.close();
	}  // end unZip
		

}

