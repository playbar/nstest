#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3DRMReturnVoucherErrorEvent.h"
namespace avmshell
{
	DRMReturnVoucherErrorEventClass::DRMReturnVoucherErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* DRMReturnVoucherErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) DRMReturnVoucherErrorEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	DRMReturnVoucherErrorEventObject::DRMReturnVoucherErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_licenseID = NULL;
		m_policyID = NULL;
		m_serverURL = NULL;
		m_subErrorID = 0;
	}
	//////////////////////////////////////////////////////////


	Stringp DRMReturnVoucherErrorEventObject::AS3_licenseID_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_licenseID;//NULL;
	}
	void DRMReturnVoucherErrorEventObject::AS3_licenseID_set(Stringp licenseID)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_licenseID = licenseID;
	}
	int DRMReturnVoucherErrorEventObject::AS3_subErrorID_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_subErrorID;//0;
	}
	void DRMReturnVoucherErrorEventObject::AS3_subErrorID_set(int subErrorID)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_subErrorID = subErrorID;
	}
    
	Stringp DRMReturnVoucherErrorEventObject::AS3_policyID_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_policyID;//NULL;
	}
	void DRMReturnVoucherErrorEventObject::AS3_policyID_set(Stringp policyID)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_policyID = policyID;
	}
    
	Stringp DRMReturnVoucherErrorEventObject::AS3_serverURL_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_serverURL;//NULL;
	}
	void DRMReturnVoucherErrorEventObject::AS3_serverURL_set(Stringp serverURL)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_serverURL = serverURL;
	}

	EventObject* DRMReturnVoucherErrorEventObject::Clone(EventObject* pObj)
	{
		if(NULL == pObj)
		{
			ClassClosure* pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_DRMReturnVoucherErrorEvent);
			pObj = (EventObject*) pClass->createInstance(pClass->ivtable(), pClass->prototype);
		}

		register DRMReturnVoucherErrorEventObject* pEvent = (DRMReturnVoucherErrorEventObject*) pObj;

		pEvent->m_licenseID = m_licenseID;
		pEvent->m_policyID = m_policyID;
		pEvent->m_serverURL = m_serverURL;
		pEvent->m_subErrorID = m_subErrorID;

		return ErrorEventObject::Clone(pObj);
	}

	DRMReturnVoucherErrorEventObject* DRMReturnVoucherErrorEventObject::AS3_clone()
	{
		//Add your construct code here...
		//LOGWHERE();

		return (DRMReturnVoucherErrorEventObject*)Clone(NULL);
	}
}