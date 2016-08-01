#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3VideoEvent.h"
namespace avmshell{
VideoEventClass::VideoEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* VideoEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) VideoEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
VideoEventObject::VideoEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Stringp VideoEventObject::AS3_status_get()
//{
//	//Add your act code here...
//	return NULL;//Modify this please!
//}
//
////AS3 contructor function..
//void VideoEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp status)
//{
//	//Add your act code here...
//}

}