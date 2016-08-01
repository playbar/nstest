#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3DRMLicenseRequestEvent.h"
namespace avmshell
{
	DRMLicenseRequestEventClass::DRMLicenseRequestEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* DRMLicenseRequestEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) DRMLicenseRequestEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	DRMLicenseRequestEventObject::DRMLicenseRequestEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_serverURL = NULL;
	}
	//////////////////////////////////////////////////////////


	Stringp DRMLicenseRequestEventObject::AS3_serverURL_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_serverURL;//return NULL;
	}
	void DRMLicenseRequestEventObject::AS3_serverURL_set(Stringp serverURL)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_serverURL = serverURL;
	}


	EventObject* DRMLicenseRequestEventObject::Clone(EventObject*pObj)
	{
		if(NULL == pObj)
		{
			ClassClosure* pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_DRMLicenseRequestEvent);
			pObj = (EventObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		}

		register DRMLicenseRequestEventObject* pEvent = (DRMLicenseRequestEventObject*)pObj;

		pEvent->m_serverURL = m_serverURL;

		return EventObject::Clone(pObj);
	}

	DRMLicenseRequestEventObject* DRMLicenseRequestEventObject::AS3_clone()
	{
		//Add your construct code here...
		//LOGWHERE();
		return (DRMLicenseRequestEventObject*)Clone(NULL);//NULL;
	}

}