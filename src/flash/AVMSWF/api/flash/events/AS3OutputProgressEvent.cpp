#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3OutputProgressEvent.h"
namespace avmshell{
OutputProgressEventClass::OutputProgressEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* OutputProgressEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) OutputProgressEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
OutputProgressEventObject::OutputProgressEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double OutputProgressEventObject::AS3_bytesPending_get()
//{
//	return 0;
//}
//void OutputProgressEventObject::AS3_bytesPending_set(double bytesPending)
//{
//
//}
//double OutputProgressEventObject::AS3_bytesTotal_get()
//{
//	return 0;
//}
//void OutputProgressEventObject::AS3_bytesTotal_set(double bytesTotals)
//{
//
//}
//
//void OutputProgressEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double bytesPending, double bytesTotal)
//{
//
//}
//EventObject* OutputProgressEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp OutputProgressEventObject::AS3_toString()
//{
//	return NULL;
//}

}