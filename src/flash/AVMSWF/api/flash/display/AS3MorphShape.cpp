#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MorphShape.h"
namespace avmshell{
MorphShapeClass::MorphShapeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())MorphShapeObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* MorphShapeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MorphShapeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MorphShapeObject::MorphShapeObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}