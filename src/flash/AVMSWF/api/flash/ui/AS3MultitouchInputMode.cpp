#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MultitouchInputMode.h"
namespace avmshell{
MultitouchInputModeClass::MultitouchInputModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MultitouchInputModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MultitouchInputModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MultitouchInputModeObject::MultitouchInputModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}