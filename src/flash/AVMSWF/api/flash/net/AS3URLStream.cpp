#include "StdAfxflash.h"

#include "ShellCore.h"
using namespace avmplus;
#include "avm2.h"
#include "AS3URLStream.h"
#include "AS3Endian.h"
#include "splayer.h"
namespace avmshell{
URLStreamClass::URLStreamClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())URLStreamObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* URLStreamClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) URLStreamObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
URLStreamObject* URLStreamClass::CreateURLStream()
{
	URLStreamObject *pObj = (URLStreamObject *)createInstance(ivtable(), prototype);

	return pObj;
}

URLStreamObject::URLStreamObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0),
				 m_data(toplevel())
{
	//Add your construct code here...
	m_bConnected=false;
	m_nTotalBytes=0;
	m_nLoadedBytes=0;
	m_pClient=NULL;
	m_nLoadID = 0;
}
//////////////////////////////////////////////////////////
//Native Method start...
uint32_t URLStreamObject::AS3_bytesAvailable_get()
{
	//Add your act code here...
	return m_data.InCacheBytesAvailable();
}

bool URLStreamObject::AS3_connected_get()
{
	//Add your act code here...
	return m_bConnected;
}

Stringp URLStreamObject::AS3_endian_get()
{
	//Add your act code here...
	EndianClass*pClass=((ShellToplevel*)toplevel())->getEndianClass();
	return (m_data.GetEndian() == kBigEndian) ? pClass->getSlotBIG_ENDIAN():pClass->getSlotLITTLE_ENDIAN();
}

void URLStreamObject::AS3_endian_set(Stringp endianSet)
{
	//Add your act code here...
	AvmCore* core = this->core();
	endianSet = core->internString(endianSet);
	EndianClass*pClass=((ShellToplevel*)toplevel())->getEndianClass();
	if (endianSet == pClass->getSlotBIG_ENDIAN())//core->internConstantStringLatin1("bigEndian"))
	{
		m_data.SetEndian(kBigEndian);
	}
	else if (endianSet == pClass->getSlotLITTLE_ENDIAN())//core->internConstantStringLatin1("littleEndian"))
	{
		m_data.SetEndian(kLittleEndian);
	}
	else
	{
		toplevel()->throwArgumentError(kInvalidArgumentError,endianSet);// "endianSet");
	}
}

void URLStreamObject::Release()
{
	
	EventDispatcherObject::Release();
}

uint32_t URLStreamObject::AS3_objectEncoding_get()
{
	//Add your act code here...
	return m_data.GetObjectEncoding();
}

void URLStreamObject::AS3_objectEncoding_set(uint32_t objectEncoding)
{
	//Add your act code here...
	m_data.SetObjectEncoding((ObjectEncoding)objectEncoding);
}

void URLStreamObject::AS3_close()
{
	//Add your act code here...
	if(m_pClient)
	{
		((XClient*)m_pClient)->Cancel();
		((XClient*)m_pClient)->SetWindow(NULL,0,0);
		((ShellCore*)core())->GetPlayer()->CloseClient(m_pClient);
		m_pClient=NULL;
	}
	else if(m_nLoadID)
	{
		((ShellCore*)core())->GetPlayer()->RemoveLoader(m_nLoadID);
		m_nLoadID = 0;
	}
	//if(m_bConnected)
	//{
		m_bConnected=false;
	//}
#ifdef _WINEMU_DEBUGFILE
	if(m_file.IsValid())
		m_file.Close();
#endif
}


void URLStreamObject::AS3_load(URLRequestObject *url)
{
	//return;
	//Add your act code here...
	/*ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	StUTF8String strURL(url->m_strURL);
	XU8Array datas;
	Stringp strType=url->GetPostData(datas,false);
	if(strType==NULL) strType=url->m_strContext;
	int nType=kHttpLoadTarget|kHttpLoadData;
	if(url->m_nMethod==URLRequestMethodClass::POST)
		nType|=kHttpSendUsePost;
	else
		nType|=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),NULL,nType,NULL,this,strType?StUTF8String(strType).c_str():NULL);
	if(datas.GetSize())
		pInfo->postData.Append(datas);*/
	//StUTF8String s(url->m_strURL);
	//if(strstr(s.c_str(),"skeleton/color/down/key.swf?gnlk089s")!=0)
	//	int v=0;
	if(m_pClient)
	{
		((XClient*)m_pClient)->Cancel();
		((XClient*)m_pClient)->SetWindow(NULL,0,0);
		((ShellCore*)core())->GetPlayer()->CloseClient(m_pClient);
		m_pClient=NULL;
	}
	else if(m_nLoadID)
	{
		((ShellCore*)core())->GetPlayer()->RemoveLoader(m_nLoadID);
		m_nLoadID = 0;
	}
	m_bConnected=true;
	m_nLoadID = LoadURL(url,NULL);
}


