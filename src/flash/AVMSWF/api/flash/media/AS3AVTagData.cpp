#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AVTagData.h"
namespace avmshell
{
	AVTagDataClass::AVTagDataClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* AVTagDataClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) AVTagDataObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	AVTagDataObject::AVTagDataObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_strData = NULL;
		m_nLocalTime = 0;
	}
	//////////////////////////////////////////////////////////
	Stringp AVTagDataObject::AS3_data_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strData;//NULL;
	}
	double AVTagDataObject::AS3_localTime_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_nLocalTime;//0.0;
	}

	void AVTagDataObject::AS3_constructor(Stringp data, double localTime)
	{
		m_strData = data;
		m_nLocalTime = localTime;
	}

}