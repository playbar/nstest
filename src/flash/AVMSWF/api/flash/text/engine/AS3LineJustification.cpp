#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3LineJustification.h"
namespace avmshell{
LineJustificationClass::LineJustificationClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* LineJustificationClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LineJustificationObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
LineJustificationObject::LineJustificationObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}