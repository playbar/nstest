#ifndef _AS3BlurFilter_
#define _AS3BlurFilter_
#include "AS3BitmapFilter.h"
#define AS3BLURFILTERDATA avmplus::NativeID::BlurFilterObjectSlots

namespace avmplus{namespace NativeID{
class BlurFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class BlurFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//#if (__CORE_VERSION__>=0x02076000)
	int	   quality;
	double blurX;
	double blurY;
//#else	
//	double blurX;
//	double blurY;
//	int	   quality;
//#endif
	
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class BlurFilterObject;
	class BlurFilterClass : public ClassClosure
	{
	public:
		BlurFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		BlurFilterObject* CreateFilter(void*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BlurFilterClassSlots m_slots_BlurFilterClass;
};
class BlurFilterObject : public BitmapFilterObject
{
	public:
		BlurFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AS3BLURFILTERDATA& GetData(){return m_slots_BlurFilterObject;}
		virtual _XFilter* CreateFilter();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BlurFilterObjectSlots m_slots_BlurFilterObject;
};}
#endif