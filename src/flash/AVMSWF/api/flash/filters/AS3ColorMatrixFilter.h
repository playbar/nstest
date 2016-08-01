#ifndef _AS3ColorMatrixFilter_
#define _AS3ColorMatrixFilter_
#include "AS3BitmapFilter.h"

namespace avmplus{namespace NativeID{
class ColorMatrixFilterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ColorMatrixFilterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
	ArrayObject* matrix;
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ColorMatrixFilterObject;
	class ColorMatrixFilterClass : public ClassClosure
	{
	public:
		ColorMatrixFilterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ColorMatrixFilterObject* CreateFilter(void*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ColorMatrixFilterClassSlots m_slots_ColorMatrixFilterClass;
};
class ColorMatrixFilterObject : public BitmapFilterObject
{
	public:
		ColorMatrixFilterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		ArrayObject* GetMatrix(){return m_slots_ColorMatrixFilterObject.matrix;}
		virtual _XFilter* CreateFilter();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ColorMatrixFilterObjectSlots m_slots_ColorMatrixFilterObject;
};}
#endif