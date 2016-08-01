#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DatagramSocketDataEvent.h"
namespace avmshell{
DatagramSocketDataEventClass::DatagramSocketDataEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DatagramSocketDataEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DatagramSocketDataEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DatagramSocketDataEventObject::DatagramSocketDataEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ByteArrayObject* DatagramSocketDataEventObject::AS3_data_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return NULL;
}
void DatagramSocketDataEventObject::AS3_data_set(ByteArrayObject *pData)
{

}
Stringp DatagramSocketDataEventObject::AS3_dstAddress_get()
{
	return NULL;
}
void DatagramSocketDataEventObject::AS3_dstAddress_set(Stringp pDstAddress)
{
	
}
int DatagramSocketDataEventObject::AS3_dstPort_get()
{
	return 0;
}
void DatagramSocketDataEventObject::AS3_dstPort_set(int dstPort)
{

}
Stringp DatagramSocketDataEventObject::AS3_srcAddress_get()
{
	return NULL;
}
void DatagramSocketDataEventObject::AS3_srcAddress_set(Stringp pSrcAddress)
{

}
int DatagramSocketDataEventObject::AS3_srcPort_get()
{
	return 0;
}
void DatagramSocketDataEventObject::AS3_srcPort_set(int srcPort)
{

}

void DatagramSocketDataEventObject::AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
													  Stringp pSrcAddress, int srcPort, 
													  String pDstAddress, int dstPort, ByteArrayObject *pAata)
{

}
EventObject* DatagramSocketDataEventObject::AS3_clone()
{
	return NULL;
}
Stringp DatagramSocketDataEventObject::AS3_toString()
{
	return NULL;
}

}