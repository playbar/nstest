#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMMetadataEvent.h"
namespace avmshell
{
	DRMMetadataEventClass::DRMMetadataEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* DRMMetadataEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) DRMMetadataEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	DRMMetadataEventObject::DRMMetadataEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_drmMetadata = NULL;
		m_timestamp = 0.0;
	}
	//////////////////////////////////////////////////////////


	DRMContentDataObject* DRMMetadataEventObject::AS3_drmMetadata_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_drmMetadata;//NULL;
	}
	double DRMMetadataEventObject::AS3_timestamp_get()
	{
		//Add your construct code here...
		//LOGWHERE();
		return m_timestamp;//0;
	}


}