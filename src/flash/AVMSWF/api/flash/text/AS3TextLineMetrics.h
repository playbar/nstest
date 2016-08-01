#ifndef _AS3TextLineMetrics_
#define _AS3TextLineMetrics_

#define AS3TEXTLINEMETRICS avmplus::NativeID::TextLineMetricsObjectSlots

namespace avmplus{namespace NativeID{
class TextLineMetricsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextLineMetricsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	//double ascent;
	//double descent;
 	//double height;
 	//double leading;
 	//double width;
 	//double x;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextLineMetricsClass : public ClassClosure
	{
	public:
		TextLineMetricsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineMetricsClassSlots m_slots_TextLineMetricsClass;
};
class TextLineMetricsObject : public ScriptObject
{
	public:
		TextLineMetricsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		double m_ascent,m_descent,m_width,m_height,m_x,m_leading;
		AS3TEXTLINEMETRICS& GetData(){return m_slots_TextLineMetricsObject;};		
		void AS3_constructor(double x,double w,double h,double ascent,double decent,double leading);
		double AS3_ascent_get(){return m_ascent;}
		void AS3_ascent_set(double v){m_ascent=v;}
		double AS3_descent_get(){return m_descent;}
		void AS3_descent_set(double v){m_descent=v;}
		double AS3_height_get(){return m_height;}
		void AS3_height_set(double v){m_height=v;}
		double AS3_width_get(){return m_width;}
		void AS3_width_set(double v){m_width=v;}
		double AS3_x_get(){return m_x;}
		void AS3_x_set(double v){m_x=v;}
		double AS3_leading_get(){return m_leading;}
		void AS3_leading_set(double v){m_leading=v;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineMetricsObjectSlots m_slots_TextLineMetricsObject;
};}
#endif