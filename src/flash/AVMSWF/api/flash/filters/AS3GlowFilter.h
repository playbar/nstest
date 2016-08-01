#ifndef _AS3GlowFilter_
#define _AS3GlowFilter_
#include "AS3BitmapFilter.h"
#define AS3GLOWFILTERDATA avmplus::NativeID::GlowFilterObjectSlots

namespace avmplus{namespace NativeID{
class GlowFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GlowFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
//#if (__CORE_VERSION__>=0x02076000)
	 uint32_t color;// : uint
	 XBOOL inner;// : Boolean
	 XBOOL knockout;// : Boolean
	 int quality;// : int
	 
	 double alpha;// : Number 
	 double blurX;// : Number 
	 double blurY;// : Number	 
	 double strength;// : Number
//#else	 
//	 double alpha;// : Number 
//	 double blurX;// : Number 
//	 double blurY;// : Number	 
//	 uint32_t color;// : uint
//	 XBOOL inner;// : Boolean
//	 XBOOL knockout;// : Boolean
//	 int quality;// : int
//	 double strength;// : Number
//
//#endif
private:};
}}
namespace avmshell{
	class GlowFilterObject;
	class GlowFilterClass : public ClassClosure
	{
	public:
		GlowFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		GlowFilterObject* CreateFilter(void*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GlowFilterClassSlots m_slots_GlowFilterClass;
};
class GlowFilterObject : public BitmapFilterObject
{
	public:
		GlowFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AS3GLOWFILTERDATA& GetData(){return m_slots_GlowFilterObject;}
		virtual _XFilter* CreateFilter();	
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GlowFilterObjectSlots m_slots_GlowFilterObject;
};}
#endif
