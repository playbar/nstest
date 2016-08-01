#ifndef _AS3CameraUI_
#define _AS3CameraUI_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class CameraUIClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CameraUIObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CameraUIClass : public ClassClosure
	{
	public:
		CameraUIClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraUIClassSlots m_slots_CameraUIClass;
};
class CameraUIObject : public EventDispatcherObject
{
	public:
		CameraUIObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_isSupported_get();

		void AS3_constructor();
		void AS3_launch(Stringp pRequestedMediaType);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraUIObjectSlots m_slots_CameraUIObject;
};}
#endif