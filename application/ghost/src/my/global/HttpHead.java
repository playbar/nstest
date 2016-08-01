package my.global;
import java.io.*;

public class HttpHead 
{
	protected String		m_data;
	
	public HttpHead()
	{
		m_data = new String();
	}
	
	public void Create(String Uri,String method)
	{
		m_data = method+" "+Uri+" "+"HTTP/1.1"+"\r\n";
	}
	
	public void addHead(String name,String value)
	{
		m_data = m_data+name+": "+value+"\r\n";
	}
	
	public byte[] getUtf8()
	{
		byte[] temp=null;
		try
		{
			m_data+="\r\n";
			temp = m_data.getBytes("UTF-8");
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		return temp;
	}
}
