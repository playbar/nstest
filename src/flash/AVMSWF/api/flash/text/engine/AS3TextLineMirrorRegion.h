#ifndef _AS3TextLineMirrorRegion_
#define _AS3TextLineMirrorRegion_

#include "AS3Rectangle.h"
#include "AS3TextLine.h"
#include "AS3ContentElement.h"
#include "AS3EventDispatcher.h"
#include "AS3TextLineMirrorRegion.h"

namespace avmplus{namespace NativeID{
class TextLineMirrorRegionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextLineMirrorRegionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextLineMirrorRegionClass : public ClassClosure
	{
	public:
		TextLineMirrorRegionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineMirrorRegionClassSlots m_slots_TextLineMirrorRegionClass;
};
class TextLineMirrorRegionObject : public ScriptObject
{
	public:
		TextLineMirrorRegionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		RectangleObject* AS3_bounds_get();
		ContentElementObject* AS3_element_get();
		EventDispatcherObject* AS3_mirror_get();
		TextLineMirrorRegionObject* AS3_nextRegion_get();
		TextLineMirrorRegionObject* AS3_previousRegion_get();
		TextLineObject* AS3_textLine_get();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineMirrorRegionObjectSlots m_slots_TextLineMirrorRegionObject;
};}
#endif