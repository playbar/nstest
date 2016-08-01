#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3DRMDeviceGroupErrorEvent.h"
namespace avmshell
{
	DRMDeviceGroupErrorEventClass::DRMDeviceGroupErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* DRMDeviceGroupErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) DRMDeviceGroupErrorEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	DRMDeviceGroupErrorEventObject::DRMDeviceGroupErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_deviceGroup = NULL;
		m_drmUpdateNeeded = false;
		m_subErrorID = 0;
		m_systemUpdateNeeded = false;
	}
	//////////////////////////////////////////////////////////
	EventObject* DRMDeviceGroupErrorEventObject::Clone(EventObject* pObj)
	{
		if(NULL == pObj)
		{
			ClassClosure* pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_DRMDeviceGroupErrorEvent);
			pObj = (EventObject*)pClass->createInstance(pClass->ivtable(), pClass->prototype);
		}
		register DRMDeviceGroupErrorEventObject* pEvent = (DRMDeviceGroupErrorEventObject*) pObj;

		pEvent->m_deviceGroup = m_deviceGroup;
		pEvent->m_drmUpdateNeeded = m_drmUpdateNeeded;
		pEvent->m_subErrorID = m_subErrorID;
		pEvent->m_systemUpdateNeeded = m_systemUpdateNeeded;

		return ErrorEventObject::Clone(pObj);
	}

	DRMDeviceGroupObject* DRMDeviceGroupErrorEventObject::AS3_deviceGroup_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_deviceGroup;
		//return NULL;
	}
	void DRMDeviceGroupErrorEventObject::AS3_deviceGroup_set(DRMDeviceGroupObject* deviceGroup)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_deviceGroup = deviceGroup;
	}
			
	bool DRMDeviceGroupErrorEventObject::AS3_drmUpdateNeeded_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_drmUpdateNeeded;//
		//return false;
	}
			
	int  DRMDeviceGroupErrorEventObject::AS3_subErrorID_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_subErrorID;
		//return 0;
	}
	void DRMDeviceGroupErrorEventObject::AS3_subErrorID_set(int subErrorID)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_subErrorID = subErrorID;
	}
			
	bool DRMDeviceGroupErrorEventObject::AS3_systemUpdateNeeded_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_systemUpdateNeeded; //return false;
	}

	DRMDeviceGroupErrorEventObject* DRMDeviceGroupErrorEventObject::AS3_clone()
	{
		//Add your construct code here...
		//LOGWHERE();
		return (DRMDeviceGroupErrorEventObject*)Clone(NULL);
	}
	Stringp DRMDeviceGroupErrorEventObject::AS3_toString()
	{
		//Add your construct code here...
		//LOGWHERE();
		return ErrorEventObject::AS3_toString();//return NULL;
	}

}