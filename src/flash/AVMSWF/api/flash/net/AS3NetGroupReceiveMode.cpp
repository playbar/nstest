#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetGroupReceiveMode.h"
namespace avmshell{
NetGroupReceiveModeClass::NetGroupReceiveModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetGroupReceiveModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetGroupReceiveModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetGroupReceiveModeObject::NetGroupReceiveModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}