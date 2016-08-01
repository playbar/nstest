#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IIMEClient.h"
namespace avmshell{
IIMEClientClass::IIMEClientClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IIMEClientClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IIMEClientObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IIMEClientObject::IIMEClientObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}