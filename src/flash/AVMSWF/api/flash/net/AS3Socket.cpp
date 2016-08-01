#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;

#include "ShellCore.h"
#include "AS3Endian.h"
using namespace avmshell;
using namespace ExtErrorContext;


#include "avm2.h"
#include "splayer.h"
#include "XConnect.h"
#include "XStream.h"
#include "XConnectTCP.h"
#include "XClientApply.h"

#include "AS3Socket.h"


#define MIN_PORT 1024

namespace avmshell{
SocketClass::SocketClass(VTable* cvtable):ClassClosure(cvtable)//EventDispatcherClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SocketObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SocketClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SocketObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SocketObject* SocketClass::CreateSocket(Stringp host, int32_t port)
{
	SocketObject *pObj = (SocketObject *)createInstance(ivtable(), prototype);
	//pObj->AS3_connect(host, port);
	/*CONNECTREQ req;
	StUTF8String url(host);
	req.URL.setURL(url.c_str());
	XSWFPlayer*p=((ShellCore*)core())->GetPlayer();
	XWindow*pWnd=p->pSocketWnd;
	XClientApply pApply=p->pApply;


	m_pClient=pAlly->CreateClient(req,(XU32)this,pWnd);
	
	SocketObject*pObj=(m_pClient->GetData();
	//XClient*pApply->CreateClient(this);*/
	pObj->AS3_connect(host,port);
	
	return pObj;
}

SocketObject::SocketObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0), m_cache(toplevel())
{
	//Add your construct code here...
	/*ShellCore *pShellCore = (ShellCore *)core();
	XSWFPlayer *pPlayer = (XSWFPlayer *)pShellCore->GetPlayer();

	CONNECTREQ req;
//	m_pClient = pPlayer->m_pClientApply->CreateClient(req, (XU32)this, pPlayer->m_pNetWindow);
	m_pClient->m_nProtoType = XPROTO_SOCKET;*/
	m_nLoaded=0;
	m_nTimeout=30;
	m_pClient = NULL;
}

void SocketObject::ReleaseClient()
{
	if(m_pClient)
	{
		ShellCore *pShellCore = (ShellCore *)core();
		XSWFPlayer *pPlayer = (XSWFPlayer *)pShellCore->GetPlayer();
		m_pClient->SetUsing(false);
		m_pClient->Cancel();
		m_pClient->SetWindow(NULL,0,0);
		//pPlayer->m_pClientApply->ReleaseClient(m_pClient);
		m_pClient=NULL;
	}
}

void SocketObject::Release()
{
	//int v=sizeof(SocketObject);//SocketFile);
	ReleaseClient();
	EventDispatcherObject::Release();
}


//////////////////////////////////////////////////////////
//Native Method start...
uint32_t SocketObject::AS3_bytesAvailable_get()
{
	//Add your act code here...
	return this->m_cache.BytesAvailable();//Modify this please!
}

bool SocketObject::AS3_connected_get()
{
	//Add your act code here...
	bool bOK= m_pClient!=NULL&&m_pClient->IsConnecting();
	return bOK;
		//((XConnectTCP *)m_pClient->GetConnect())->IsConnect();
}

Stringp SocketObject::AS3_endian_get()
{
	//Add your act code here...
	EndianClass*pClass=((ShellToplevel*)toplevel())->getEndianClass();
	return (m_cache.GetEndian() == kBigEndian) ? pClass->getSlotBIG_ENDIAN():pClass->getSlotLITTLE_ENDIAN();
	//if(m_cache.GetEndian()==;
	//return NULL;//Modify this please!
}

void SocketObject::AS3_endian_set(Stringp endianSet)
{
	//Add your act code here...
	AvmCore* core = this->core();
	endianSet = core->internString(endianSet);
	EndianClass*pClass=((ShellToplevel*)toplevel())->getEndianClass();
	if (endianSet == pClass->getSlotBIG_ENDIAN())//core->internConstantStringLatin1("bigEndian"))
	{
		m_cache.SetEndian(kBigEndian);
	}
	else if (endianSet == pClass->getSlotLITTLE_ENDIAN())//core->internConstantStringLatin1("littleEndian"))
	{
		m_cache.SetEndian(kLittleEndian);
	}
	else
	{
		toplevel()->throwArgumentError(kInvalidArgumentError, endianSet);//"endianSet");
	}
}