bool URLStreamObject::StreamInNew(void*sreamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo)
{
	//Atom atom=m_pData;
	m_pClient=pClient;
	AvmCore*c=core();
	ShellToplevel*top=(ShellToplevel*)toplevel();
	_XStreamData*stream=(_XStreamData*)sreamData;	// link the objects
	//this->m_bytesTotal=stream->nTotalBytes;
	//this->m_bytesLoaded=0;
	//m_bCanceled=false;
	m_bConnected=true;
	m_nTotalBytes=stream->nTotalBytes;
	m_nLoadedBytes=0;
	m_data.SetInCacheLength(stream->nTotalBytes);

#ifdef _WINEMU_DEBUGFILE
			char*host=strstr(streamUrl,"://");
			if(host)
			{
				//((ShellToplevel*)toplevel())->PushObject(m_pLoaderInfo);	
				
				char* id=strrchr(streamUrl,'?');
				char* start=NULL;//strchr(host+3,'/');	
				if(id)
				{
					start=id-1;
					while(start>streamUrl)
					{
						if(*start=='/') break;
						start--;
					}
				}
				else
					start=strrchr(host,'/');

				if(start)
				{
					//start+=3;
					char*s=streamUrl;
					int iCode=0;
					while(s<start)
					{
						iCode+=*s;
						s++;
					}
					//ShellCodeContext*code=(ShellCodeContext*)core()->codeContext();
					
						//code->m_pRoot;
					XString8 strPath=((ShellCore*)core())->GetPlayer()->m_strWorkPath.strTxt;
					strPath+="Datas\\";
					XFile::CreateFolder(strPath);
					//int is=strPath.GetLength(),ie;
					XString8 strInt;
					strInt.FromInt(iCode);
					strPath+=strInt;
					strPath+="STREAM_";
					if(id)
						strPath.AddString(start+1,id-start-1);
					else
						strPath.AddString(start+1);
					if(m_file.IsValid())
						m_file.Close();
					m_file.Open(strPath,XFile::XCREATE|XFile::XBINARY|XFile::XWRITE);
				}

			}
#endif

	OnEvent(XAS3_EVENT_open,false);
	OnHTTPStatusEvent(nCode);
	/*if(hasEvent(XAS3_EVENT_httpStatus))
	{
		EventObject*pEvent=top->getHTTPStatusEventClass()->CreateEventObject(XAS3_EVENT_httpStatus,nCode);
		pEvent->pTarget=this;
		//pEvent->pTarget=(ScriptObject*)m_pLoaderInfo;
		AS3_dispatchEvent(pEvent);
	}*/
	//p->AS3OnOpen();
	//p->AS3OnHttpStatus(nCode);
	
	return true;
}
int URLStreamObject::StreamInWrite(void* buffer,int  length,void*gcEnter,void*pClient)
{
	m_nLoadedBytes+=length;
	m_data.WriteInCache(buffer,length);
	m_data.ClearInput();
#ifdef _WINEMU_DEBUGFILE
	if(m_file.IsValid())
	{
		m_file.Write(buffer,length);
	}
#endif
	OnProgressEvent(XAS3_EVENT_progress,this->m_nLoadedBytes,this->m_nTotalBytes);
	if(!m_bConnected) return XSWFPlayer::LOAD_BREAK;
	return XSWFPlayer::LOAD_OK;
	
}
bool URLStreamObject::StreamInDestroy(XBOOL bOK,const char*msg,void*pClient)
{
#ifdef _WINEMU_DEBUGFILE
	if(m_file.IsValid())
		m_file.Close();
#endif
	if(m_pClient&&m_pClient!=pClient)
		return true;
	m_nLoadID = 0;
	//m_bConnected=false;
	if(bOK)
	{
		OnEvent(XAS3_EVENT_complete,false);
	}
	else
	{
		OnIOErrorEvent(msg);
	}
	return true;
}

bool URLStreamObject::AS3_readBoolean()
{
	//Add your act code here...
	return m_data.ReadBoolean();
}

int URLStreamObject::AS3_readByte()
{
	//Add your act code here...
	return m_data.ReadByte();
}

void URLStreamObject::AS3_readBytes(ByteArrayObject *pBytes, uint32_t offset, uint32_t length)
{
	//Add your act code here...
	m_data.ReadBytes(pBytes->GetByteArray(),offset,length);
}

double URLStreamObject::AS3_readDouble()
{
	//Add your act code here...
	return m_data.ReadDouble();
}

double URLStreamObject::AS3_readFloat()
{
	//Add your act code here...
	return m_data.ReadFloat();
}

int32_t URLStreamObject::AS3_readInt()
{
	//Add your act code here...
	return m_data.ReadInt();
}

Stringp URLStreamObject::AS3_readMultiByte(uint32_t length, Stringp charSet)
{
	//Add your act code here...
	return m_data.ReadMultiByte(length,charSet);
}

Atom URLStreamObject::AS3_readObject()
{
	//Add your act code here...
	//ArrayClass*pClass=toplevel()->arrayClass
	ArrayObject*refString=toplevel()->arrayClass->newArray(0);
	ArrayObject*refObject=toplevel()->arrayClass->newArray(0);
	ArrayObject*refTraits=toplevel()->arrayClass->newArray(0);
	return m_data.ReadObject(this->atom(),refString,refObject,refTraits);
}

int32_t URLStreamObject::AS3_readShort()
{
	//Add your act code here...
	return m_data.ReadShort();
}

uint32_t URLStreamObject::AS3_readUnsignedByte()
{
	//Add your act code here...
	return m_data.ReadUnsignedByte();
}

uint32_t URLStreamObject::AS3_readUnsignedInt()
{
	//Add your act code here...
	return m_data.ReadUnsignedInt();
}

uint32_t URLStreamObject::AS3_readUnsignedShort()
{
	//Add your act code here...
	return m_data.ReadUnsignedShort();
}

Stringp URLStreamObject::AS3_readUTF()
{
	//Add your act code here...
	return m_data.ReadUTF();
}

Stringp URLStreamObject::AS3_readUTFBytes(uint32_t length)
{
	//Add your act code here...
	return m_data.ReadUTFBytes(length);
}

}