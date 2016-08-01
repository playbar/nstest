#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FileMode.h"
namespace avmshell{
FileModeClass::FileModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* FileModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FileModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FileModeObject::FileModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}