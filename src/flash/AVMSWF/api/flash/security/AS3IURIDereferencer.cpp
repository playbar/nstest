#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IURIDereferencer.h"
namespace avmshell{
IURIDereferencerClass::IURIDereferencerClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IURIDereferencerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IURIDereferencerObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IURIDereferencerObject::IURIDereferencerObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}