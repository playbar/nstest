#ifndef _AS3MorphShape_
#define _AS3MorphShape_

#include "AS3DisplayObject.h"

namespace avmplus{namespace NativeID{
class MorphShapeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MorphShapeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MorphShapeClass : public ClassClosure
	{
	public:
		MorphShapeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MorphShapeClassSlots m_slots_MorphShapeClass;
};
class MorphShapeObject : public DisplayObjectObject
{
	public:
		MorphShapeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MorphShapeObjectSlots m_slots_MorphShapeObject;
};}
#endif