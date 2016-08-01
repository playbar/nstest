#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SQLTransactionLockType.h"
namespace avmshell{
SQLTransactionLockTypeClass::SQLTransactionLockTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SQLTransactionLockTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SQLTransactionLockTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SQLTransactionLockTypeObject::SQLTransactionLockTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}