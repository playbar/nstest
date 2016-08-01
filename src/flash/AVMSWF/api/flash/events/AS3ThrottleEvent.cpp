#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3ThrottleEvent.h"
namespace avmshell
{
	ThrottleEventClass::ThrottleEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* ThrottleEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) ThrottleEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	ThrottleEventObject::ThrottleEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_nTargetFrameRate = 0.0;
		m_strState = NULL;
	}
	//////////////////////////////////////////////////////////


	Stringp ThrottleEventObject::AS3_state_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strState;//NULL;
	}
	double ThrottleEventObject::AS3_targetFrameRate_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_nTargetFrameRate;//0;
	}

	EventObject* ThrottleEventObject::Clone(EventObject *pObj)
	{
		if (NULL == pObj)
		{
			ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_ThrottleEvent);
			pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
		}

		register ThrottleEventObject *pEvent = (ThrottleEventObject *)pObj;

		pEvent->m_pData = m_pData;
		pEvent->m_nTargetFrameRate = m_nTargetFrameRate;
		pEvent->m_strState = m_strState;

		return EventObject::Clone(pObj);
	}
	ThrottleEventObject* ThrottleEventObject::AS3_clone()
	{
		//Add your construct code here...

		return (ThrottleEventObject*)Clone(NULL);
	}
	
	Stringp ThrottleEventObject::AS3_toString()
	{
		//Add your construct code here...

		return EventObject::AS3_toString();
	}
}