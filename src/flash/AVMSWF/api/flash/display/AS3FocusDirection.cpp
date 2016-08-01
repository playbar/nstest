#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FocusDirection.h"
namespace avmshell{
FocusDirectionClass::FocusDirectionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* FocusDirectionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FocusDirectionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FocusDirectionObject::FocusDirectionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}