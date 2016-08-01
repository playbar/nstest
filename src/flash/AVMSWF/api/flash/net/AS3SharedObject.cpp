#include "StdAfxflash.h"
#include "splayer.h"
#include "ShellCore.h"
#include "avm2.h"
using namespace avmplus;
#include "AS3SharedObject.h"
#include "AS3SharedObjectFlushStatus.h"
#include "AS3ByteArray.h"

namespace avmshell{
SharedObjectClass::SharedObjectClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	m_nDefaultEncoding=kEncodeDefault;
	m_localObjs=new (gc())EventHashTable(4);
//#ifdef __APPLE__
//	m_bPreventBackup = false;
//#endif
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SharedObjectObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SharedObjectClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SharedObjectObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SharedObjectObject::SharedObjectObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	ShellToplevel*top=(ShellToplevel*)toplevel();
	ByteArrayClass*pClass=top->getByteArrayClass();
	SharedObjectClass*pc=top->getSharedObjectClass();
	m_nDelay=4000;
	m_pBuffer=pClass->CreateArray(0);
	m_pBuffer->AS3_objectEncoding_set(pc->m_nDefaultEncoding);//AS3_defaultObjectEncoding_set(pc->m_nDefaultEncoding);
	RCHashTable*hash=top->GetReleaseObjects();
	if(hash)
		hash->put(this);
	//m_pData=toplevel()->objectClass->newInstance();
}

void SharedObjectObject::Release()
{
	//SaveLocal();
	if(!m_strFile.IsEmpty())
	{
		SaveLocal();
		m_strFile.Empty();
	}
	if(IsRunning())
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		RCHashTable*hash=top->GetReleaseObjects();
		if(hash)
			hash->remove(this);
		SharedObjectClass*pClass=top->getSharedObjectClass();
		pClass->m_localObjs->remove(this->m_strName,pClass);
	}
	EventDispatcherObject::Release();
}

void SharedObjectObject::ReleaseData()
{
	if(m_strFile.IsEmpty()) return;
	SaveLocal();
	m_strFile.Empty();
}


SharedObjectObject* SharedObjectClass::AS3_getLocal(Stringp name,Stringp path,bool)
{
	//StUTF8String s(name);
	//if(strcmp(s.c_str(),"mole_so0")==0)
	//	int v=0;
	name=core()->internString(name);
	Binding bind=m_localObjs->get(name,this);
	if(bind!=BIND_NONE)
	{
		return (SharedObjectObject*)bind;
	}
	SharedObjectObject*pObj=(SharedObjectObject*)createInstance(ivtable(),prototype);
	//pObj->m_strName=name;
	//StUTF8String s(name);
	pObj->m_strName=name;
	pObj->m_strPath=path;
	pObj->CreateLocalFile(name,path);
	pObj->LoadLocal();
	m_localObjs->put(name,this,(Binding)pObj);
	return pObj;
}

SharedObjectObject* SharedObjectClass::AS3_getRemote(Stringp name,Stringp path,Atom atom,bool s)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	SharedObjectObject*pObj=AS3_getLocal(name,path,s);
	return pObj;
}

//////////////////////////////////////////////////////////
//Native Method start...
/*uint32_t SharedObjectObject::AS3_defaultObjectEncoding_get()
{
	//Add your act code here...
	return m_pBuffer->AS3_defaultObjectEncoding_get();
	//return 0;
}*/

