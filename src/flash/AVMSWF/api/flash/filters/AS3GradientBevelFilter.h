#ifndef _AS3GradientBevelFilter_
#define _AS3GradientBevelFilter_
#include "AS3BitmapFilter.h"
#define AS3GRADIENTBEVELDATA avmplus::NativeID::GradientBevelFilterObjectSlots

namespace avmplus{namespace NativeID{
class GradientBevelFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GradientBevelFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//#if (__CORE_VERSION__>=0x02076000)	

	 XBOOL knockout;// : Boolean
	 int quality;// : int	 
	 ArrayObject* alphas;// : Array 	
	 ArrayObject* colors;// : Array	 
	 ArrayObject* ratios;// : Array
	 Stringp type;// : String
	 double angle;// : Number
	 double blurX;// : Number
	 double blurY;// : Number
	 double distance;// : Number
	 double strength;// : Number	
//#else
//	 ArrayObject* alphas;// : Array 	
//	 double angle;// : Number
//	 double blurX;// : Number
//	 double blurY;// : Number
//	 ArrayObject* colors;// : Array	 
//	 double distance;// : Number
//	 XBOOL knockout;// : Boolean
//	 int quality;// : int	 
//	 ArrayObject* ratios;// : Array
//	 double strength;// : Number	
//	 Stringp type;// : String	 	 	 
//#endif
	 
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class GradientBevelFilterObject;
	class GradientBevelFilterClass : public ClassClosure
	{
	public:
		GradientBevelFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		class GradientBevelFilterObject* CreateFilter(void*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GradientBevelFilterClassSlots m_slots_GradientBevelFilterClass;
};
class GradientBevelFilterObject : public BitmapFilterObject
{
	public:
		GradientBevelFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AS3GRADIENTBEVELDATA& GetData(){return m_slots_GradientBevelFilterObject;}
		virtual _XFilter* CreateFilter();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GradientBevelFilterObjectSlots m_slots_GradientBevelFilterObject;
};}
#endif