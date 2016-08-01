#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextLineValidity.h"
namespace avmshell{
TextLineValidityClass::TextLineValidityClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextLineValidityClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextLineValidityObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextLineValidityObject::TextLineValidityObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}