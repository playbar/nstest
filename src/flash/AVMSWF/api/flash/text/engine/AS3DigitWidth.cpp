#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DigitWidth.h"
namespace avmshell{
DigitWidthClass::DigitWidthClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DigitWidthClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DigitWidthObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DigitWidthObject::DigitWidthObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}