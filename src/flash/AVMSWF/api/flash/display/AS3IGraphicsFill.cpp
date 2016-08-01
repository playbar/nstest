#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IGraphicsFill.h"
namespace avmshell{
IGraphicsFillClass::IGraphicsFillClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IGraphicsFillClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IGraphicsFillObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IGraphicsFillObject::IGraphicsFillObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}