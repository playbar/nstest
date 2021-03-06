#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TouchscreenType.h"
namespace avmshell{
TouchscreenTypeClass::TouchscreenTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TouchscreenTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TouchscreenTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TouchscreenTypeObject::TouchscreenTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}