uint32_t SocketObject::AS3_objectEncoding_get()
{
	//Add your act code here...
	return m_cache.GetObjectEncoding();
	//return -1;
}

void SocketObject::AS3_objectEncoding_set(uint32_t objectEncoding)
{
	//Add your act code here...
	m_cache.SetObjectEncoding((ObjectEncoding)objectEncoding);
}

//AS3 contructor function..
void SocketObject::AS3_constructor(Stringp host, int32_t port)
{
//	m_sock.CreateSocket((XPCTSTR)host->GetBuffer()->p8, port);
	//XSWFPlayer::m_pInstance->Trace(XString16("---Socket"));
	ShellCore	*pCore = (ShellCore*)core();
	XAVM2 *pAVM2 = pCore->GetPlayer()->m_pAVM2;

	if(host)
		AS3_connect(host,port);
	/*if (NULL == host || port == 0)
	{
		((XConnectTCP *)m_pClient->GetConnect())->CreateSocket((XPCTSTR)host->GetBuffer()->p8, port);
	}
	else if (!CheckDomainArea(host) || (port < MIN_PORT))
	{
		//EventObject *pEvent = (EventObject*)pAVM2->CreateEventObject(XAS3_EVENT_securityError,XAS3EVENT_SECURITYERROR,true,0,(int *)this);
		//this->AS3_dispatchEvent(pEvent);
		this->OnSecurityErrorEvent("error");
	}
	else
	{
		CONNECTINFO* pConnectInfo = m_pClient->GetConnectInfo();
		XString8	strHost((XPCTSTR)host->GetBuffer()->p8);

		pConnectInfo->URL.m_strHost = strHost;
		pConnectInfo->URL.m_nProto = port;

		//{
		//	EventObject *pEvent = (EventObject*)pAVM2->CreateEventObject(XAS3_EVENT_connect,XAS3EVENT_EVENT,true,0,(int *)this);
		//	this->AS3_dispatchEvent(pEvent);
		//}
		if (XConnect::CWAIT_OK != m_pClient->GetConnect()->Connect(XTRUE))
		{
			//EventObject *pEvent = (EventObject*)pAVM2->CreateEventObject(XAS3_EVENT_ioError,XAS3EVENT_IOERROR,true,0,(int *)this);
			//this->AS3_dispatchEvent(pEvent);
			this->OnIOErrorEvent("error");
		}
	}*/
}

void SocketObject::AS3_close()
{
	if(m_pClient)
	{
		m_pClient->SetUsing(false);
		m_pClient->Cancel();
		m_pClient->SetWindow(NULL,0,0);
		((ShellCore*)core())->GetPlayer()->RemoveClient(m_pClient);
		m_pClient=NULL;
		//OnClose(NULL);//OnEvent(XAS3_EVENT_close,false,pObject);
		//Release();
	}
	//Add your act code here...
	//if (m_pClient->IsConnecting())
	//{
	//	((XConnectTCP *)m_pClient->GetConnect())->DisConnect();
	//}
	//else
	//{
	//	ShellCore	*pCore = (ShellCore*)core();
	//	XAVM2 *pAVM2 = pCore->GetPlayer()->m_pAVM2;

	//	EventObject *pEvent = (EventObject*)pAVM2->CreateEventObject(XAS3_EVENT_ioError,XAS3EVENT_IOERROR,true,0,(int *)this);
	//	this->AS3_dispatchEvent(pEvent);
	//}
}



