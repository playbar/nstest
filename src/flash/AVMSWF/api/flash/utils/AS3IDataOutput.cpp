#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IDataOutput.h"
namespace avmshell{
IDataOutputClass::IDataOutputClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IDataOutputClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IDataOutputObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IDataOutputObject::IDataOutputObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}