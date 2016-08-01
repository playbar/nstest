#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageTextInitOptions.h"
namespace avmshell
{
	StageTextInitOptionsClass::StageTextInitOptionsClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* StageTextInitOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) StageTextInitOptionsObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	StageTextInitOptionsObject::StageTextInitOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_bMultiline = false;
	}
	//////////////////////////////////////////////////////////
	
	bool StageTextInitOptionsObject::AS3_multiline_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_bMultiline;//false;
	}
		
	void StageTextInitOptionsObject::AS3_multiline_set(bool bMul)
	{
		//Add your construct code here...
		//LOGWHERE();
		m_bMultiline = bMul;
	}

}