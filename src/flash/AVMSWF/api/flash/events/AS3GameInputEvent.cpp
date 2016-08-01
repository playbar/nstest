#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GameInputEvent.h"
namespace avmshell
{

	GameInputEventClass::GameInputEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* GameInputEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) GameInputEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	GameInputEventObject::GameInputEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_pDevice = NULL;
	}
	//////////////////////////////////////////////////////////


	GameInputDeviceObject* GameInputEventObject::AS3_device_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_pDevice;//NULL;
	}

}