void SocketObject::AS3_connect(Stringp host, int32_t port)
{
	//Add your act code here...
	//XSWFPlayer::m_pInstance->Trace(XString16("---Socket"));
	//return;
	if(m_pClient&&m_pClient->IsConnecting()) 
	{
		//[Note X]
		//this->OnConnected();
		return;
	}
	if(!port||!host||host->isEmpty()) return;
	ShellCore	*pCore = (ShellCore*)core();
	XAVM2 *pAVM2 = pCore->GetPlayer()->m_pAVM2;

	if (NULL == host)
	{
		//...get host
		if(m_codeContext)
		{
			host=m_codeContext->GetHost(pCore);
		}
	}

	if (!host)//!CheckDomainArea(host) || (port < MIN_PORT))
	{
		//EventObject *pEvent = (EventObject*)pAVM2->CreateEventObject(XAS3_EVENT_securityError,XAS3EVENT_SECURITYERROR,true,0,(int *)this);
		//this->AS3_dispatchEvent(pEvent);
		this->OnIOErrorEvent("error");
		return;
		//this->OnSecurityErrorEvent("error");
	}
	else
	{
		//if(!m_pClient)
		//	m_pClient=pCore->GetPlayer()->m_pClientApply->CreateClient(
		if(m_pClient)
			ReleaseClient();
		m_bNeedRelease = false;
		StUTF8String str(host);
		//XString8 strHost(str.c_str(),str.length());
		XSWFPlayer*player=pCore->GetPlayer();
		CONNECTREQ req;
		req.URL.m_nProto=XPROTO_SOCKET;
		//req.URL.m_strURL=strHost;
		req.URL.m_strHost=str.c_str();//strHost;
		req.URL.m_uPort=port;
		m_nLoaded=0;
		m_pClient=player->m_pClientApply->CreateClient(req,0,(XU32)this,player->m_pNetWindow);
		//_sClient = (XClient*)m_pClient;
		m_pClient->SetUsing(true);
		m_pClient->Connect(req,XCHARSET_GB2312);
#ifdef _WINEMU
		XString16 si("======>Try socket to host:");
		si+=XString16(req.URL.m_strHost);
		XString16 sii;
		sii.FromInt(req.URL.m_uPort);
		si+=XString16("(");
		si+=sii;
		si+=XString16(")");
		pCore->GetPlayer()->Trace(si);
#endif
	}
}

void SocketObject::AS3_flush()
{
	//Add your act code here...
	//XStream *pStream = m_pClient->GetConnect()->GetOutStream();
	//pStream->WriteData(m_cache.m_outCache.GetBuffer(), m_cache.m_outCache.GetLength());

	//m_cache.SeekOutCacheWritePointer(0);
	if(m_pClient)
	{
		int l=0;
		XU8*pData=m_cache.GetOutput(l);
		if(l)
		{
			//XStream *pStream = m_pClient->GetConnect()->GetOutStream();
			//pStream->WriteData(pData,l);
			m_pClient->GetConnect()->Write(pData,l);//SendData();
			m_cache.ResetOutput();
		}
	}
}

bool SocketObject::AS3_readBoolean()
{
	//Add your act code here...
	return m_cache.ReadBoolean();
}

int32_t SocketObject::AS3_readByte()
{
	//Add your act code here...
	return m_cache.ReadByte();
}

void SocketObject::AS3_readBytes(ByteArrayObject* byteArray, uint32_t offset, uint32_t length)
{
	//Add your act code here...
	m_cache.ReadBytes(byteArray->GetByteArray(), offset, length);
}

double SocketObject::AS3_readDouble()
{
	//Add your act code here...
	return m_cache.ReadDouble();
}

double SocketObject::AS3_readFloat()
{
	//Add your act code here...
	return m_cache.ReadFloat();
}

int32_t SocketObject::AS3_readInt()
{
	//Add your act code here...
	return m_cache.ReadInt();
}

Stringp SocketObject::AS3_readMultiByte(uint32_t length, Stringp charSet)
{
	//Add your act code here...
	return m_cache.ReadMultiByte(length, charSet);
}

