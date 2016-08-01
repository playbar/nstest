#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IExternalizable.h"
namespace avmshell{
IExternalizableClass::IExternalizableClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IExternalizableClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IExternalizableObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IExternalizableObject::IExternalizableObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}