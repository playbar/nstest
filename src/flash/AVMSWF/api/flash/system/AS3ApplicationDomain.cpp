#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ApplicationDomain.h"
#include "AS3ByteArray.h"
namespace avmshell{
ApplicationDomainClass::ApplicationDomainClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ApplicationDomainObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ApplicationDomainClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ApplicationDomainObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ApplicationDomainObject* ApplicationDomainClass::CreateAppDomain(ApplicationDomainObject*pParent,ShellCodeContext*code,Toplevel*pTop)
{
	//Atom a[]={atom(),pParent?pParent->atom():0};
	ApplicationDomainObject*pObj=(ApplicationDomainObject*)this->createInstance(ivtable(),prototype);
	//pObj->AS3_constructor(pParent);
	pObj->m_pParent=pParent;
	if(code)
		pObj->m_pDomainEnv=code->m_domainEnv;
	else
		int v=0;
	//pObj->m_pDomainEnv=code->m_domainEnv;m_pCodeContext=code;
	//pObj->m_pDomainToplevel=pTop;
	
		//(ApplicationDomainObject*)AvmCore::atomToScriptObject(construct(1,a));
	//pObj->m_pParent=pParent;
	return pObj;
}


XUINT ApplicationDomainClass::AS3_MIN_DOMAIN_MEMORY_LENGTH_get()
{
	return Domain::GLOBAL_MEMORY_MIN_SIZE;
}

ByteArrayObject* ApplicationDomainObject::AS3_domainMemory_get()
{
	/*if(m_domainMemory==NULL)
	{
		Domain*pDomain=this->m_pDomainEnv->domain();
		ByteArrayClass*pClass=((ShellToplevel*)toplevel())->getByteArrayClass();
		//ByteArrayObject*pArray=pClass->CreateArray(0);
		
	}*/
	return m_domainMemory;
}
void ApplicationDomainObject::AS3_domainMemory_set(ByteArrayObject*obj)
{
	if(m_domainMemory!=obj)
	{
		m_domainMemory=obj;
		Domain*pDomain=this->m_pDomainEnv->domain();
		//if(obj)
		//	obj->GetByteArray().m_pDomain=pDomain;
		pDomain->set_globalMemory(obj);
	}
}

ApplicationDomainObject::ApplicationDomainObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pParent=NULL;
	//m_pDomainToplevel=NULL;
	m_pDomainEnv=NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
ApplicationDomainObject* ApplicationDomainClass::AS3_currentDomain_get()
{
	//Add your act code here...
	ShellCore*c=(ShellCore*)core();
	/*MethodEnv*env=c->methodEnv();
	if(env&&env->getOverMethodEnv())
	{
		env=env->getOverMethodEnv();
		ShellCodeContext*pContext=(ShellCodeContext*)env->codeContext();
		if(pContext)
			return pContext->m_pAppDomain;
	}*/
	ShellCodeContext*pContext=(ShellCodeContext*)c->codeContext();
	if(pContext==NULL) pContext=c->GetShellContext();
	return pContext->m_pAppDomain;
	//return kAvmThunkUndefined;//Modify this please!
}

void ApplicationDomainObject::Init(ApplicationDomainObject *parentDomain)
{
	ShellCore *core = (ShellCore*) this->core();

	Domain* baseDomain;
	DomainEnv* baseEnv;
	if (parentDomain) {
		//this->m_pCodeContext=parentDomain->m_pCodeContext;
		baseDomain = parentDomain->m_pDomainEnv->domain();
		baseEnv    = parentDomain->m_pDomainEnv;
	} else {
		baseDomain = core->GetShellContext()->m_domain;
		baseEnv	   = core->GetShellContext()->m_domainEnv;
		//parentDomain= core->GetShellContext()->m_domainEnv;
			//core->builtinDomain;
	}

	/*RCObject*pDump=((ShellToplevel*)core->GetToplevel())->m_pDumpObject.value();
	ShellCodeContext* codeContext = new (core->GetGC()) ShellCodeContext(core,(ScriptObject*)pDump,m_pCore->GetGC());	
	ShellCodeContext* pp=(ShellCodeContext*)(parentDomain?parentDomain->m_pCodeContext:core->codeContext());
	if(pp==NULL)
			pp=core->GetShellContext();
	codeContext->m_pRoot=pp->m_pRoot;
	if(pCode)
		{
			codeContext->m_domain=pCode->m_domain;
			codeContext->m_domainEnv=pCode->m_domainEnv;
			codeContext->m_pAppDomain=pCode->m_pAppDomain;
			codeContext->m_pSecurityDomain=pCode->m_pSecurityDomain;
		}
		else
		{
			Domain* pDomain = new (m_pCore->GetGC()) Domain(m_pCore, pp->m_domainEnv->domain());
				
				// Return a new DomainEnv for the user code
			codeContext->m_domain=pDomain;
			codeContext->m_domainEnv = new (m_pCore->GetGC()) DomainEnv(m_pCore, pDomain, pp->m_domainEnv);
			codeContext->m_pAppDomain= ((ShellToplevel*)m_pCore->GetToplevel())->getApplicationDomainClass()->
				CreateAppDomain(pp->m_pAppDomain,codeContext,m_pCore->GetToplevel());
			codeContext->m_pSecurityDomain=
					(SecurityDomainObject*)
					(((ShellToplevel*)m_pCore->GetToplevel())->getSecurityDomainClass()->newInstance());
		}
		LoaderInfoObject*pLoaderInfo=(LoaderInfoObject*)pInfo;
		//if(pInfo)
		//	codeContext->m_pLoaderInfo=(LoaderInfoObject*)pInfo;
		//else
		if(!pLoaderInfo)
		{
			LoaderInfoClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getLoaderInfoClass();
			pLoaderInfo=pClass->CreateLoaderInfo(p,codeContext);
			PushObject(pLoaderInfo);
		}
		pLoaderInfo->SetCodeContext(codeContext);//m_pCodeContext=codeContext;
		if(pLoader)
			pLoaderInfo->m_pLoader=(LoaderObject*)pLoader;

		m_pCore->SaveContext(codeContext);
		//codeContext->m_pAppDomain->m_pLoader=(LoaderObject*)pLoader;//codeContext->m_pLoader;
		//PushObject(pLoader);
		return pLoaderInfo;*/
	
	Domain* domain = new (core->GetGC()) Domain(core, baseDomain);

	/*if (parentDomain) {
		m_pDomainToplevel = parentDomain->m_pDomainToplevel;
	} else {
		m_pDomainToplevel = core->initShellBuiltins();
	}*/
	
	m_pDomainEnv = new (core->GetGC()) DomainEnv(core, domain, baseEnv);
		//new (core->GetGC()) DomainEnv(core, domain, parentDomain ? parentDomain->m_pDomainEnv : (DomainEnv*)NULL);
}

void ApplicationDomainObject::AS3_constructor(ApplicationDomainObject*pParent)
	{
		m_pParent=pParent;		
		//	pParent=m_pCodeContext->m_pDomain;		
		//if(pParent)
		Init(pParent);
	}


static ScriptObject* _FindClass(const Multiname&multiname,DomainEnv*env,ShellToplevel*toplevel)
{
	ScriptEnv* script = (ScriptEnv*) env->getScriptInit(multiname);
	if (script == (ScriptEnv*)BIND_AMBIGUOUS)
	{
			return NULL;
	}

	if (script == (ScriptEnv*)BIND_NONE)
	{
			return NULL;
	}

	if (script->global == NULL)
	{
		script->initGlobal();
		script->coerceEnter(script->global->atom());
	}

	ScriptObject *container = script->global;
	//return container;
	if (!container) 
	{
			return NULL;
	}
	Atom atom = toplevel->getproperty(container->atom(),
											&multiname,
											container->vtable);
	
	return (ClassClosure*)AvmCore::atomToScriptObject(atom);
}


static ScriptObject* _findvec(const Stringp& name,
							 DomainEnv* domainEnv,bool bThrow,Toplevel*toplevel)
{
	//Toplevel* toplevel = core->toplevel();
	{
		//return core->FindClass(nm,code,true);
		StUTF8String str(name);
		const char* pname=str.c_str();
		int iStart= strstr(pname,".<")-pname;
		int iEnd =	strrchr(pname,'>')-pname;
		AvmCore*core=toplevel->core();
		if(iStart>0&&iEnd>iStart)
		{
				Stringp vecName=core->internStringUTF8(pname,iStart);
				Stringp typeName=core->internStringUTF8(pname+iStart+2,iEnd-iStart-2);
				Multiname vec;//(vecName);
				ShellCore::MultinameOf(vecName,vec,core);
				//vec.setName(vecName);
				ScriptObject*pClass=_FindClass(vec,domainEnv,(ShellToplevel*)toplevel);
				//ScriptEnv* pClass=domainEnv->getScriptEntry(vec);
				//ScriptEnv* script = (ScriptEnv*) domainEnv->getScriptInit(multiname);
					//FindClass(vec,env,bMust);
				if (pClass == NULL)
				{
					if(bThrow)
						toplevel->throwReferenceError(kAmbiguousBindingError, vec);
					return XNULL;
				}
				{
					Multiname type;//(multiname);
					ShellCore::MultinameOf(typeName,type,core);
					//type.setName(typeName);
					ScriptObject*pType=_FindClass(type,domainEnv,(ShellToplevel*)toplevel);
					if(pType==NULL)
					{
						if(bThrow)
							toplevel->throwReferenceError(kAmbiguousBindingError, type);
						return XNULL;
					}
					Atom args[]={pType->atom()};
					Atom atom=pClass->applyTypeArgs(1,args);
					return core->atomToScriptObject(atom);
				}
		}
	}

	return XNULL;
}

static ScriptObject* _finddef(const Multiname& multiname,
							 DomainEnv* domainEnv,bool bThrow,Toplevel*toplevel)
{
	//Toplevel* toplevel = core->toplevel();

	ScriptEnv* script = (ScriptEnv*) domainEnv->getScriptInit(multiname);
	//ScriptEnv* script = (ScriptEnv*) domainEnv->getScriptEntry(multiname);
	if (script == (ScriptEnv*)BIND_AMBIGUOUS)
	{
		if(bThrow)
			toplevel->throwReferenceError(kAmbiguousBindingError, multiname);
		else
			return false;
	}

	if (script == (ScriptEnv*)BIND_NONE)
	{
		if(bThrow)
			toplevel->throwReferenceError(kUndefinedVarError, multiname);
		else return false;
	}

	if (script->global == NULL)
	{
		//domainEnv->SetTopSeek(true);
		script->initGlobal();
		script->coerceEnter(script->global->atom());
		//domainEnv->SetTopSeek(false);
	}

	return script->global;
}

static bool _hasdef(const Multiname& multiname,
							 DomainEnv* domainEnv,Toplevel*toplevel)
{
	//Toplevel* toplevel = core->toplevel();

	ScriptEnv* script = (ScriptEnv*) domainEnv->getScriptInit(multiname);
	//ScriptEnv* script = (ScriptEnv*) domainEnv->getScriptEntry(multiname);
	if (script == (ScriptEnv*)BIND_AMBIGUOUS)
	{
		return false;
	}

	if (script == (ScriptEnv*)BIND_NONE)
	{
		return false;
	}

	return true;
}


ScriptObject* ApplicationDomainObject::findvec(Stringp name,bool bThrow)
{
	ShellCore*c=(ShellCore*)core();
	
	if(m_pDomainEnv)
	{
		return _findvec(name,m_pDomainEnv,bThrow,toplevel());
	}
	else
	{
		Multiname multiname;//(ns, className);
		ShellCore::MultinameOf(name,multiname,c);
		ShellCodeContext*pCode=c->m_pContextList;
		while(pCode)
		{
			c->doLazyPools(multiname,pCode);
			ScriptObject*obj=_findvec(name,pCode->m_domainEnv,false,toplevel());
			if(obj) return obj;
			pCode=pCode->m_pNext;
		}
	}
	//if(bThrow)
	//	toplevel()->throwReferenceError(kUndefinedVarError, multiname);
	return NULL;
}

ScriptObject* ApplicationDomainObject::finddef(const Multiname&multiname,bool bThrow)
{
	ShellCore*c=(ShellCore*)core();
	
	if(m_pDomainEnv)
	{
		return _finddef(multiname,m_pDomainEnv,bThrow,toplevel());
	}
	else
	{
		
		ShellCodeContext*pCode=c->m_pContextList;
		while(pCode)
		{
			c->doLazyPools(multiname,pCode);
			ScriptObject*obj=_finddef(multiname,pCode->m_domainEnv,false,toplevel());
			if(obj) return obj;
			pCode=pCode->m_pNext;
		}
	}
	if(bThrow)
		toplevel()->throwReferenceError(kUndefinedVarError, multiname);
	return NULL;
}


Atom ApplicationDomainObject::FindDefinition(Stringp name)
{
		AvmCore *core = this->core();

		if (name == NULL) {
			return kUnusedAtomTag;
		}
			

		// Search for a dot from the end.		

		Multiname multiname;//(ns, className);
		ShellCore::MultinameOf(name,multiname,core);

		ScriptObject *container = finddef(multiname, false);
		if (!container) {
			return kUnusedAtomTag;
		}
		Atom atom = toplevel()->getproperty(container->atom(),
											&multiname,
											container->vtable);

		//if (!AvmCore::istype(atom, core->traits.class_itraits)) {
		//	return NULL;
		//}			
		return atom;//(ClassClosure*)AvmCore::atomToScriptObject(atom);
}

Atom ApplicationDomainObject::AS3_getDefinition(Stringp name)
{
	//Add your act code here...
		AvmCore *core = this->core();

		if (name == NULL) {
			toplevel()->throwArgumentError(kNullArgumentError, "null");//core->toErrorString(name));
		}
			
		//StUTF8String s(name);
		//if(strstr(s.c_str(),"CrossDomainRSLItem")!=NULL)
		//	int v=0;
		// Search for a dot from the end.
		

		Multiname multiname;//(ns, className);
		ShellCore::MultinameOf(name,multiname,core);

		ScriptObject *container = NULL;//
		if(name->contains(core->newConstantStringLatin1(".<")))
		{
			container=findvec(name,true);
			return container->atom();
		}
		else
			container=finddef(multiname, true);
		if (!container) {
			toplevel()->throwTypeError(kClassNotFoundError, core->toErrorString(&multiname));
		}
		Atom atom = toplevel()->getproperty(container->atom(),
											&multiname,
											container->vtable);

		if (!AvmCore::istype(atom, core->traits.class_itraits)) {
			toplevel()->throwTypeError(kClassNotFoundError, core->toErrorString(&multiname));
		}			
		return atom;//(ClassClosure*)AvmCore::atomToScriptObject(atom);
}





bool ApplicationDomainObject::AS3_hasDefinition(Stringp name)
{
	//Add your act code here...
		AvmCore *core = this->core();

		if (name == NULL) {
			return false;
			//toplevel()->throwArgumentError(kNullArgumentError, core->knull);//core->toErrorString("name"));
		}
			
		// Search for a dot from the end.
		if(name->contains(core->newConstantStringLatin1(".<")))
		{
			return findvec(name,false)!=NULL;
		}
		Multiname multiname;//(ns, className);
		ShellCore::MultinameOf(name,multiname,core);

		//ScriptObject *container = finddef(multiname, false);
		ShellCore*c=(ShellCore*)core;
	
		if(m_pDomainEnv)
		{
			return _hasdef(multiname,m_pDomainEnv,toplevel());
		}
		else
		{
			
			ShellCodeContext*pCode=c->m_pContextList;
			while(pCode)
			{
				c->doLazyPools(multiname,pCode);
				if(_hasdef(multiname,pCode->m_domainEnv,toplevel()))
					return true;
				
				pCode=pCode->m_pNext;
			}
		}
		return false;

		//return container!=NULL;
}



}
