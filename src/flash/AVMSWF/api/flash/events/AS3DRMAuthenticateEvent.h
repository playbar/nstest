#ifndef _AS3DRMAuthenticateEvent_
#define _AS3DRMAuthenticateEvent_

#include "AS3Event.h"
#include "AS3NetStream.h"

namespace avmplus{namespace NativeID{

class NetStreamObject;

class DRMAuthenticateEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//AUTHENTICATION_TYPE_DRM : String = "drm"
//AUTHENTICATION_TYPE_PROXY : String = "proxy"
//DRM_AUTHENTICATE : String = "drmAuthenticate"

	Stringp AUTHENTICATION_TYPE_DRM;
	Stringp AUTHENTICATION_TYPE_PROXY;
	Stringp DRM_AUTHENTICATE;

private:};
class DRMAuthenticateEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp m_header;
	Stringp m_userPrompt;
	Stringp m_passPrompt;
	Stringp m_urlPrompt;
	Stringp m_authenticationType;
	NetStreamObject *m_netstream;
private:};
}}
namespace avmshell{
	class DRMAuthenticateEventClass : public ClassClosure//EventClass
	{
	public:
		DRMAuthenticateEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMAuthenticateEventClassSlots m_slots_DRMAuthenticateEventClass;
};
class DRMAuthenticateEventObject : public EventObject
{
	public:
		DRMAuthenticateEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Stringp AS3_authenticationType_get();
		//Stringp AS3_header_get();
		//NetStreamObject* AS3_netstream_get();
		//Stringp AS3_passwordPrompt_get();
		//Stringp AS3_urlPrompt_get();
		//Stringp AS3_usernamePrompt_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
		//						Stringp pHeader, Stringp pUserPrompt, Stringp pPassPrompt, Stringp pUrlPrompt, 
		//						String pAuthenticationType, NetStreamObject *pNetstream);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMAuthenticateEventObjectSlots m_slots_DRMAuthenticateEventObject;
};}
#endif