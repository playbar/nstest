#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "JavaScript.h"
#include "splayer.h"
#include "XDomView.h"
#include "XClientApply.h"

//Delete JavaScript for BUGS

/*namespace avmshell{
WindowClass::WindowClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	m_pWindow=(WindowObject*)createInstance(ivtable(),prototype);
	ShellToplevel*top=(ShellToplevel*)toplevel();
	NavigatorClass*pClass=top->getNavigatorClass();
	m_pNavigator=(NavigatorObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())AccessibilityObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* WindowClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) WindowObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
WindowObject::WindowObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	//XFlashView*pView=(XFlashView*)player->pWindow;
	m_pDom=player->m_pDom;
}
WindowObject* WindowClass::CreateDomWindow(void*pDom)
{
	WindowObject*obj=(WindowObject*)createInstance(ivtable(),prototype);
	obj->SetDom(pDom);
	return obj;
}

WindowObject* WindowClass::AS3_window_get(MethodEnv*env)
{
	//ShellCore*c=(ShellCore*)env->core();
	ShellToplevel*top=(ShellToplevel*)env->toplevel();
	WindowClass*pClass=top->getWindowClass();
	return pClass->m_pWindow;
}

void WindowObject::Release()
{
	//XSWFPlayer::m_pInstance->Trace(XString16("Window"));
}
//////////////////////////////////////////////////////////
//Native Method start...
void WindowObject::Connect(void*url)
{
	XURL*pURL=(XURL*)url;
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	//ShellCore*c=(ShellCore*)core();
	int nType=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	//Stringp tmp = (window != XNULL) ? (window) : (c->kEmptyString);
	XDom*pDom=(XDom*)m_pDom;
	_XLoadInfo*pInfo=player->AddLoader(pURL->m_strURL,NULL,nType,NULL,NULL,NULL);
	pInfo->strRefer=player->strURLBase;
}
void WindowObject::AS3_location_set(Stringp url)
{
	if(url==NULL) return;
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	//ShellCore*c=(ShellCore*)core();
	StUTF8String strURL(url);
	int nType=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	//Stringp tmp = (window != XNULL) ? (window) : (c->kEmptyString);
	_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),NULL,nType,NULL,NULL,NULL);
	pInfo->strRefer=player->strURLBase;
}

NavigatorObject* WindowObject::AS3_navigator_get()
{
	ShellToplevel*top=(ShellToplevel*)toplevel();
	return top->getWindowClass()->m_pNavigator;
}

LocationObject* WindowObject::AS3_location_get()
{
	if(!m_pLocation)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		m_pLocation=top->getLocationClass()->CreateLocation(this);
	}
	return m_pLocation;
	//ShellCore*c=(ShellCore*)core();
	//XSWFPlayer*player=c->GetPlayer();
	//XString8 strURL;
	//strURL.ConvertFrom(player->strURLBase.strTxt,XCHARSET_GB2312);
	//return c->newStringUTF8(strURL,strURL.GetLength());
}
/////////////////////////////////////////////////////////
NavigatorClass::NavigatorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())AccessibilityObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* NavigatorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NavigatorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NavigatorObject::NavigatorObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	ShellCore*c=(ShellCore*)core();
	//XSWFPlayer*player=c->GetPlayer();
	//XFlashView*pView=(XFlashView*)player->pWindow;
	XSWFPlayer*player=c->GetPlayer();
	this->m_strUserAgent=c->kEmptyString;
	if(player->m_pDom)
		{
			XDom*pDom=(XDom*)player->m_pDom;
			XDomView*pView=pDom->GetView();
			if(pView)
			{
				XClientApply*pApply=pView->GetClientApply();
				//XString16 s;
				//s.FromInt((int)pView->GetClientApply());
				//XSWFPlayer::m_pInstance->Trace(s);
				if(pApply)
					m_strUserAgent=c->newStringLatin1(pApply->m_info.strUserAgent);
			}
		}

}
Stringp NavigatorObject::AS3_appCodeName_get()
{
	return core()->newConstantStringLatin1("JQB");
}
Stringp NavigatorObject::AS3_appMinorVersion_get()
{
	return core()->newConstantStringLatin1("0.0.1");
}
Stringp NavigatorObject::AS3_appName_get()
{
	return core()->newConstantStringLatin1("JQB");
}
Stringp NavigatorObject::AS3_appVersion_get()
{
	return core()->newConstantStringLatin1("2");
}
Stringp NavigatorObject::AS3_browserLanguage_get()
{
	return core()->newConstantStringLatin1("zh-CN");
}
bool    NavigatorObject::AS3_cookieEnabled_get()
{
	return true;
}
Stringp NavigatorObject::AS3_cpuClass_get()
{
#ifdef _WIN32
	return core()->newConstantStringLatin1("x86");
#else
	return core()->newConstantStringLatin1("Arm");
#endif
}
bool	NavigatorObject::AS3_onLine_get()
{
	return true;
}
Stringp NavigatorObject::AS3_platform_get()
{
#ifdef _WIN32
	return core()->newConstantStringLatin1("Windows");
#elif defined(__APPLE__)
	return core()->newConstantStringLatin1("IOS");
#elif defined(__ANDROID__)
	return core()->newConstantStringLatin1("Android");
#elif defined(_SYMBIAN)
	return core()->newConstantStringLatin1("Symbian");
#else
	@Error
#endif
}
Stringp NavigatorObject::AS3_systemLanguage_get()
{
	return core()->newConstantStringLatin1("zh-CN");
}
Stringp NavigatorObject::AS3_userAgent_get()
{

	return core()->newConstantStringLatin1("JQB");
}
Stringp  NavigatorObject::AS3_userLanguage_get()
{
	return m_strUserAgent;
}
bool NavigatorObject::AS3_javaEnabled()
{
	return false;
}
bool NavigatorObject::AS3_taintEnabled()
{
	return false;
}
//////////////////////////////////////////////////////////
LocationClass::LocationClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())AccessibilityObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* LocationClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LocationObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
LocationObject::LocationObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...

}
LocationObject* LocationClass::CreateLocation(WindowObject*pObj)
{
	LocationObject*obj=(LocationObject*)createInstance(ivtable(),prototype);
	if(obj)
		obj->m_pWindow=pObj;
	return obj;
}

Stringp LocationObject::AS3_hash_get()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	XString8 str;
	str.ConvertFrom(url.m_strCard,XCHARSET_GB2312);
	return core()->newStringUTF8(str,str.GetLength());
}
void    LocationObject::AS3_hash_set(Stringp hash)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL url=pDom->GetURL0();
	if(!hash)
		url.m_strCard.Empty();
	else
	{
		StUTF8String s(hash);
		url.m_strCard=s.c_str();
		url.m_strCard.ConvertToGB();
	}
	m_pWindow->Connect(&url);
}
Stringp LocationObject::AS3_host_get()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	XString8 str;
	str.ConvertFrom(url.m_strHost,XCHARSET_GB2312);
	if(url.m_nProto<=XPROTO_FTP&&url.m_uPort!=80)
    {
		str+=XCOLON;
		str+=url.m_uPort;
    }
	return core()->newStringUTF8(str,str.GetLength());
}
void	LocationObject::AS3_host_set(Stringp host)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL url=pDom->GetURL0();
	int ddot= host->lastIndexOf(core()->cachedChars[(int)':']);
	if(ddot>0)
	{
		Stringp strHost=host->substring(0,ddot);
		StUTF8String sh(strHost);
		url.m_strHost=sh.c_str();
		url.m_strHost.ConvertToGB();
		url.m_uPort=core()->integer(host->substring(ddot+1)->atom());
	}
	else
	{
		StUTF8String sh(host);
		url.m_strHost=sh.c_str();
		url.m_strHost.ConvertToGB();
	}
	url.MakeURL();
	m_pWindow->Connect(&url);
}
Stringp LocationObject::AS3_hostname_get()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	XString8 str;
	str.ConvertFrom(url.m_strHost,XCHARSET_GB2312);
	return core()->newStringUTF8(str,str.GetLength());
}
void	LocationObject::AS3_hostname_set(Stringp hostname)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL url=pDom->GetURL0();
	StUTF8String s(hostname);
	url.m_strHost=s.c_str();
	url.m_strHost.ConvertToGB();
	url.MakeURL();
	m_pWindow->Connect(&url);
}
Stringp LocationObject::AS3_href_get()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	XString8 str;
	str.ConvertFrom(url.m_strURL,XCHARSET_GB2312);
	return core()->newStringUTF8(str,str.GetLength());
}
void	LocationObject::AS3_href_set(Stringp href)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL url=pDom->GetURL0();
	StUTF8String ss(href);
	XString8 strURL(ss.c_str(),ss.length());
	strURL.ConvertToGB();
	url.SetURL(strURL);
	m_pWindow->Connect(&url);
}
Stringp LocationObject::AS3_pathname_get()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	XString8 str;
	str.ConvertFrom(url.m_strPath,XCHARSET_GB2312);
	return core()->newStringUTF8(str,str.GetLength());
}
void	LocationObject::AS3_pathname_set(Stringp pathname)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL url=pDom->GetURL0();
	StUTF8String s(pathname);
	url.m_strPath=s.c_str();
	url.m_strPath.ConvertToGB();
	url.MakeURL();
	m_pWindow->Connect(&url);
}
Stringp LocationObject::AS3_port_get()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	return core()->intToString(url.m_uPort);
}
void	LocationObject::AS3_port_set(Stringp portname)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL url=pDom->GetURL0();
	url.m_uPort=core()->integer(portname->atom());
	url.MakeURL();
	m_pWindow->Connect(&url);
}
Stringp LocationObject::AS3_protocol_get()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	return core()->newStringLatin1(url.m_strProtocol);
}
void	LocationObject::AS3_protocol_set(Stringp protocol)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL url=pDom->GetURL0();
	StUTF8String s(protocol);
	url.m_strProtocol=s.c_str();
	url.MakeURL();
	m_pWindow->Connect(&url);
}
Stringp LocationObject::AS3_search_get()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	int id=url.m_strURL.Find('?');
	if(id<0) return core()->kEmptyString;
	else
	{
		XString8 a,args;
		url.m_strURL.Mid(a,id+1);
		args.ConvertFrom(a,XCHARSET_GB2312);
		return core()->newStringLatin1(args,args.GetLength());
	}
}
void    LocationObject::AS3_search_set(Stringp args)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL url=pDom->GetURL0();
	StUTF8String s(args);
	XString8 ar;
	ar=s.c_str();
	ar.ConvertToGB();
	int id=url.m_strFile.Find('?');
	if(id>=0)
		url.m_strFile[id]=0;
	url.m_strFile+="?";
	url.m_strFile+=ar;
	url.MakeURL();
	m_pWindow->Connect(&url);
}
void	LocationObject::AS3_assign(Stringp url)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	AS3_href_set(url);
}
void	LocationObject::AS3_reload()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	m_pWindow->Connect(&url);
}
void	LocationObject::AS3_replace(Stringp url)
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return;
	AS3_href_set(url);
}
Stringp LocationObject::AS3_toString()
{
	if(!m_pWindow||!m_pWindow->GetDom())
		return core()->kEmptyString;
	XDom*pDom=(XDom*)m_pWindow->GetDom();
	XURL&url=pDom->GetURL0();
	XString8 str;
	str.ConvertFrom(url.m_strURL,XCHARSET_GB2312);
	return core()->newStringLatin1(str,str.GetLength());
}

}*/
