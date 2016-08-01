#ifndef _AS3DisplacementMapFilter_
#define _AS3DisplacementMapFilter_
#include "AS3BitmapFilter.h"

#define AS3DISPLACEDATA avmplus::NativeID::DisplacementMapFilterObjectSlots

namespace avmplus{namespace NativeID{
class DisplacementMapFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DisplacementMapFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//#if (__CORE_VERSION__>=0x02076000)		 
	 uint32_t color;// : uint
	 uint32_t componentX;// : uint
	 uint32_t componentY;// : uint
	 class BitmapDataObject* mapBitmap;// : BitmapData
	 class PointObject* mapPoint;// : Point
	 Stringp mode;// : String
	 double alpha;// : Number
	 double scaleX;// : Number
	 double scaleY;// : Number
//#else
//	 double alpha;// : Number
//	 uint32_t color;// : uint
//	 uint32_t componentX;// : uint
//	 uint32_t componentY;// : uint
//	 class BitmapDataObject* mapBitmap;// : BitmapData
//	 class PointObject* mapPoint;// : Point
//	 Stringp mode;// : String	 
//	 double scaleX;// : Number
//	 double scaleY;// : Number
//#endif
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DisplacementMapFilterClass : public ClassClosure
	{
	public:
		DisplacementMapFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DisplacementMapFilterClassSlots m_slots_DisplacementMapFilterClass;
};
class DisplacementMapFilterObject : public BitmapFilterObject
{
	public:
		DisplacementMapFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AS3DISPLACEDATA& GetData(){return m_slots_DisplacementMapFilterObject;}
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DisplacementMapFilterObjectSlots m_slots_DisplacementMapFilterObject;
};}
#endif