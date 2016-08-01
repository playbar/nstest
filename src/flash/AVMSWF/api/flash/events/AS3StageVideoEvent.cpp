#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StageVideoEvent.h"
namespace avmshell{
StageVideoEventClass::StageVideoEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StageVideoEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StageVideoEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StageVideoEventObject::StageVideoEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Stringp StageVideoEventObject::AS3_colorSpace_get()
//{
//	//Add your act code here...
//	return NULL;//Modify this please!
//}
//
//Stringp StageVideoEventObject::AS3_status_get()
//{
//	//Add your act code here...
//	return NULL;//Modify this please!
//}
//
////AS3 contructor function..
//void StageVideoEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pStatus, Stringp pColorSpace)
//{
//	//Add your act code here...
//}

}