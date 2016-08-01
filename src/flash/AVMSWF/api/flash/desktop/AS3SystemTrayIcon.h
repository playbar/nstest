#ifndef _AS3SystemTrayIcon_
#define _AS3SystemTrayIcon_

#include "AS3InteractiveIcon.h"

namespace avmplus{namespace NativeID{
class SystemTrayIconClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//MAX_TIP_LENGTH : Number = 63

	double MAX_TIP_LENGTH;

private:};
class SystemTrayIconObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SystemTrayIconClass : public ClassClosure
	{
	public:
		SystemTrayIconClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemTrayIconClassSlots m_slots_SystemTrayIconClass;
};
class SystemTrayIconObject : public InteractiveIconObject
{
	public:
		SystemTrayIconObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_bitmaps_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bitmaps_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_height_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_menu_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_menu_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_tooltip_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_tooltip_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_width_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemTrayIconObjectSlots m_slots_SystemTrayIconObject;
};}
#endif