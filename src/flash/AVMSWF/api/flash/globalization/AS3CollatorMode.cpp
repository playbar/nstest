#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CollatorMode.h"
namespace avmshell{
CollatorModeClass::CollatorModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* CollatorModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CollatorModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CollatorModeObject::CollatorModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
