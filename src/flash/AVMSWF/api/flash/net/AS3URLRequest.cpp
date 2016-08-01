#include "StdAfxflash.h"
#include "ShellCore.h"
#include "stags.h"
#include "splay.h"
#include "splayer.h"

using namespace avmplus;
#include "AS3URLRequest.h"
#include "AS3ByteArray.h"
#include "AS3URLVariables.h"

namespace avmshell{
URLRequestClass::URLRequestClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//m_aliasTable=new (core()->GetGC()) EventHashTable(4);
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())URLRequestObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};

/*ClassClosure* URLRequestClass::getClassByAlias(AvmMethodEnv env,Stringp alias)
{
	ShellCore*c=(ShellCore*)env->core();
	URLRequestClass*pClass=((ShellToplevel*)c->GetToplevel())->getURLRequestClass();
	//Namespacep ns=c->getPublicNamespace(0);
	Binding b=pClass->m_aliasTable->get(alias,pClass);
	if(b==BIND_NONE) return NULL;
	return (ClassClosure*)b;
}*/
void URLRequestClass::navigateToURL(AvmMethodEnv env,URLRequestObject* req,Stringp window)
{
	ShellCore*c=(ShellCore*)env->core();
	XSWFPlayer*player=c->GetPlayer();
	//ShellCore*c=(ShellCore*)core();
	StUTF8String strURL(req->m_strURL);
	XU8Array datas;
	Stringp strType=req->GetPostData(datas,false);
	if(strType==NULL) strType=req->m_strContext;
	int nType=0;
	if(req->m_nMethod==URLRequestMethodClass::POST)
		nType|=kHttpSendUsePost;
	else
		nType|=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	//Stringp tmp = (window != XNULL) ? (window) : (c->kEmptyString);
	Stringp tmp = c->kEmptyString;
	StUTF8String target( (window != XNULL) ? window : tmp );
	const char*targets = target.c_str();
	_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),targets,nType,NULL,NULL,strType?StUTF8String(strType).c_str():NULL);
	pInfo->strRefer=player->strURLBase;
	if(datas.GetSize())
		pInfo->postData.Append(datas);
}

void URLRequestClass::sendToURL(AvmMethodEnv env,URLRequestObject*req)
{
	ShellCore*c=(ShellCore*)env->core();
	XSWFPlayer*player=c->GetPlayer();
	//ShellCore*c=(ShellCore*)core();
	StUTF8String strURL(req->m_strURL);
	XU8Array datas;
	Stringp strType=req->GetPostData(datas,false);
	if(strType==NULL) strType=req->m_strContext;
	int nType=kHttpNoResponse;
	if(req->m_nMethod==URLRequestMethodClass::POST)
		nType|=kHttpSendUsePost;
	else
		nType|=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),NULL,nType,NULL,NULL,strType?StUTF8String(strType).c_str():NULL);
	pInfo->strRefer=player->strURLBase;
	if(datas.GetSize())
		pInfo->postData.Append(datas);
}

ScriptObject* URLRequestClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) URLRequestObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
URLRequestObject::URLRequestObject(VTable *vtable, ScriptObject* proto, int capacity): 
	ScriptObject(vtable, proto, 0),m_pURLRequestDefaultsClass(((ShellToplevel *)toplevel())->getURLRequestDefaultsClass())
{
	//Add your construct code here...
	m_nMethod=URLRequestMethodClass::GET;
	m_strURL=NULL;
	m_headers=NULL;
	m_strContext=NULL;
	m_pData=nullObjectAtom;
}


ArrayObject* URLRequestObject::AS3_requestHeaders_get()
{
	if(m_headers==NULL)
	{
		m_headers=toplevel()->arrayClass->newArray(0);
	}
	return m_headers;
}
//////////////////////////////////////////////////////////
//Native Method start...

Stringp URLRequestObject::AS3_method_get()
{
	//Add your act code here...
	URLRequestMethodClass*pClass=((ShellToplevel*)toplevel())->getURLRequestMethodClass();
	return m_nMethod==URLRequestMethodClass::POST?pClass->getSlotPOST():pClass->getSlotGET();
}

void URLRequestObject::AS3_method_set(Stringp s)
{
	//Add your act code here...
	URLRequestMethodClass*pClass=((ShellToplevel*)toplevel())->getURLRequestMethodClass();
	if(s->equals(pClass->getSlotPOST()))
		m_nMethod=URLRequestMethodClass::POST;
	else
		m_nMethod=URLRequestMethodClass::GET;
}

Stringp URLRequestObject::GetPostData(XU8Array&data,bool bFileRef)
{
	if(!AvmCore::isNullOrUndefined(m_pData))
	{
		AvmCore*c=core();
		
		switch(atomKind(m_pData))
		{
		default:
			 {
			   Stringp s=c->string(m_pData);
			   if(s)
			   {
				   StUTF8String us(s);
				   if(us.length())
					   data.Append((XU8*)us.c_str(),us.length());
			   }
			 }
			 break;
		case kStringType:
			{
				Stringp s=c->atomToString(m_pData);
				if(s)
				{
					StUTF8String us(s);
					if(us.length())
						data.Append((XU8*)us.c_str(),us.length());
				}
			}
			 break;
		case kObjectType:
			{
				ShellToplevel*p=(ShellToplevel*)toplevel();
				ByteArrayClass*pClass=p->getByteArrayClass();
				ScriptObject*pObject=AvmCore::atomToScriptObject(m_pData);
				//if(ShellCore::IsProtoTypeOf(pClass,pObject))
				if(ShellCore::IsTypeClass(pObject,pClass))
				{
					ByteArrayObject*pObj=(ByteArrayObject*)pObject;
					if(pObj->GetLength())
					{
						data.SetSize(pObj->GetLength());
						//pObj->ReadBytes(data.GetData(),pObj->GetLength());
						VMPI_memcpy(data.GetData(),pObj->GetByteArray().GetBuffer(),pObj->GetLength());
						m_nMethod=URLRequestMethodClass::POST;
					}
					return m_strContext;
				}
				else
				{
					Stringp cnt=m_strContext;
					if(m_nMethod==URLRequestMethodClass::POST)
						cnt=c->newConstantStringLatin1("application/x-www-form-urlencoded");
					else if(bFileRef)
						cnt=c->newConstantStringLatin1("multipart/form-data");
					Stringp strTxt=c->string(m_pData);
					StUTF8String s(strTxt);
					data.Append((XU8*)s.c_str(),s.length());
					return cnt;
				}
			}break;
		}
	}
	return this->m_strContext;
}

//new adds 11
void URLRequestObject::AS3_useRedirectedURL(URLRequestObject* pSourceRequest, bool wholeURL, void* pattern, Stringp sReplace)
{
	LOGWHERE();
}

//new adds 11 end
}
