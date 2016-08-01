#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IGraphicsData.h"
namespace avmshell{
IGraphicsDataClass::IGraphicsDataClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IGraphicsDataClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IGraphicsDataObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IGraphicsDataObject::IGraphicsDataObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}