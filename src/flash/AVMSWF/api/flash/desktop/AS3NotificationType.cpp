#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NotificationType.h"
namespace avmshell{
NotificationTypeClass::NotificationTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NotificationTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NotificationTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NotificationTypeObject::NotificationTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}