#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GameInputDevice.h"
namespace avmshell
{
	GameInputDeviceClass::GameInputDeviceClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* GameInputDeviceClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) GameInputDeviceObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	GameInputDeviceObject::GameInputDeviceObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_bEnabled = false;
		m_strID = NULL;
		m_strName = NULL;
		m_numControls = 0;
		m_sampleInterval = 0;
	}
	//////////////////////////////////////////////////////////
	bool GameInputDeviceObject::AS3_enabled_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_bEnabled;//false;
	}
	void GameInputDeviceObject::AS3_enabled_set(bool bEnabled)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_bEnabled = bEnabled;
	}
    
	Stringp GameInputDeviceObject::AS3_id_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strID;//NULL;
	}
    
	Stringp GameInputDeviceObject::AS3_name_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strName;//NULL;
	}
    
	int GameInputDeviceObject::AS3_numControls_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_numControls;//0;
	}
    
	int GameInputDeviceObject::AS3_sampleInterval_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_sampleInterval;//0;
	}
	void GameInputDeviceObject::AS3_sampleInterval_set(int nSam)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_sampleInterval = nSam;
	}
	
	
	int GameInputDeviceObject::AS3_getCachedSamples(ByteArrayObject* data, bool append)
	{
		//Add your construct code here...
		LOGWHERE();

		return 0;
	}
	
	GameInputControlObject* GameInputDeviceObject::AS3_getControlAt(int i)
	{
		//Add your construct code here...
		LOGWHERE();

		return NULL;
	}
	
	void GameInputDeviceObject::AS3_startCachingSamples(int numSamples, StringpVectorObject* controls)
	{
		//Add your construct code here...
		LOGWHERE();

	}
	
	void GameInputDeviceObject::AS3_stopCachingSamples()
	{
		//Add your construct code here...
		LOGWHERE();

	}

}