#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TypographicCase.h"
namespace avmshell{
TypographicCaseClass::TypographicCaseClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TypographicCaseClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TypographicCaseObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TypographicCaseObject::TypographicCaseObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}