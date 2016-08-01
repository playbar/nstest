#ifndef _AS3Font_
#define _AS3Font_
namespace avmplus{namespace NativeID{
class FontClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FontObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FontClass : public ClassClosure
	{
	public:
		FontClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		ArrayObject* AS3_enumerateFonts(XBOOL b);
		void AS3_registerFont(ClassClosure*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontClassSlots m_slots_FontClass;
};
class FontObject : public ScriptObject
{
	public:
		FontObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
//#if (__CORE_VERSION__>=0x02075000)
		Stringp AS3_fontName_get();
		Stringp AS3_fontStyle_get();
		Stringp AS3_fontType_get();
		void*	m_pChar;
//#else
//		DRCWB(Stringp) m_strName;
//		DRCWB(Stringp) m_strStyle;
//		DRCWB(Stringp) m_strType;
//		
//		Stringp AS3_fontName_get(){return m_strName;}
//		Stringp AS3_fontStyle_get(){return m_strStyle;}
//		Stringp AS3_fontType_get(){return m_strType;}
//#endif
		bool AS3_hasGlyphs(Stringp str);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontObjectSlots m_slots_FontObject;
};}
#endif