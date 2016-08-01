#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3LastOperationStatus.h"
namespace avmshell{
LastOperationStatusClass::LastOperationStatusClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* LastOperationStatusClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LastOperationStatusObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
LastOperationStatusObject::LastOperationStatusObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}