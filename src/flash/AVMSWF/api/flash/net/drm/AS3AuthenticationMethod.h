#ifndef _AS3AuthenticationMethod_
#define _AS3AuthenticationMethod_
namespace avmplus{namespace NativeID{
class AuthenticationMethodClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ANONYMOUS : String = "anonymous"
//USERNAME_AND_PASSWORD : String = "usernameAndPassword"

	Stringp ANONYMOUS;
	Stringp USERNAME_AND_PASSWORD;

private:};
class AuthenticationMethodObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class AuthenticationMethodClass : public ClassClosure
	{
	public:
		AuthenticationMethodClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AuthenticationMethodClassSlots m_slots_AuthenticationMethodClass;
};
class AuthenticationMethodObject : public ScriptObject
{
	public:
		AuthenticationMethodObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AuthenticationMethodObjectSlots m_slots_AuthenticationMethodObject;
};}
#endif