#ifndef _AS3XMLUI_
#define _AS3XMLUI_
namespace avmplus{namespace NativeID{
class XMLUIClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class XMLUIObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class XMLUIClass : public ClassClosure
	{
	public:
		XMLUIClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		void AS3_accept();
		void AS3_cancel();
		Stringp AS3_getProperty(Stringp name);
		void AS3_setProperty(Stringp name, Stringp value);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLUIClassSlots m_slots_XMLUIClass;
};
class XMLUIObject : public ScriptObject
{
	public:
		XMLUIObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLUIObjectSlots m_slots_XMLUIObject;
};}
#endif