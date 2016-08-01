#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NetGroup.h"
namespace avmshell{
NetGroupClass::NetGroupClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NetGroupClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetGroupObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NetGroupObject::NetGroupObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double NetGroupObject::AS3_estimatedMemberCount_get()
//{
//	return 0;
//}
//double NetGroupObject::AS3_neighborCount_get()
//{
//	return 0;
//}
//Stringp NetGroupObject::AS3_receiveMode_get()
//{
//	return NULL;
//}
//void NetGroupObject::AS3_receiveMode_set(Stringp pReceiveMode)
//{
//
//}
//Stringp NetGroupObject::AS3_replicationStrategy_get()
//{
//	return NULL;
//}
//void NetGroupObject::AS3_replicationStrategy_set(Stringp pReplicationStrategy)
//{
//
//}
//void NetGroupObject::AS3_addHaveObjects(double startIndex, double endIndex)
//{
//
//}
//bool NetGroupObject::AS3_addMemberHint(Stringp pPeerID)
//{
//	return false;
//}
//bool NetGroupObject::AS3_addNeighbor(Stringp pPeerID)
//{
//	return false;
//}
//void NetGroupObject::AS3_addWantObjects(double startIndex, double endIndex)
//{
//
//}
//void NetGroupObject::AS3_close()
//{
//
//}
//void NetGroupObject::AS3_denyRequestedObject(int requestID)
//{
//
//}
//Stringp NetGroupObject::AS3_post(Atom pMessage)
//{
//	return NULL;
//}
//void NetGroupObject::AS3_removeHaveObjects(double startIndex, double endIndex)
//{
//
//}
//void NetGroupObject::AS3_removeWantObjects(double startIndex, double endIndex)
//{
//
//}
//Stringp NetGroupObject::AS3_sendToAllNeighbors(Atom pMessage)
//{
//	return NULL;
//}
//Stringp NetGroupObject::AS3_sendToNearest(Atom message, Stringp pGroupAddress)
//{
//	return NULL;
//}
//Stringp NetGroupObject::AS3_sendToNeighbor(Atom message, Stringp pSendMode)
//{
//	return NULL;
//}
//void NetGroupObject::AS3_writeRequestedObject(int requestID, Atom object)
//{
//
//}


void NetGroupObject::AS3_constructor(NetConnectionObject *pConnection, Stringp pGroupspec)
{

}
NetGroupInfoObject* NetGroupObject::AS3_info_get()
{
	return NULL;
}
Stringp NetGroupObject::AS3_convertPeerIDToGroupAddress(Stringp pPeerID)
{
	return NULL;
}
Stringp NetGroupObject::AS3_localCoverageFrom_get()
{
	return NULL;
}
Stringp NetGroupObject::AS3_localCoverageTo_get()
{
	return NULL;
}
void NetGroupObject::AS3_invoke(uint32_t index, ArrayObject *pArgs)
{

}
}