#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3DRMReturnVoucherCompleteEvent.h"
namespace avmshell
{
	DRMReturnVoucherCompleteEventClass::DRMReturnVoucherCompleteEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* DRMReturnVoucherCompleteEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) DRMReturnVoucherCompleteEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	DRMReturnVoucherCompleteEventObject::DRMReturnVoucherCompleteEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		Stringp m_licenseID = NULL;
		int m_numberOfVouchersReturned = 0;
		Stringp m_policyID = NULL;
		Stringp m_serverURL = NULL;
	}
	//////////////////////////////////////////////////////////


	Stringp DRMReturnVoucherCompleteEventObject::AS3_licenseID_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_licenseID;//NULL;
	}
	void DRMReturnVoucherCompleteEventObject::AS3_licenseID_set(Stringp licenseID)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_licenseID = licenseID;
	}
	int DRMReturnVoucherCompleteEventObject::AS3_numberOfVouchersReturned_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_numberOfVouchersReturned;//0;
	}
	void DRMReturnVoucherCompleteEventObject::AS3_numberOfVouchersReturned_set(int numOfVR)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_numberOfVouchersReturned = numOfVR;
	}
    
	Stringp DRMReturnVoucherCompleteEventObject::AS3_policyID_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_policyID;//NULL;
	}
	void DRMReturnVoucherCompleteEventObject::AS3_policyID_set(Stringp policyID)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_policyID = policyID;
	}
    
	Stringp DRMReturnVoucherCompleteEventObject::AS3_serverURL_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_serverURL;//NULL;
	}
	void DRMReturnVoucherCompleteEventObject::AS3_serverURL_set(Stringp serverURL)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_serverURL = serverURL;
	}

	EventObject* DRMReturnVoucherCompleteEventObject::Clone(EventObject*pObj)
	{
		if(NULL == pObj)
		{
			ClassClosure* pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_DRMReturnVoucherCompleteEvent);
			pObj = (EventObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		}

		register DRMReturnVoucherCompleteEventObject* pEvent = (DRMReturnVoucherCompleteEventObject*)pObj;

		pEvent->m_licenseID = m_licenseID;
		pEvent->m_numberOfVouchersReturned = m_numberOfVouchersReturned;
		pEvent->m_policyID = m_policyID;
		pEvent->m_serverURL = m_serverURL;

		return EventObject::Clone(pObj);
	}

	DRMReturnVoucherCompleteEventObject* DRMReturnVoucherCompleteEventObject::AS3_clone()
	{
		//Add your construct code here...
		//LOGWHERE();

		return (DRMReturnVoucherCompleteEventObject*)Clone(NULL);//NULL;
	}
}