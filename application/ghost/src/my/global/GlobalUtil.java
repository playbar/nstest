package my.global;

import java.io.InputStream;
import android.content.Context;
import java.lang.String;
import android.content.res.AssetManager;
import java.io.IOException;

public class GlobalUtil {
	public static final boolean m_bDesktop = true;
	
	public static InputStream OpenApkFile(Context context,String fname)
	{
		InputStream input = null;
		AssetManager assetManager = context.getAssets();
		try{
		input = assetManager.open(fname);
		}
		catch(IOException e)
		{
			return null;
		}
		return input;
	}

}
