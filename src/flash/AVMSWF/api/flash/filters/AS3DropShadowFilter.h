#ifndef _AS3DropShadowFilter_
#define _AS3DropShadowFilter_
#include "AS3BitmapFilter.h"
#define AS3DROPSHADOWDATA avmplus::NativeID::DropShadowFilterObjectSlots

namespace avmplus{namespace NativeID{
class DropShadowFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DropShadowFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//#if (__CORE_VERSION__>=0x02076000)		
	uint32_t color;// : uint

	XBOOL hideObject;// : Boolean		 
	XBOOL inner;// : Boolean
	XBOOL knockout;// : Boolean		 
	int quality;// : int
	double alpha;// : Number
	double angle;// : Number
	double blurX;// : Number

	double blurY;// : Number
	double distance;// : Number
	double strength;// : Number
//#else	
//	double alpha;// : Number
//	double angle;// : Number
//	double blurX;// : Number
//	uint32_t color;// : uint
//	double blurY;// : Number
//	double distance;// : Number
//	XBOOL hideObject;// : Boolean		 
//	XBOOL inner;// : Boolean
//	XBOOL knockout;// : Boolean		 
//	int quality;// : int
//	double strength;// : Number
//#endif
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DropShadowFilterObject;
	class DropShadowFilterClass : public ClassClosure
	{
	public:
		DropShadowFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		DropShadowFilterObject* CreateFilter(void*data);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DropShadowFilterClassSlots m_slots_DropShadowFilterClass;
};
class DropShadowFilterObject : public BitmapFilterObject
{
	public:
		DropShadowFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AS3DROPSHADOWDATA& GetData(){return m_slots_DropShadowFilterObject;}
		virtual _XFilter* CreateFilter();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DropShadowFilterObjectSlots m_slots_DropShadowFilterObject;
};}
#endif