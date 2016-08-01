#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3KeyLocation.h"
namespace avmshell{
KeyLocationClass::KeyLocationClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())KeyLocationObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* KeyLocationClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) KeyLocationObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
KeyLocationObject::KeyLocationObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}