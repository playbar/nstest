#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMDeviceGroup.h"
namespace avmshell
{
	DRMDeviceGroupClass::DRMDeviceGroupClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* DRMDeviceGroupClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) DRMDeviceGroupObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	DRMDeviceGroupObject::DRMDeviceGroupObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_strAuthenticationMethod = NULL;
		m_strDomain = NULL;
		m_strName = NULL;
		m_strServerURL = NULL;
	}
	//////////////////////////////////////////////////////////

	Stringp DRMDeviceGroupObject::AS3_authenticationMethod_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strAuthenticationMethod;//NULL;
	}
	
	Stringp DRMDeviceGroupObject::AS3_domain_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strDomain;//NULL;
	}
	
	Stringp DRMDeviceGroupObject::AS3_name_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strName;//NULL;
	}
	void DRMDeviceGroupObject::AS3_name_set(Stringp value)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_strName = value;
	}
	
	Stringp DRMDeviceGroupObject::AS3_serverURL_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strServerURL;//NULL;
	}
}