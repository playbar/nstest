#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetGroupReplicationStrategy.h"
namespace avmshell{
NetGroupReplicationStrategyClass::NetGroupReplicationStrategyClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetGroupReplicationStrategyClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetGroupReplicationStrategyObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetGroupReplicationStrategyObject::NetGroupReplicationStrategyObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}