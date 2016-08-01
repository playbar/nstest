#ifndef _AS3GradientGlowFilter_
#define _AS3GradientGlowFilter_

#include "AS3BitmapFilter.h"
#define AS3GRADIENTGLOW avmplus::NativeID::GradientGlowFilterObjectSlots

namespace avmplus{namespace NativeID{
class GradientGlowFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GradientGlowFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
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
private:};
}}
namespace avmshell{
	class GradientGlowFilterObject;
	class GradientGlowFilterClass : public ClassClosure
	{
	public:
		GradientGlowFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		GradientGlowFilterObject* CreateFilter(void*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GradientGlowFilterClassSlots m_slots_GradientGlowFilterClass;
};
class GradientGlowFilterObject : public BitmapFilterObject
{
	public:
		GradientGlowFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AS3GRADIENTGLOW& GetData(){return m_slots_GradientGlowFilterObject;}
		virtual _XFilter* CreateFilter();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GradientGlowFilterObjectSlots m_slots_GradientGlowFilterObject;
};}
#endif