#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SampleDataEvent.h"
namespace avmshell{
SampleDataEventClass::SampleDataEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SampleDataEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SampleDataEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SampleDataEventObject::SampleDataEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//ByteArrayObject* SampleDataEventObject::AS3_data_get()
//{
//	return NULL;
//}
//void SampleDataEventObject::AS3_data_set(ByteArrayObject *pData)
//{
//
//}
//double SampleDataEventObject::AS3_position_get()
//{
//	return 0;
//}
//void SampleDataEventObject::AS3_position_set(double position)
//{
//
//}
//
//void SampleDataEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double theposition, ByteArrayObject *pThedata)
//{
//
//}
//EventObject* SampleDataEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp SampleDataEventObject::AS3_toString()
//{
//	return NULL;
//}

}