#ifndef _AS3DRMAuthenticationErrorEvent_
#define _AS3DRMAuthenticationErrorEvent_

#include "AS3Event.h"
#include "AS3ErrorEvent.h"

namespace avmplus{namespace NativeID{
class DRMAuthenticationErrorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//AUTHENTICATION_ERROR : String = "authenticationError"

	Stringp AUTHENTICATION_ERROR;

private:};
class DRMAuthenticationErrorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	int32_t m_subErrorID;
	Stringp m_serverURL;
	Stringp m_domain;
private:};
}}
namespace avmshell{
	class DRMAuthenticationErrorEventClass : public ClassClosure//ErrorEventClass
	{
	public:
		DRMAuthenticationErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMAuthenticationErrorEventClassSlots m_slots_DRMAuthenticationErrorEventClass;
};
class DRMAuthenticationErrorEventObject : public ErrorEventObject
{
	public:
		DRMAuthenticationErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//Stringp AS3_domain_get();
		//void AS3_domain_set(Stringp pDomain);
		//Stringp AS3_serverURL_get();
		//void AS3_serverURL_set(Stringp pServerURL);
		//int AS3_subErrorID_get();
		//void AS3_subErrorID_set(int subErrorID);

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, String pInDetail, 
		//						int inErrorID, int inSubErrorID, Stringp pInServerURL, Stringp pInDomain);
		//EventObject* AS3_clone();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMAuthenticationErrorEventObjectSlots m_slots_DRMAuthenticationErrorEventObject;
};}
#endif