#ifndef _AS3Icon_
#define _AS3Icon_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class IconClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class IconObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class IconClass : public ClassClosure
	{
	public:
		IconClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IconClassSlots m_slots_IconClass;
};
class IconObject : public EventDispatcherObject
{
	public:
		IconObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_bitmaps_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bitmaps_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IconObjectSlots m_slots_IconObject;
};}
#endif