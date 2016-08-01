#ifndef _AS3TextExtent_
#define _AS3TextExtent_
namespace avmplus{namespace NativeID{
class TextExtentClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextExtentObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double width;
	double height;
	double textFieldWidth;
	double textFieldHeight;
	double ascent;
	double descent;
private:};
}}
namespace avmshell{
	class TextExtentClass : public ClassClosure
	{
	public:
		TextExtentClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextExtentClassSlots m_slots_TextExtentClass;
};
class TextExtentObject : public ScriptObject
{
	public:
		TextExtentObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_ascent_get();
		//void AS3_ascent_set(double ascent);
		//double AS3_descent_get();
		//void AS3_descent_set(double descent);
		//double AS3_height_get();
		//void AS3_height_set(double height);
		//double AS3_textFieldHeight_get();
		//void AS3_textFieldHeight_set(double textFieldHeight);
		//double AS3_textFieldWidth_get();
		//void AS3_textFieldWidth_set(double textFieldWidth);
		//double AS3_width_get();
		//void AS3_width_set(double width);

		//void AS3_constructor(double width, double height, double textFieldWidth, double textFieldHeight, double ascent, double descent);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextExtentObjectSlots m_slots_TextExtentObject;
};}
#endif