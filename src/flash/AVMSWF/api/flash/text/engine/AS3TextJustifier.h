#ifndef _AS3TextJustifier_
#define _AS3TextJustifier_
namespace avmplus{namespace NativeID{
class TextJustifierClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextJustifierObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextJustifierObject;
	class TextJustifierClass : public ClassClosure
	{
	public:
		TextJustifierClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		//TextJustifierObject* CreateObject(Stringp strLocal,Stringp strLineJust);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextJustifierClassSlots m_slots_TextJustifierClass;
};
class TextJustifierObject : public ScriptObject
{
	public:
		TextJustifierObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRC(Stringp) m_strLocal;
//		AvmBox AS3_getJustifierForLocale(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		Stringp AS3_lineJustification_get();
		void AS3_lineJustification_set(Stringp lineJustification);
		Stringp AS3_locale_get();
		void AS3_setLocal(Stringp);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_clone(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextJustifierObjectSlots m_slots_TextJustifierObject;
};}
#endif