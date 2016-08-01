#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SystemUpdater.h"
namespace avmshell{
SystemUpdaterClass::SystemUpdaterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SystemUpdaterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SystemUpdaterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SystemUpdaterObject::SystemUpdaterObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void SystemUpdaterObject::AS3_constructor()
{

}
void SystemUpdaterObject::AS3_cancel()
{

}
void SystemUpdaterObject::AS3_update(Stringp pType)
{

}
}