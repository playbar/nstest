#ifndef _AS3TextRenderer_
#define _AS3TextRenderer_
namespace avmplus{namespace NativeID{
class TextRendererClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextRendererObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextRendererClass : public ClassClosure
	{
	public:
		TextRendererClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		void AS3_displayMode_set(Stringp);
		Stringp AS3_displayMode_get();
		int AS3_maxLevel_get();
		void AS3_maxLevel_set(int);
		void AS3_setAdvancedAntiAliasingTable
			(Stringp,Stringp,Stringp,ArrayObject*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextRendererClassSlots m_slots_TextRendererClass;
};
class TextRendererObject : public ScriptObject
{
	public:
		TextRendererObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextRendererObjectSlots m_slots_TextRendererObject;
};}
#endif