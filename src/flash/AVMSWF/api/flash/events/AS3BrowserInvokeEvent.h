#ifndef _AS3BrowserInvokeEvent_
#define _AS3BrowserInvokeEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class BrowserInvokeEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//BROWSER_INVOKE : String = "browserInvoke"

	Stringp BROWSER_INVOKE;

private:};
class BrowserInvokeEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BrowserInvokeEventClass : public ClassClosure//EventClass
	{
	public:
		BrowserInvokeEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BrowserInvokeEventClassSlots m_slots_BrowserInvokeEventClass;
};
class BrowserInvokeEventObject : public EventObject
{
	public:
		BrowserInvokeEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		ArrayObject* AS3_arguments_get();
		bool AS3_isHTTPS_get();
		bool AS3_isUserEvent_get();
		Stringp AS3_sandboxType_get();
		Stringp AS3_securityDomain_get();

		void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, 
								ArrayObject *pArguments, Stringp pSandboxType, Stringp pSecurityDomain, 
								bool isHTTPS, bool isUserEvent);
		EventObject* AS3_clone();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BrowserInvokeEventObjectSlots m_slots_BrowserInvokeEventObject;
};}
#endif