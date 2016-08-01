#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsPathWinding.h"
namespace avmshell{
GraphicsPathWindingClass::GraphicsPathWindingClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsPathWindingClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsPathWindingObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsPathWindingObject::GraphicsPathWindingObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}