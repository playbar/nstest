#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetGroupInfo.h"
namespace avmshell{
NetGroupInfoClass::NetGroupInfoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetGroupInfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetGroupInfoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetGroupInfoObject::NetGroupInfoObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double NetGroupInfoObject::AS3_objectReplicationReceiveBytesPerSecond_get()
//{
//	return 0;
//}
//double NetGroupInfoObject::AS3_objectReplicationSendBytesPerSecond_get()
//{
//	return 0;
//}
//double NetGroupInfoObject::AS3_postingReceiveControlBytesPerSecond_get()
//{
//	return 0;
//}
//double NetGroupInfoObject::AS3_postingReceiveDataBytesPerSecond_get()
//{
//	return 0;
//}
//double NetGroupInfoObject::AS3_postingSendControlBytesPerSecond_get()
//{
//	return 0;
//}
//double NetGroupInfoObject::AS3_postingSendDataBytesPerSecond_get()
//{
//	return 0;
//}
//double NetGroupInfoObject::AS3_routingReceiveBytesPerSecond_get()
//{
//	return 0;
//}
//double NetGroupInfoObject::AS3_routingSendBytesPerSecond_get()
//{
//	return 0;
//}
//
//Stringp NetGroupInfoObject::AS3_toString()
//{
//	return NULL;
//}

}