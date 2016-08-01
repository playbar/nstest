#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GameInput.h"
namespace avmshell
{
	GameInputClass::GameInputClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
		m_bSupported = false;
		m_numDevices = 0;
	};
	ScriptObject* GameInputClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) GameInputObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	GameInputObject::GameInputObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
	{
		//Add your construct code here...

	}
	//////////////////////////////////////////////////////////
	bool GameInputClass::AS3_isSupported_get()
	{
		//Add your construct code here...
		//LOGWHERE();
	
		return m_bSupported;
	}
	int GameInputClass::AS3_numDevices_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_numDevices;
	}

	GameInputDeviceObject* GameInputClass::AS3_getDeviceAt(int index)
	{
		//Add your construct code here...
		LOGWHERE();

		return NULL;
	}

}