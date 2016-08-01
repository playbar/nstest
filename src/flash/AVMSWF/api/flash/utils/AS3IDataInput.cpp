#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IDataInput.h"
namespace avmshell{
IDataInputClass::IDataInputClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IDataInputClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IDataInputObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IDataInputObject::IDataInputObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}