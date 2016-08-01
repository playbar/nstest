#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsPathCommand.h"
namespace avmshell{
GraphicsPathCommandClass::GraphicsPathCommandClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsPathCommandClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsPathCommandObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsPathCommandObject::GraphicsPathCommandObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}