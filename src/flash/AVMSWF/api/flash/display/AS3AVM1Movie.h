#ifndef _AS3AVM1Movie_
#define _AS3AVM1Movie_

#include "AS3DisplayObject.h"

namespace avmplus{namespace NativeID{
class AVM1MovieClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class AVM1MovieObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class AVM1MovieClass : public ClassClosure
	{
	public:
		AVM1MovieClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AVM1MovieClassSlots m_slots_AVM1MovieClass;
};
class AVM1MovieObject : public DisplayObjectObject
{
	public:
		AVM1MovieObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AVM1MovieObjectSlots m_slots_AVM1MovieObject;
};}
#endif