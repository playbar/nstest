#ifndef _AS3NetGroup_
#define _AS3NetGroup_

#include "AS3EventDispatcher.h"

#include "AS3NetGroupInfo.h"
#include "AS3NetConnection.h"

namespace avmplus{namespace NativeID{
class NetGroupClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	uint32_t	kClose;
	uint32_t	kPost;
	uint32_t	kSendToNearest;
	uint32_t	kSendToNeighbor;
	uint32_t	kGetReceiveMode;
	uint32_t	kSetReceiveMode;
	uint32_t	kGetEstimatedMemberCount;
	uint32_t	kGetNeighborCount;
	uint32_t	kGetLocalCoverageFrom;
	uint32_t	kGetLocalCoverageTo;
	uint32_t	kGetReplicationStrategy;
	uint32_t	kSetReplicationStrategy;
	uint32_t	kUpdateHaveBlocks;
	uint32_t	kUpdateWantBlocks;
	uint32_t	kWriteRequestedBlock;
	uint32_t	kDenyRequestedBlock;
	uint32_t	kAddNeighbor;
	uint32_t	kAddMemberHint;
private:};
class NetGroupObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NetGroupClass : public ClassClosure
	{
	public:
		NetGroupClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupClassSlots m_slots_NetGroupClass;
};
class NetGroupObject : public EventDispatcherObject
{
	public:
		NetGroupObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_estimatedMemberCount_get();
		//double AS3_neighborCount_get();
		//Stringp AS3_receiveMode_get();
		//void AS3_receiveMode_set(Stringp pReceiveMode);
		//Stringp AS3_replicationStrategy_get();
		//void AS3_replicationStrategy_set(Stringp pReplicationStrategy);

		//void AS3_addHaveObjects(double startIndex, double endIndex);
		//bool AS3_addMemberHint(Stringp pPeerID);
		//bool AS3_addNeighbor(Stringp pPeerID);
		//void AS3_addWantObjects(double startIndex, double endIndex);
		//void AS3_close();
		//void AS3_denyRequestedObject(int requestID);
		//Stringp AS3_post(Atom pMessage);
		//void AS3_removeHaveObjects(double startIndex, double endIndex);
		//void AS3_removeWantObjects(double startIndex, double endIndex);
		//Stringp AS3_sendToAllNeighbors(Atom pMessage);
		//Stringp AS3_sendToNearest(Atom message, Stringp pGroupAddress);
		//Stringp AS3_sendToNeighbor(Atom message, Stringp pSendMode);
		//void AS3_writeRequestedObject(int requestID, Atom object);

		void AS3_constructor(NetConnectionObject *pConnection, Stringp pGroupspec);
		NetGroupInfoObject* AS3_info_get();
		Stringp AS3_convertPeerIDToGroupAddress(Stringp pPeerID);
		Stringp AS3_localCoverageFrom_get();
		Stringp AS3_localCoverageTo_get();

		void AS3_invoke(uint32_t index, ArrayObject *pArgs);

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetGroupObjectSlots m_slots_NetGroupObject;
};}
#endif