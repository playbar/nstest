#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IGraphicsStroke.h"
namespace avmshell{
IGraphicsStrokeClass::IGraphicsStrokeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IGraphicsStrokeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IGraphicsStrokeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IGraphicsStrokeObject::IGraphicsStrokeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}