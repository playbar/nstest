#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DigitCase.h"
namespace avmshell{
DigitCaseClass::DigitCaseClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DigitCaseClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DigitCaseObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DigitCaseObject::DigitCaseObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}