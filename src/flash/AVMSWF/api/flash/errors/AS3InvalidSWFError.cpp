#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3InvalidSWFError.h"
namespace avmshell{
InvalidSWFErrorClass::InvalidSWFErrorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())InvalidSWFErrorObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* InvalidSWFErrorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) InvalidSWFErrorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
InvalidSWFErrorObject::InvalidSWFErrorObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorObject(vtable, proto)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}