bool SharedObjectObject::CreateLocalFile(Stringp name,Stringp path)
{
	if(!m_codeContext) return false;
	ShellCore*c=(ShellCore*)core();
	ScriptPlayer*player=c->GetPlayer()->player;
		//m_codeContext->m_pRoot;//((ShellCore*)core())->GetScriptPlayer();//((ShellCodeContext*)c->codeContext())->m_pRoot;
	m_strFile=c->GetPlayer()->m_strWorkPath.strTxt;
				//ps->m_strPath+="\\";
    m_strFile+="Shared/";

//#if (__CORE_VERSION__>=0x02079000)
	int l=0;
	if(path!=NULL)
	{
		StUTF8String ss(path);
		int i,nc=ss.length();
		const char*str=ss.c_str();
		for(i=0;i<nc;i++)
		{
			char c=str[i];
			if(c=='/'||c=='\\')
				c='_';
			m_strFile+=c;
		}
	}
	else
	{		
		const char*appName=player->GetAppName(l);
		if(appName&&l)
		{
			XString8 s(appName,l);
			m_strFile+=s;
		}
	}
//#else
//	int l=0;
//	const char*appName=player->GetAppName(l);
//	if(appName&&l)
//	{
//		XString8 s(appName,l);
//		m_strFile+=s;
//	}
//#endif
		//m_strFile+=appName.StringAdd(appName,l);
//#if (__CORE_VERSION__>=0x02070100)
	//for Update 2.7.0 to support tian zhi ren,so modify it on 2.7.0
	StUTF8String s(name);
	//m_strFile+=s.c_str();//.StringAdd(s.c_str(),s.length());
	MD5 md5;
	XU8 passwd[16];
	char *sname=(char*)s.c_str();
	md5.MakeMD5(sname,s.length(),(char*)passwd);
	for(l=0;l<16;l++)
	{
		XU8 c=passwd[l]>>4;
		if(c<10)
			m_strFile+=(char)('0'+c);
		else
			m_strFile+=(char)('A'+(c-10));
		c=passwd[l]&0xf;
		if(c<10)
			m_strFile+=(char)('0'+c);
		else
			m_strFile+=(char)('A'+(c-10));
	}
	int iStart=s.length()-1;
	while(iStart>=0)
	{
		if(sname[iStart]=='/'||
		   sname[iStart]=='\\') break;
		iStart--;
	}
	iStart++;
	for(l=0;l<16;l++)
	{
		if(iStart+l>=s.length()) break;
		char c=sname[iStart+l];
		if(c>='0'&&c<='9')
			m_strFile+=c;
		else if(c>='A'&&c<='Z')
			m_strFile+=c;
		else if(c>='a'&&c<='z')
			m_strFile+=c;
		else if(c=='_'||c<0)
			m_strFile+=c;
		else break;
	}
/*#else
	l=m_strFile.GetLength();
	StUTF8String s(name);
	m_strFile+=s.c_str();//.StringAdd(s.c_str(),s.length());
	char*txt=m_strFile.GetData();
	while(txt[l])
	{
		switch(txt[l])
		{
		case '\\':
		case '/':
		case '.':
			txt[l]='_';
			break;
		}
		l++;
	}
#endif*/
	//if(m_strFile.Find("movie")>=0)
	//	int v=0;
	//m_strFile=c->newStringLatin1(txt,m_strPath.GetLength());
	//if(!m_file.Open(txt,XFile::XREAD|
	//m_strPath.StringAdd(player->m_strAppName);
	//m_strPath.StringAdd(list[0].strTxt);
	//LoadObject();
	return true;
}



bool SharedObjectObject::LoadLocal()
{
	if(m_strFile.IsEmpty()) return false;
	XFile file;
	//StUTF8String s(m_strFile);
	if(!file.Open(m_strFile,XFile::XREAD|XFile::XBINARY)) return false;	
	bool bOK=false;
	int l=file.GetLength();
	if(l>3)
	{
		char buf[3];
		file.Read(buf,3);
		if(buf[0]=='3'&&buf[1]=='S'&&buf[2]=='O')
		{
			//ByteArrayObject*pObj=((ShellToplevel*)toplevel())->getByteArrayClass()->CreateArray(l-3);
			m_pBuffer->SetLength(l-3);			
			file.Read(m_pBuffer->GetByteArray().GetBuffer(),l-3);
			m_pBuffer->Seek(0);
			m_pData=core()->atomToScriptObject(m_pBuffer->AS3_readObject());
			bOK=m_pData!=NULL;
		}
	}
	file.Close();
	return bOK;
}

bool SharedObjectObject::SaveToBuffer()
{
	m_pBuffer->Seek(0);
	m_pBuffer->SetLength(0);
	if(m_pData)
		m_pBuffer->AS3_writeObject(m_pData->atom());
	return true;
}

