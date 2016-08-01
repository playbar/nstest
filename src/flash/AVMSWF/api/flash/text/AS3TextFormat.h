#ifndef _AS3TextFormat_
#define _AS3TextFormat_

#define AS3TEXTFORMAT avmplus::NativeID::TextFormatObjectSlots

namespace avmplus{namespace NativeID{
class TextFormatClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextFormatObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	 Stringp align;
	 Atom blockIndent;
	 Atom bold;
	 Atom bullet;
	 Atom color;
	 Stringp font;
	 Atom indent;
	 Atom italic;
	 Atom kerning;
	 Atom leading;
	 Atom leftMargin;
	 Atom letterSpacing;
	 Atom rightMargin;
	 Atom size;
	 ArrayObject* tabStops;
	 Stringp target;
	 Atom underline;
	 Stringp display;
	 //Stringp url;
private:};
}}
namespace avmshell{
	class TextFormatClass : public ClassClosure
	{
	public:
		TextFormatClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFormatClassSlots m_slots_TextFormatClass;
};
class TextFormatObject : public ScriptObject
{
	public:
		TextFormatObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
	public:
		DRCWB(Stringp) m_strURL;
		void SetObjFormat(ScriptObject*pFmt);
		void SetFormat(void*);
		void GetFormat(void*);
		Stringp AS3_url_get(){return m_strURL;}
		void AS3_url_set(Stringp s){m_strURL=s;}
		AS3TEXTFORMAT& getFormat(){return m_slots_TextFormatObject;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFormatObjectSlots m_slots_TextFormatObject;
};}
#endif