#include "StdAfxflash.h"


#include "ShellCore.h"
using namespace avmplus;
#include "avm2.h"
#include "AvmCore.h"
#include "BXFile.h"
#include "AS3ByteArray.h"
#include "StdAfxggdom.h"
#include "splayer.h"
#include "XSystem.h"

namespace avmshell
{
	static void GetFileName(Stringp name,XString8&fileName)
	{
		fileName = XSWFPlayer::m_pInstance->m_strFilePath;
		StUTF8String s(name);
		XString8 sn(s.c_str(),s.length());
		if(sn.Find('/')>=0)
		{
			MD5 md5;
			char chars[16];
			md5.MakeMD5(sn.GetData(),sn.GetLength(),chars);
			//XString8 strOut;//(chars,16);
			for(int i=0;i<16;i++)
			{
				//XString8 strTmp;
				//strTmp.FromInt(chars[i],XTRUE);
				//strOut+=strTmp;
				int c=chars[i]&0xf;
				if(c>=0&&c<=9)
					fileName+=(char)('0'+c);
				else
					fileName+=(char)('a'+c-10);
				c=(chars[i]>>4)&0xf;
				if(c>=0&&c<=9)
					fileName+=(char)('0'+c);
				else
					fileName+=(char)('a'+c-10);
					//str
			}
			int is=sn.ReverseFind('/');
			int ie=sn.Find('.',is);
			XString8 strTmp;
			sn.Mid(strTmp,is+1,ie-is-1);
			//fileName+=strOut;
			fileName+=strTmp;
		}

		else
			fileName+=sn;//XString8(s.c_str(),s.length());
		fileName+=XString8(".dat");
#ifdef _WIN32
		fileName.ConvertToGB();
#endif
	}
	BXFileClass::BXFileClass(VTable *vtable):ClassClosure( vtable )
	{
		createVanillaPrototype();
	}
	ScriptObject* BXFileClass::createInstance(VTable* ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) BXFileObject(ivtable, prototype, 0);
	}
	////////////////////////////////////////////////////////////////////////////////////
	bool BXFileClass::AS3_deleteFile(Stringp file)
	{
		XString8 fileName;
		GetFileName(file,fileName);
		XFile::RemoveFile(fileName);
		return true;
	}
	
	bool BXFileClass::AS3_copyFile(Stringp src,Stringp dst)
	{
		XString8 srcFile,dstFile;
		GetFileName(src,srcFile);
		GetFileName(dst,dstFile);
		XFile file1,file2;
		if(!file1.Open(srcFile,XFile::XBINARY|XFile::XREAD)) return false;
		if(!file2.Open(dstFile,XFile::XBINARY|XFile::XCREATE|XFile::XWRITE))
		{
			file1.Close();
			return false;
		}
		int l=file1.GetLength();
		if(l>0)
		{
			XU8Array dat;
			dat.SetSize(l);
			file1.Read(dat,l);
			file2.Write(dat,l);
		}
		file1.Close();
		file2.Close();
		return true;
	}
	bool BXFileClass::AS3_exec(Stringp name,int type)
	{
		if(name==NULL||name->length()<=0) return false;
		XSWFPlayer*player=XSWFPlayer::m_pInstance;
		StUTF8String str(name);
		switch(type)
		{
		case 0://SWF
			 {
				 XString8 file;
				 GetFileName(name,file);
				 player->RunSWF(file);
			 }
			 break;
		case 1://NETGAME
			 {
				//const char*targets = target.c_str();
				 if(!player->IsJQBar(str.c_str())) return false;
				 //player->RunSWF(str.c_str()); 
				_XLoadInfo*pInfo=player->AddLoader(str.c_str(),NULL);//,NULL);//,nType,NULL,NULL,strType?StUTF8String(strType).c_str():NULL);
				//pInfo->strRefer=player->strURLBase;	
			 }
			 break;
		case 2://Web Page
			 if(!player->IsJQBar(str.c_str())) return false;
			 XSystem::OpenWebView(str.c_str(),0);
			
			 break;
		case 3://Application
			XSystem::Execl(str.c_str(),NULL);
			break;
		default:
			return false;
		}
		return true;
	}
	
	bool BXFileClass::AS3_createShortcutIcon(Stringp file,Stringp image,Stringp name)
	{
		return true;
	}
	
	bool BXFileClass::AS3_reload()
	{
		XSWFPlayer*player=XSWFPlayer::m_pInstance;
		player->Reload();
		return true;
	}
	
	bool BXFileClass::AS3_isExist(Stringp file)
	{
		XString8 strFile;
		GetFileName(file,strFile);
		XFile f;
		if(!f.Open(strFile,XFile::XBINARY|XFile::XREAD))
			return false;
		f.Close();
		return true;
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	BXFileObject::BXFileObject(VTable* vtable, ScriptObject* proto, int capacity) : ScriptObject(vtable, proto, 0)
	{

	}

	void BXFileObject::Release()
	{
		ScriptObject::Release();
		m_file.Close();
	}

	

	bool BXFileObject::AS3_open(Stringp file,int mode)
	{
		//return false;
		int os = XFile::XBINARY;
		if((mode&1)!=0)
			os|= XFile::XREAD;
		if((mode&2)!=0)
			os|= XFile::XWRITE;
		if((mode&4)!=0)
			os|= XFile::XCREATE;
		if((mode&8)!=0)
			os|= XFile::XAPPEND;
		XString8 strFile;
		GetFileName(file,strFile);
		//return false;
		m_nMode = os;
		if(!m_file.Open(strFile,os)) return false;
		return true;
	}
		
	bool BXFileObject::AS3_close()
	{
		if(!m_file.IsValid()) return false;
		m_file.Close();
		return true;
	}
	
	int  BXFileObject::AS3_getLength()
	{
		if(m_file.IsValid())
		{
			return m_file.GetLength();
		}
		return 0;
	}
	
	int  BXFileObject::AS3_seek(int pos,int type)
	{
		if(!m_file.IsValid()) return 0;
		switch(type)
		{
		default:
		case 0:
			return m_file.Seek(pos,XFile::XSEEK_SET);
		case 1:
			return m_file.Seek(pos,XFile::XSEEK_CUR);
		case 2:
			return m_file.Seek(pos,XFile::XSEEK_END);
		}
		return 0;
	}
	
	int  BXFileObject::AS3_write(ByteArrayObject*data,int pos,int len)
	{
		if(data==NULL||!m_file.IsValid()) return 0;
		if((m_nMode&XFile::XWRITE)==0)
			return 0;
		DataFile&dat=data->GetByteArray();
		if(pos+len>dat.GetLength())
			len=dat.GetLength()-pos;
		if(len<=0) return 0;
		m_file.Write(dat.GetBuffer()+pos,len);
		return 0;
	}
	
	int  BXFileObject::AS3_read(ByteArrayObject*data,int pos,int len)
	{
		if(data==NULL||!m_file.IsValid()) return 0;
		if((m_nMode&XFile::XREAD)==0)
			return 0;
		DataFile&dat=data->GetByteArray();
		if(dat.GetLength()<pos+len)
			dat.SetLength(pos+len);
		return m_file.Read(dat.GetBuffer()+pos,len);
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////

}
