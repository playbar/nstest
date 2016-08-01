#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3PixelSnapping.h"
namespace avmshell{
PixelSnappingClass::PixelSnappingClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())PixelSnappingObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* PixelSnappingClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) PixelSnappingObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
PixelSnappingObject::PixelSnappingObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}