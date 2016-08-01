package my.global;
import java.io.*;

public class StrUtil 
{
	public static int find(String src,byte[] utf8)
	{
		int len=0;
		try
		{
		len = src.indexOf(new String(utf8,"UTF-8"));
		}
		catch(IOException e)
		{
			e.printStackTrace();
			return 0;
		}
		return len;
	}
}
