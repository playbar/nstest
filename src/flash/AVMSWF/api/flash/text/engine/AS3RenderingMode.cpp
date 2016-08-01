#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3RenderingMode.h"
namespace avmshell{
RenderingModeClass::RenderingModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* RenderingModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) RenderingModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
RenderingModeObject::RenderingModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}