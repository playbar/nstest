#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetStreamMulticastInfo.h"
namespace avmshell{
NetStreamMulticastInfoClass::NetStreamMulticastInfoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetStreamMulticastInfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetStreamMulticastInfoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetStreamMulticastInfoObject::NetStreamMulticastInfoObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double NetStreamMulticastInfoObject::AS3_bytesPushedFromPeers_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_bytesPushedToPeers_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_bytesReceivedFromIPMulticast_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_bytesReceivedFromServer_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_bytesRequestedByPeers_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_bytesRequestedFromPeers_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_fragmentsPushedFromPeers_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_fragmentsPushedToPeers_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_fragmentsReceivedFromIPMulticast_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_fragmentsReceivedFromServer_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_fragmentsRequestedByPeers_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_fragmentsRequestedFromPeers_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_receiveControlBytesPerSecond_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_receiveDataBytesPerSecond_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_receiveDataBytesPerSecondFromIPMulticast_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_receiveDataBytesPerSecondFromServer_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_sendControlBytesPerSecond_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_sendControlBytesPerSecondToServer_get()
//{
//	return 0;
//}
//double NetStreamMulticastInfoObject::AS3_sendDataBytesPerSecond_get()
//{
//	return 0;
//}
//
//Stringp NetStreamMulticastInfoObject::AS3_toString()
//{
//	return NULL;
//}

}