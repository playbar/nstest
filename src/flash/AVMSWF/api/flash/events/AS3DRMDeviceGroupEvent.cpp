#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3DRMDeviceGroupEvent.h"
namespace avmshell
{
	DRMDeviceGroupEventClass::DRMDeviceGroupEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* DRMDeviceGroupEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) DRMDeviceGroupEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	DRMDeviceGroupEventObject::DRMDeviceGroupEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_deviceGroup = NULL;
	}
	//////////////////////////////////////////////////////////


	DRMDeviceGroupObject* DRMDeviceGroupEventObject::AS3_deviceGroup_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		//if(m_deviceGroup == NULL)
		//{
		//	DRMDeviceGroupClass* pClass = ((ShellToplevel*)toplevel())->getDRMDeviceGroupClass();
		//	m_deviceGroup = (DRMDeviceGroupObject*)pClass->createInstance(pClass->ivtable(),pClass->prototype);
		//}
		return m_deviceGroup;//return NULL;
	}
	void DRMDeviceGroupEventObject::AS3_deviceGroup_set(DRMDeviceGroupObject* deviceGroup)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_deviceGroup = deviceGroup;
	}
	EventObject* DRMDeviceGroupEventObject::Clone(EventObject* pObj)
	{
		if(NULL == pObj)
		{
			ClassClosure* pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_DRMDeviceGroupEvent);
			pObj = (EventObject*)pClass->createInstance(pClass->ivtable(), pClass->prototype);
		}
		register DRMDeviceGroupEventObject* pEvent = (DRMDeviceGroupEventObject*)pObj;

		pEvent->m_deviceGroup = m_deviceGroup;

		return EventObject::Clone(pObj);
	}
	DRMDeviceGroupEventObject* DRMDeviceGroupEventObject::AS3_clone()
	{
		//Add your construct code here...
		//LOGWHERE();
		return (DRMDeviceGroupEventObject*)Clone(NULL);//NULL;
	}
	Stringp DRMDeviceGroupEventObject::AS3_toString()
	{
		//Add your construct code here...
		//LOGWHERE();
		return EventObject::AS3_toString();//return NULL;
	}

}