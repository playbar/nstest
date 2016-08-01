#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SoftKeyboardType.h"
namespace avmshell
{
	SoftKeyboardTypeClass::SoftKeyboardTypeClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* SoftKeyboardTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) SoftKeyboardTypeObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	SoftKeyboardTypeObject::SoftKeyboardTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}