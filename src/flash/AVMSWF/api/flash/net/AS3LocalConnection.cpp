#include "StdAfxflash.h"
#include "splay.h"
#include "splayer.h"
#include "XURL.h"
#include "ShellCore.h"
#include "avm2.h"
using namespace avmplus;
#include "AS3LocalConnection.h"
#include "AS3StatusEvent.h"

namespace avmshell{
LocalConnectionClass::LocalConnectionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())LocalConnectionObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* LocalConnectionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LocalConnectionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

LocalConnectionObject *LocalConnectionClass::CreateLocalConnection()
{
	LocalConnectionObject *pObj = (LocalConnectionObject *)createInstance(ivtable(), prototype);

	pObj->AS3_constructor();

	return pObj;
}

LocalConnectionObject::LocalConnectionObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	AvmCore*c=core();
	//ShellCodeContext*pContext=(ShellCodeContext*)c->codeContext();
	if(m_codeContext)
	{
		ScriptPlayer*p=m_codeContext->m_pRoot;//((ShellCore*)core())->GetScriptPlayer();//pContext->m_pRoot;
		char*url=p->m_url.strTxt;
		XURL u;
		u.SetURL(url);
		if(u.m_strHost.IsEmpty())
			m_domain=c->newConstantStringLatin1("localhost");
		else
			m_domain=c->newStringLatin1(u.m_strHost);
	}
}
//////////////////////////////////////////////////////////
//Native Method start...
Atom LocalConnectionObject::AS3_client_get()
{
	//Add your act code here...
	if(m_pClient)
		return m_pClient->atom();
	else
		return nullObjectAtom;
}

void LocalConnectionObject::AS3_client_set(Atom obj)
{
	//Add your act code here...
	m_pClient = core()->atomToScriptObject(obj);
}

Stringp LocalConnectionObject::AS3_domain_get()
{
	//Add your act code here...
	return m_domain;
}

//AS3 contructor function..
void LocalConnectionObject::AS3_constructor()
{
	//Add your act code here...
}

void LocalConnectionObject::AS3_allowDomain(ArrayObject*)
{
	//Add your act code here...
}

void LocalConnectionObject::AS3_allowInsecureDomain(ArrayObject*)
{
	//Add your act code here...
}

void LocalConnectionObject::AS3_close()
{
	//Add your act code here...
	ShellCore*c=(ShellCore*)core();
	c->GetPlayer()->AS3Close(this);
}

void LocalConnectionObject::AS3_connect(Stringp pConnectionName)
{
	//Add your act code here...
	ShellCore*c=(ShellCore*)core();
	StUTF8String s(pConnectionName);
	c->GetPlayer()->AS3Connect(s.c_str(),this,XAS3CON_LOCAL,m_codeContext);
}

void LocalConnectionObject::AS3_send(Stringp name,Stringp method,ArrayObject*args)
{
	//Add your act code here...
	ShellCore*c=(ShellCore*)core();
	StUTF8String n(name);
	StUTF8String m(method);
	bool bOK=c->GetPlayer()->AS3Send(n.c_str(),m.c_str(),args);
	OnStatusEvent(c->knull,bOK?StatusEventObject::LV_STATUS:StatusEventObject::LV_ERROR);
	//Stringp pCode=c->knull;//c->newConstantStringLatin1("0");
	//Stringp pStatus=bOK?c->newConstantStringLatin1("status"):c->newConstantStringLatin1("error");
	//EventObject*pEvent=((ShellToplevel*)toplevel())->getStatusEventClass()->CreateEventObject(XAS3_EVENT_status,NULL,pStatus);
	//pEvent->pTarget=this;
	//AS3_dispatchEvent(pEvent);
}

bool LocalConnectionObject::SendMethod(const char*name,const char*strDomain,const char*method,ArrayObject*args)
{
	AvmCore*c=core();
	ScriptObject*pClient=m_pClient;
	if(pClient==NULL) pClient=this;
	//pClient->callProperty(
	//callProperty
	Stringp s=c->internStringLatin1(method);
	if(!pClient->hasAtomProperty(s->atom())) return false;
	Atom atom=pClient->getAtomProperty(s->atom());
	args->setIntProperty(0,atom);
	if(atomKind(atom)==kObjectType)
	{
		ScriptObject*pCall=c->atomToScriptObject(atom);
		if(pCall)
		{
			//pCall->call(args->get_length()-1,args->GetAtoms());
			pCall->call(args->getDenseLength()-1,args->GetAtoms());
			return true;
		}
	}
	return true;
}

//new adds
bool LocalConnectionObject::AS3_isPerUser_get()
{
	return m_isPerUser;
}
void LocalConnectionObject::AS3_isPerUser_set(bool isPerUser)
{
	m_isPerUser = isPerUser;
}
//new adds end


}