#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetStreamPlayTransitions.h"
namespace avmshell{
NetStreamPlayTransitionsClass::NetStreamPlayTransitionsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetStreamPlayTransitionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetStreamPlayTransitionsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetStreamPlayTransitionsObject::NetStreamPlayTransitionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}