#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3UncaughtErrorEvents.h"
namespace avmshell
{

UncaughtErrorEventsClass::UncaughtErrorEventsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* UncaughtErrorEventsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) UncaughtErrorEventsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

UncaughtErrorEventsObject* UncaughtErrorEventsClass::CreateObject()
{
	UncaughtErrorEventsObject*pObj=(UncaughtErrorEventsObject*)createInstance(ivtable(),prototype);
	return pObj;
}



UncaughtErrorEventsObject::UncaughtErrorEventsObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
//void UncaughtErrorEventsObject::AS3_constructor()
//{
//	//Add your act code here...
//}

}