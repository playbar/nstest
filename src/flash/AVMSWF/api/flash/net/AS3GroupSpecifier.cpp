#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GroupSpecifier.h"
namespace avmshell{
GroupSpecifierClass::GroupSpecifierClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
	m_maxSupportedGroupspecVersion = 2;
};
ScriptObject* GroupSpecifierClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GroupSpecifierObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GroupSpecifierObject::GroupSpecifierObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_minGroupspecVersion = 2;
}
//////////////////////////////////////////////////////////
//Native Method start...
//bool GroupSpecifierObject::AS3_ipMulticastMemberUpdatesEnabled_get()
//{
//	return false;
//}
//void GroupSpecifierObject::AS3_ipMulticastMemberUpdatesEnabled_set(bool ipMulticastMemberUpdatesEnabled)
//{
//
//}
//bool GroupSpecifierObject::AS3_multicastEnabled_get()
//{
//	return false;
//}
//void GroupSpecifierObject::AS3_multicastEnabled_set(bool multicastEnabled)
//{
//
//}
//bool GroupSpecifierObject::AS3_objectReplicationEnabled_get()
//{
//	return false;
//}
//void GroupSpecifierObject::AS3_objectReplicationEnabled_set(bool objectReplicationEnabled)
//{
//
//}
//bool GroupSpecifierObject::AS3_peerToPeerDisabled_get()
//{
//	return false;
//}
//void GroupSpecifierObject::AS3_peerToPeerDisabled_set(bool peerToPeerDisabled)
//{
//
//}
//bool GroupSpecifierObject::AS3_postingEnabled_get()
//{
//	return false;
//}
//void GroupSpecifierObject::AS3_postingEnabled_set(bool postingEnabled)
//{
//
//}
//bool GroupSpecifierObject::AS3_routingEnabled_get()
//{
//	return false;
//}
//void GroupSpecifierObject::AS3_routingEnabled_set(bool routingEnabled)
//{
//
//}
//bool GroupSpecifierObject::AS3_serverChannelEnabled_get()
//{
//	return false;
//}
//void GroupSpecifierObject::AS3_serverChannelEnabled_set(bool serverChannelEnabled)
//{
//
//}
//
//void GroupSpecifierObject::AS3_constructor(Stringp pName)
//{
//
//}
//void GroupSpecifierObject::AS3_addBootstrapPeer(Stringp pPeerID)
//{
//
//}
//void GroupSpecifierObject::AS3_addIPMulticastAddress(Stringp pAddress, Atom port, Stringp pSource)
//{
//
//}
//Stringp GroupSpecifierObject::AS3_authorizations()
//{
//	return NULL;
//}
//Stringp GroupSpecifierObject::AS3_encodeBootstrapPeerIDSpec(Stringp pPeerID)
//{
//	return NULL;
//}
//Stringp GroupSpecifierObject::AS3_encodeIPMulticastAddressSpec(Stringp pAddress, Atom port, Stringp pSource)
//{
//	return NULL;
//}
//Stringp GroupSpecifierObject::AS3_encodePostingAuthorization(Stringp pPassword)
//{
//	return NULL;
//}
//Stringp GroupSpecifierObject::AS3_encodePublishAuthorization(Stringp pPassword)
//{
//	return NULL;
//}
//Stringp GroupSpecifierObject::AS3_groupspecWithAuthorizations()
//{
//	return NULL;
//}
//Stringp GroupSpecifierObject::AS3_groupspecWithoutAuthorizations()
//{
//	return NULL;
//}
//void GroupSpecifierObject::AS3_makeUnique()
//{
//
//}
//void GroupSpecifierObject::AS3_setPostingPassword(Stringp pPassword, Stringp pSalt)
//{
//
//}
//void GroupSpecifierObject::AS3_setPublishPassword(Stringp pPassword, Stringp pSalt)
//{
//
//}
//Stringp GroupSpecifierObject::AS3_toString()
//{
//	return NULL;
//}

		//new adds 11
        int GroupSpecifierObject::AS3_minGroupspecVersion_get()
		{
			//LOGWHERE();
			return m_minGroupspecVersion;
		}
		void GroupSpecifierObject::AS3_minGroupspecVersion_set(int iVersion)
		{
			//LOGWHERE();
			m_minGroupspecVersion = iVersion;
		}
        //new adds 11 end

		//new adds 11
        int GroupSpecifierClass::AS3_maxSupportedGroupspecVersion_get()
		{
			//LOGWHERE();
			return m_maxSupportedGroupspecVersion;
		}
        //new adds 11 end
}