#ifndef _AS3SystemUpdater_
#define _AS3SystemUpdater_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class SystemUpdaterClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SystemUpdaterObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SystemUpdaterClass : public ClassClosure
	{
	public:
		SystemUpdaterClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemUpdaterClassSlots m_slots_SystemUpdaterClass;
};
class SystemUpdaterObject : public EventDispatcherObject
{
	public:
		SystemUpdaterObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		void AS3_constructor();
		void AS3_cancel();
		void AS3_update(Stringp pType);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SystemUpdaterObjectSlots m_slots_SystemUpdaterObject;
};}
#endif