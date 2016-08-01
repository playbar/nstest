#ifndef _AS3TextBaseline_
#define _AS3TextBaseline_
namespace avmplus{namespace NativeID{
class TextBaselineClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ASCENT : String = "ascent"
//DESCENT : String = "descent"
//IDEOGRAPHIC_BOTTOM : String = "ideographicBottom"
//IDEOGRAPHIC_CENTER : String = "ideographicCenter"
//IDEOGRAPHIC_TOP : String = "ideographicTop"
//ROMAN : String = "roman"
//USE_DOMINANT_BASELINE : String = "useDominantBaseline"

	Stringp ASCENT;
	Stringp DESCENT;
	Stringp IDEOGRAPHIC_BOTTOM;
	Stringp IDEOGRAPHIC_CENTER;
	Stringp IDEOGRAPHIC_TOP;
	Stringp ROMAN;
	Stringp USE_DOMINANT_BASELINE;

private:};
class TextBaselineObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextBaselineClass : public ClassClosure
	{
	public:
		enum {ASCENT,DESCENT,IDEOGRAPHIC_BOTTOM,
			  IDEOGRAPHIC_CENTER,IDEOGRAPHIC_TOP,
			  ROMAN,USE_DOMINANT_BASELINE};
		int GetBaseMode(Stringp strBaseLine);
		Stringp GetBaseName(int iMode); 
		TextBaselineClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		Stringp getROMAN(){return m_slots_TextBaselineClass.ROMAN;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextBaselineClassSlots m_slots_TextBaselineClass;
};
class TextBaselineObject : public ScriptObject
{
	public:
		TextBaselineObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextBaselineObjectSlots m_slots_TextBaselineObject;
};}
#endif