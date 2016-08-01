#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3WorkerDomain.h"
namespace avmshell
{
	WorkerDomainClass::WorkerDomainClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
		//m_pCurrent = (WorkerDomainObject*)createInstance(ivtable(),prototype);
		m_bSupported = true;
	};
	ScriptObject* WorkerDomainClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) WorkerDomainObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	WorkerDomainObject::WorkerDomainObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////

	WorkerDomainObject* WorkerDomainClass::AS3_current_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		//ShellCore*c=(ShellCore*)core();
		//ShellCodeContext*pContext=(ShellCodeContext*)c->codeContext();
		//if(pContext==NULL) pContext=c->GetShellContext();
		//return pContext->m_pWorkerDomain;
		return (WorkerDomainObject*)createInstance(ivtable(), prototype);
	}
	bool WorkerDomainClass::AS3_isSupported_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_bSupported;
	}

	WorkerObject* WorkerDomainObject::AS3_createWorker(ByteArrayObject* swf, bool giveAppPrivileges)
	{
		//Add your construct code here...
		LOGWHERE();

		return NULL;
	}
	ObjectVectorObject* WorkerDomainObject::AS3_listWorkers()
	{
		//Add your construct code here...
		LOGWHERE();

		return NULL;
	}
}