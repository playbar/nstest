#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TabAlignment.h"
namespace avmshell{
TabAlignmentClass::TabAlignmentClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TabAlignmentClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TabAlignmentObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TabAlignmentObject::TabAlignmentObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}