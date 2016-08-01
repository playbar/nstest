#ifndef _AS3BevelFilter_
#define _AS3BevelFilter_

#include "AS3BitmapFilter.h"

#define AS3BEVELFILTERDATA avmplus::NativeID::BevelFilterObjectSlots

namespace avmplus{namespace NativeID{
class BevelFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class BevelFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//#if (__CORE_VERSION__>=0x02076000)
		uint32_t highlightColor;
		XBOOL knockout;
		int quality;
		uint32_t shadowColor;
		Stringp type;
		double angle;
		double blurX;
		double blurY;
		double distance;
		double highlightAlpha;
		
		double shadowAlpha;
		
		double strength;
/*#else
		
		double angle;
		double blurX;
		double blurY;
		double distance;
		double highlightAlpha;		
		uint32_t highlightColor;
		XBOOL knockout;
		int quality;
		double shadowAlpha;
		uint32_t shadowColor;
		double strength;
		Stringp type;
#endif*/	
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BevelFilterObject;
	class BevelFilterClass : public ClassClosure
	{
	public:
		BevelFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		BevelFilterObject* CreateFilter(void*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BevelFilterClassSlots m_slots_BevelFilterClass;
};
class BevelFilterObject : public BitmapFilterObject
{
	public:
		BevelFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AS3BEVELFILTERDATA& GetData(){return m_slots_BevelFilterObject;}
		virtual _XFilter* CreateFilter();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BevelFilterObjectSlots m_slots_BevelFilterObject;
};}
#endif