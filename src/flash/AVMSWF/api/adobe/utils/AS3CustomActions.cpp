#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CustomActions.h"
namespace avmshell{
CustomActionsClass::CustomActionsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())CustomActionsObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* CustomActionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CustomActionsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CustomActionsObject::CustomActionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* CustomActionsClass::AS3_actionsList_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp CustomActionsClass::AS3_getActions(Stringp name)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void CustomActionsClass::AS3_installActions(Stringp name, Stringp data)
{
	//Add your act code here...
}

void CustomActionsClass::AS3_uninstallActions(Stringp name)
{
	//Add your act code here...
}

}