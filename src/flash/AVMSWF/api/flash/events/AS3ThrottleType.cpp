#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ThrottleType.h"
namespace avmshell
{
	ThrottleTypeClass::ThrottleTypeClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* ThrottleTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) ThrottleTypeObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	ThrottleTypeObject::ThrottleTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
	{
		//Add your construct code here...
	}
	//////////////////////////////////////////////////////////


}