Atom SocketObject::AS3_readObject()
{
	//Add your act code here...
	ArrayObject*refString=toplevel()->arrayClass->newArray(0);
	ArrayObject*refObject=toplevel()->arrayClass->newArray(0);
	ArrayObject*refTraits=toplevel()->arrayClass->newArray(0);
	return m_cache.ReadObject(this->atom(),refString,refObject,refTraits);
}

int32_t SocketObject::AS3_readShort()
{
	//Add your act code here...
	return m_cache.ReadShort();
}

uint32_t SocketObject::AS3_readUnsignedByte()
{
	//Add your act code here...
	return m_cache.ReadUnsignedByte();
}

uint32_t SocketObject::AS3_readUnsignedInt()
{
	//Add your act code here...
	return m_cache.ReadUnsignedInt();
}

uint32_t SocketObject::AS3_readUnsignedShort()
{
	//Add your act code here...
	return m_cache.ReadUnsignedShort();
}

Stringp SocketObject::AS3_readUTF()
{
	//Add your act code here...
	return m_cache.ReadUTF();
}

Stringp SocketObject::AS3_readUTFBytes(uint32_t length)
{
	//Add your act code here...
	return m_cache.ReadUTFBytes(length);
}

void SocketObject::AS3_writeBoolean(bool data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteBoolean(data);
}

void SocketObject::AS3_writeByte(int32_t data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteByte(data);
}

void SocketObject::AS3_writeBytes(ByteArrayObject *data, uint32_t offset, uint32_t length)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteBytes(data->GetByteArray(), offset, length);
}

void SocketObject::AS3_writeDouble(double data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteDouble(data);
}

void SocketObject::AS3_writeFloat(double data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteFloat(data);
}

void SocketObject::AS3_writeInt(int32_t data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteInt(data);
}

void SocketObject::AS3_writeMultiByte(Stringp data, Stringp charSet)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteMultiByte(data, charSet);
}

void SocketObject::AS3_writeObject(Atom atom)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}
	ArrayObject*refString=toplevel()->arrayClass->newArray(0);
	ArrayObject*refObject=toplevel()->arrayClass->newArray(0);
	m_cache.WriteObject(this->atom(),atom,refString,refObject);
}

void SocketObject::AS3_writeShort(int32_t data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteShort(data);
}

void SocketObject::AS3_writeUnsignedInt(uint32_t data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteUnsignedInt(data);
}

void SocketObject::AS3_writeUTF(Stringp data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteUTF(data);
}

void SocketObject::AS3_writeUTFBytes(Stringp data)
{
	//Add your act code here...
	//if (!m_sock.CheckSocket())
	//{
	//	((ShellCore *)core())->throwExtError(kExtError_IOError, core->internConstantStringLatin1("Socket"));
	//}

	m_cache.WriteUTFBytes(data);
}

bool SocketObject::OnClose(const char*msg)
{	
	//return true;
	m_bNeedRelease = true;
	bool ret=EventDispatcherObject::OnClose(msg);
	if(m_bNeedRelease)
		ReleaseClient();
	return ret;
}

//new adds
Stringp SocketObject::AS3_localAddress_get()
{
	return m_pLocalAddress;
}
int SocketObject::AS3_localPort_get()
{
	return m_localPort;
}
Stringp SocketObject::AS3_remoteAddress_get()
{
	return m_pRemoteAddress;
}
int SocketObject::AS3_remotePort_get()
{
	return m_remotePort;
}
uint32_t SocketObject::AS3_timeout_get()
{
	return m_nTimeout;
	//return (uint32_t)(m_pClient->GetClientTimeOut() * 1000);
}
void SocketObject::AS3_timeout_set(uint32_t timeout)
{
	//m_timeout = timeout;
	m_nTimeout=timeout;
	//XU32 tOut = (XU32)(timeout/1000);
	//m_pClient->SetClientTimeOut(tOut);
}
//new adds end

//new adds 11
uint32_t SocketObject::AS3_bytesPending_get()
{
	//LOGWHERE();
	return m_cache.InCacheBytesAvailable();//0;
}
//new adds 11 end

}