#ifndef _AS3GroupSpecifier_
#define _AS3GroupSpecifier_

#include "AS3ByteArray.h"

namespace avmplus{namespace NativeID{
class GroupSpecifierClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GroupSpecifierObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp	m_routing;
	Stringp	m_multicast;
	Stringp	m_objectReplication;
	Stringp	m_posting;
	Stringp	m_publishAuthHash;
	Stringp	m_postingAuthHash;
	Stringp	m_ipMulticastAddresses;
	Stringp	m_bootstrapPeers;
	Stringp	m_openServerChannel;
	Stringp	m_disablePeerToPeer;
	Stringp	m_tag;
	Stringp	m_unique;
	Stringp	m_publishAuth;
	Stringp	m_postingAuth;
	Stringp	m_ipMulticastMemberUpdates;
private:};
}}
namespace avmshell{
	class GroupSpecifierClass : public ClassClosure
	{
	public:
		GroupSpecifierClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		Stringp	AS3_calcSHA256Digest(ByteArrayObject *pInput){return NULL;}
		Stringp	AS3_GetCryptoRandomString(uint32_t length){return NULL;}
        //new adds 11
        int AS3_maxSupportedGroupspecVersion_get();
        //new adds 11 end
		int m_maxSupportedGroupspecVersion;
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GroupSpecifierClassSlots m_slots_GroupSpecifierClass;
};
class GroupSpecifierObject : public ScriptObject
{
	public:
		GroupSpecifierObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!

		//bool AS3_ipMulticastMemberUpdatesEnabled_get();
		//void AS3_ipMulticastMemberUpdatesEnabled_set(bool ipMulticastMemberUpdatesEnabled);
		//bool AS3_multicastEnabled_get();
		//void AS3_multicastEnabled_set(bool multicastEnabled);
		//bool AS3_objectReplicationEnabled_get();
		//void AS3_objectReplicationEnabled_set(bool objectReplicationEnabled);
		//bool AS3_peerToPeerDisabled_get();
		//void AS3_peerToPeerDisabled_set(bool peerToPeerDisabled);
		//bool AS3_postingEnabled_get();
		//void AS3_postingEnabled_set(bool postingEnabled);
		//bool AS3_routingEnabled_get();
		//void AS3_routingEnabled_set(bool routingEnabled);
		//bool AS3_serverChannelEnabled_get();
		//void AS3_serverChannelEnabled_set(bool serverChannelEnabled);

		//void AS3_constructor(Stringp pName);
		//void AS3_addBootstrapPeer(Stringp pPeerID);
		//void AS3_addIPMulticastAddress(Stringp pAddress, Atom port, Stringp pSource);
		//Stringp AS3_authorizations();
		//Stringp AS3_encodeBootstrapPeerIDSpec(Stringp pPeerID);
		//Stringp AS3_encodeIPMulticastAddressSpec(Stringp pAddress, Atom port, Stringp pSource);
		//Stringp AS3_encodePostingAuthorization(Stringp pPassword);
		//Stringp AS3_encodePublishAuthorization(Stringp pPassword);
		//Stringp AS3_groupspecWithAuthorizations();
		//Stringp AS3_groupspecWithoutAuthorizations();
		//void AS3_makeUnique();
		//void AS3_setPostingPassword(Stringp pPassword, Stringp pSalt);
		//void AS3_setPublishPassword(Stringp pPassword, Stringp pSalt);
		//Stringp AS3_toString();
		//new adds 11
		int m_minGroupspecVersion;
        int AS3_minGroupspecVersion_get();
		void AS3_minGroupspecVersion_set(int iVersion);
        //new adds 11 end

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GroupSpecifierObjectSlots m_slots_GroupSpecifierObject;
};}
#endif