#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DateTimeNameContext.h"
namespace avmshell{
DateTimeNameContextClass::DateTimeNameContextClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DateTimeNameContextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DateTimeNameContextObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DateTimeNameContextObject::DateTimeNameContextObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}