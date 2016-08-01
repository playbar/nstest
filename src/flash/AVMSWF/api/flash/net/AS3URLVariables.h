#ifndef _AS3URLVariables_
#define _AS3URLVariables_
namespace avmplus{namespace NativeID{
class URLVariablesClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class URLVariablesObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	//ArrayObject* context;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class URLVariablesObject;
	class URLVariablesClass : public ClassClosure
	{
	public:
		URLVariablesClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		URLVariablesObject* CreateVars(Stringp);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLVariablesClassSlots m_slots_URLVariablesClass;
};
class URLVariablesObject : public ScriptObject
{
	public:
		URLVariablesObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//ArrayObject* GetData(){return m_slots_URLVariablesObject.context;}
		//void* pData;//Set your data!!
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_decode(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_toString(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLVariablesObjectSlots m_slots_URLVariablesObject;
};}
#endif