bool SharedObjectObject::SaveLocal()
{
	if(!m_pData||m_strFile.IsEmpty())
		return false;
	//if(m_strFile.Find("GameLoadermain_xx")>=0)
	//	int v=0;
	XFile file;
	//StUTF8String s(m_strFile);
	if(!file.Open(m_strFile,XFile::XWRITE|XFile::XCREATE|XFile::XBINARY))
		return false;
	file.Write("3SO",3);
	SaveToBuffer();
	//ByteArrayObject*pObj=((ShellToplevel*)toplevel())->getByteArrayClass()->CreateArray(64);
	//m_pBuffer->Seek(0);
	//m_pBuffer->SetLength(0);
	//m_pBuffer->AS3_writeObject(m_pData->atom());
	file.Write(m_pBuffer->GetByteArray().GetBuffer(),m_pBuffer->GetLength());
	file.Close();
	return true;
}

/*void SharedObjectObject::AS3_defaultObjectEncoding_set(uint32_t v)
{
	//Add your act code here...
	m_pBuffer->AS3_defaultObjectEncoding_set(v);
}*/


Atom SharedObjectObject::AS3_data_get()
{
	//Add your act code here...
	if(m_pData==NULL)
		m_pData=toplevel()->objectClass->newInstance();
	return m_pData->atom();
}

void SharedObjectObject::AS3_fps_set(double v)
{
	//Add your act code here...
	if(v) m_nDelay=1000/v;
	else  m_nDelay=4000;
}

uint32_t SharedObjectObject::AS3_objectEncoding_get()
{
	//Add your act code here...
	return m_pBuffer->AS3_objectEncoding_get();
}

void SharedObjectObject::AS3_objectEncoding_set(uint32_t v)
{
	//Add your act code here...	
	m_pBuffer->AS3_objectEncoding_set(v);
}

uint32_t SharedObjectObject::AS3_size_get()
{
	//Add your act code here...
	//return 0;//Modify this please!
	this->SaveLocal();
	int s=m_pBuffer->GetLength();
	return s>4?s:0;
}

void SharedObjectObject::AS3_clear()
{
	//Add your act code here...	
	m_pData=NULL;
	if(!m_strFile.IsEmpty())
	{
		//StUTF8String s(m_strFile);
		XFile::RemoveFile(m_strFile);
	}
}

void SharedObjectObject::AS3_close()
{
	//Add your act code here...
	m_connect=NULL;
}

void SharedObjectObject::AS3_connect(NetConnectionObject*net,Stringp s)
{
	//Add your act code here...
	m_connect=net;
	LoadNet(s);
}

Stringp SharedObjectObject::AS3_flush(int)
{
	//Add your act code here...
	SaveToBuffer();
	SaveNet();
	bool bOK=SaveLocal();
	SharedObjectFlushStatusClass*pClass=((ShellToplevel*)toplevel())->getSharedObjectFlushStatusClass();
	//[Note X]
	return pClass->getSlotFLUSHED();
	//return bOK?pClass->getSlotFLUSHED():pClass->getSlotPENDING();
}

void SharedObjectObject::AS3_send(ArrayObject*)
{
	//Add your act code here...
	
}

void SharedObjectObject::AS3_setDirty(Stringp)
{
	//Add your act code here...
	SaveToBuffer();
	SaveLocal();
	SaveNet();
}

void SharedObjectObject::AS3_setProperty(Stringp name,Atom value)
{
	//Add your act code here...
	if(m_pData)
	{
		name = core()->internString(name);
		m_pData->setAtomProperty(name->atom(),value);
		AS3_setDirty(name);
	}
}

bool SharedObjectObject::LoadNet(Stringp)
{
	return false;
}
bool SharedObjectObject::SaveNet()
{
	return false;
}

//new adds 11
//#ifdef __APPLE__
bool SharedObjectClass::AS3_preventBackup_get()
{
	//LOGWHERE();
	return m_bPreventBackup;
}
void  SharedObjectClass::AS3_preventBackup_set(bool b)
{
	//LOGWHERE();
	m_bPreventBackup = b;
	
}
//#endif
//new adds 11 end

}