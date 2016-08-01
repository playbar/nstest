#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ReturnKeyLabel.h"
namespace avmshell
{
	ReturnKeyLabelClass::ReturnKeyLabelClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* ReturnKeyLabelClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) ReturnKeyLabelObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	ReturnKeyLabelObject::ReturnKeyLabelObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}