#ifndef _AS3URLRequestHeader_
#define _AS3URLRequestHeader_
namespace avmplus{namespace NativeID{
class URLRequestHeaderClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class URLRequestHeaderObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp name;
	Stringp value;
private:};
}}
namespace avmshell{
	class URLRequestHeaderClass : public ClassClosure
	{
	public:
		URLRequestHeaderClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLRequestHeaderClassSlots m_slots_URLRequestHeaderClass;
};
class URLRequestHeaderObject : public ScriptObject
{
	public:
		URLRequestHeaderObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		inline Stringp getName(){return m_slots_URLRequestHeaderObject.name;}
		inline Stringp getValue(){return m_slots_URLRequestHeaderObject.value;}
		//void* pData;//Set your data!!
		//AvmBox AS3_name_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_name_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_value_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_value_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLRequestHeaderObjectSlots m_slots_URLRequestHeaderObject;
};}
#endif