#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AVM1Movie.h"
namespace avmshell{
AVM1MovieClass::AVM1MovieClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())AVM1MovieObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* AVM1MovieClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) AVM1MovieObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
AVM1MovieObject::AVM1MovieObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}