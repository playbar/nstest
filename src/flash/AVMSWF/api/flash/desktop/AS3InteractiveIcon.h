#ifndef _AS3InteractiveIcon_
#define _AS3InteractiveIcon_

#include "AS3Icon.h"

namespace avmplus{namespace NativeID{
class InteractiveIconClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class InteractiveIconObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class InteractiveIconClass : public ClassClosure
	{
	public:
		InteractiveIconClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InteractiveIconClassSlots m_slots_InteractiveIconClass;
};
class InteractiveIconObject : public IconObject
{
	public:
		InteractiveIconObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_bitmaps_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bitmaps_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InteractiveIconObjectSlots m_slots_InteractiveIconObject;
};}
#endif