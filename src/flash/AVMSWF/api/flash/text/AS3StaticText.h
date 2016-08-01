#ifndef _AS3StaticText_
#define _AS3StaticText_

#include "AS3DisplayObject.h"

namespace avmplus{namespace NativeID{
class StaticTextClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class StaticTextObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class StaticTextClass : public ClassClosure
	{
	public:
		StaticTextClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StaticTextClassSlots m_slots_StaticTextClass;
};
class StaticTextObject : public DisplayObjectObject
{
	public:
		StaticTextObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		//virtual Atom construct(int argc, Atom* argv);
	public:
		Stringp AS3_text_get();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StaticTextObjectSlots m_slots_StaticTextObject;
};}
#endif