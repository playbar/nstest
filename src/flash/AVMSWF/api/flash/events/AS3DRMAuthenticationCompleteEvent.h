#ifndef _AS3DRMAuthenticationCompleteEvent_
#define _AS3DRMAuthenticationCompleteEvent_

#include "AS3Event.h"
#include "AS3ByteArray.h"

namespace avmplus{namespace NativeID{

class ByteArrayObject;

class DRMAuthenticationCompleteEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//AUTHENTICATION_COMPLETE : String = "authenticationComplete"

	Stringp AUTHENTICATION_COMPLETE;

private:};
class DRMAuthenticationCompleteEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp m_serverURL;
	Stringp m_domain;
	ByteArrayObject *m_token;
private:};
}}
namespace avmshell{
	class DRMAuthenticationCompleteEventClass : public ClassClosure//EventClass
	{
	public:
		DRMAuthenticationCompleteEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMAuthenticationCompleteEventClassSlots m_slots_DRMAuthenticationCompleteEventClass;
};
class DRMAuthenticationCompleteEventObject : public EventObject
{
	public:
		DRMAuthenticationCompleteEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Stringp AS3_domain_get();
		//void AS3_domain_set(Stringp pDomain);
		//Stringp AS3_serverURL_get();
		//void AS3_serverURL_set(Stringp pServerURL);
		//ByteArrayObject* AS3_token_get();
		//void AS3_token_set(ByteArrayObject *pToken);

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pInServerURL, 
		//						Stringp pInDomain, ByteArrayObject *pInToken);
		//EventObject* AS3_clone();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMAuthenticationCompleteEventObjectSlots m_slots_DRMAuthenticationCompleteEventObject;
};}
#endif