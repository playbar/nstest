#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "avm2.h"
#include "splayer.h"
#include "XClientApply.h"
#include "AS3XMLSocket.h"
namespace avmshell{
XMLSocketClass::XMLSocketClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())XMLSocketObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* XMLSocketClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) XMLSocketObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
XMLSocketObject* XMLSocketClass::CreateXMLSocket(Stringp host, uint32_t port)
{
	XMLSocketObject *pObj = (XMLSocketObject *)createInstance(ivtable(), prototype);

	if(host)
		pObj->AS3_constructor(host, port);

	return pObj;
}

XMLSocketObject::XMLSocketObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)//, m_cache(toplevel())
{
	//Add your construct code here..
	m_nLoaded=0;
}

bool XMLSocketObject::OnRecieve(int nSize)
{
	if(!m_pClient)
		return false;

	//XString16 ss((char*)m_data.GetData());

	return EventDispatcherObject::OnDataEvent(XAS3_EVENT_data,(char*)m_data.GetData());//OnProgressEvent(XAS3_EVENT_socketData,nSize,0)


}

bool XMLSocketObject::OnClose(const char*msg)
{
	bool ret=EventDispatcherObject::OnClose(msg);
	Release();
	return ret;
}

void XMLSocketObject::Release()
{
	if(m_pClient)
	{
		
		if(IsRunning())
		{
			ShellCore *pShellCore = (ShellCore *)core();
			XSWFPlayer *pPlayer = (XSWFPlayer *)pShellCore->GetPlayer();
			((ShellCore*)core())->GetPlayer()->RemoveClient(m_pClient);
		}
		m_pClient->Cancel();
		m_pClient->SetWindow(NULL,0,0);
		//pPlayer->m_pClientApply->ReleaseClient(m_pClient);
		m_pClient=NULL;
	}
	EventDispatcherObject::Release();
}

//////////////////////////////////////////////////////////
//Native Method start...
bool XMLSocketObject::AS3_connected_get()
{
	//Add your act code here...
	if(m_pClient&&m_pClient->IsConnecting())
		return true;
	return false;
}

//AS3 contructor function..
void XMLSocketObject::AS3_constructor(Stringp host, uint32_t port)
{
	//Add your act code here...
	if(host)
		AS3_connect(host,port);
}

void XMLSocketObject::AS3_close()
{
	//Add your act code here...
	if(m_pClient)
	{
		m_pClient->Cancel();
	}
}

void XMLSocketObject::AS3_connect(Stringp host, uint32_t port)
{
	//Add your act code here...
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
			Release();
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
		m_pClient->Connect(req,XCHARSET_GB2312);

	}
}

void XMLSocketObject::AS3_send(Atom pObject)
{
	//Add your act code here...
	if(m_pClient)
	{
		Stringp s=core()->string(pObject);
		StUTF8String str(s);
		m_pClient->GetConnect()->Write((XU8*)str.c_str(),str.length()+1);
	}
}

//new adds
int XMLSocketObject::AS3_timeout_get()
{
	//return (uint32_t)(m_pClient->GetClientTimeOut() * 1000);
	return 30;
}
void XMLSocketObject::AS3_timeout_set(int timeout)
{
	//XU32 tOut = (XU32)(timeout/1000);
	//m_pClient->SetClientTimeOut(tOut);
}
//new adds end


}