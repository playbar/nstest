#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GameInputControl.h"
namespace avmshell
{
	GameInputControlClass::GameInputControlClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* GameInputControlClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) GameInputControlObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	GameInputControlObject::GameInputControlObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_pDevice = NULL;
		m_strID = NULL;
		m_maxValue = 0;
		m_minValue = 0;
		m_value = 0;
	}
	//////////////////////////////////////////////////////////

	GameInputDeviceObject* GameInputControlObject::AS3_device_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_pDevice;
	}
	Stringp GameInputControlObject::AS3_id_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strID;
	}
	double GameInputControlObject::AS3_maxValue_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		
		return m_maxValue;
	}
	double GameInputControlObject::AS3_minValue_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		
		return m_minValue;
	}
	double GameInputControlObject::AS3_value_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		
		return m_value;
	}
}