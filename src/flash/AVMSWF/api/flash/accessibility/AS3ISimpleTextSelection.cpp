#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ISimpleTextSelection.h"
namespace avmshell{
ISimpleTextSelectionClass::ISimpleTextSelectionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ISimpleTextSelectionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ISimpleTextSelectionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ISimpleTextSelectionObject::ISimpleTextSelectionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}