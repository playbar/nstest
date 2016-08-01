#ifndef _AS3ConvolutionFilter_
#define _AS3ConvolutionFilter_
#include "AS3BitmapFilter.h"

#define AS3CONVOLUTIONDATA avmplus::NativeID::ConvolutionFilterObjectSlots

namespace avmplus{namespace NativeID{
class ConvolutionFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ConvolutionFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
//#if (__CORE_VERSION__>=0x02076000)	 
	 int  clamp;// : Boolean
	 uint32_t color;// : uint	 
	 int preserveAlpha;// : Boolean
	 ArrayObject* matrix;// : Array
	 double alpha;//:Number;
	 double bias;// : Number;
	 double divisor;// : Number
	 double matrixX;// : Number
	 double matrixY;// : Number
//#else	 
//	 double alpha;//:Number;
//	 double bias;// : Number;
//	 int  clamp;// : Boolean
//	 uint32_t color;// : uint	 
//	 double divisor;// : Number
//	 ArrayObject* matrix;// : Array
//	 double matrixX;// : Number
//	 double matrixY;// : Number
//	 int preserveAlpha;// : Boolean
//#endif
	 
private:};
}}
namespace avmshell{
	class ConvolutionFilterObject;
	class ConvolutionFilterClass : public ClassClosure
	{
	public:
		ConvolutionFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ConvolutionFilterObject* CreateFilter(void*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ConvolutionFilterClassSlots m_slots_ConvolutionFilterClass;
};
class ConvolutionFilterObject : public BitmapFilterObject
{
	public:
		ConvolutionFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		inline AS3CONVOLUTIONDATA& GetData(){return m_slots_ConvolutionFilterObject;}
		virtual _XFilter* CreateFilter();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ConvolutionFilterObjectSlots m_slots_ConvolutionFilterObject;
};}
#endif