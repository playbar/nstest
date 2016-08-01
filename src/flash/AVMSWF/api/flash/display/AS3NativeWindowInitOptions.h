#ifndef _AS3NativeWindowInitOptions_
#define _AS3NativeWindowInitOptions_

#include "AS3NativeWindow.h"
//#include "AS3NativeWindowRenderMode.h"

namespace avmplus{namespace NativeID{
class NativeWindowInitOptionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NativeWindowInitOptionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NativeWindowInitOptionsClass : public ClassClosure
	{
	public:
		NativeWindowInitOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowInitOptionsClassSlots m_slots_NativeWindowInitOptionsClass;
};
class NativeWindowObject;
class NativeWindowInitOptionsObject : public ScriptObject
{
	public:
		NativeWindowInitOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		bool AS3_maximizable_get();
		void AS3_maximizable_set(bool maximizable);
		bool AS3_minimizable_get();
		void AS3_minimizable_set(bool minimizable);
		NativeWindowObject* AS3_owner_get();
		void AS3_owner_set(NativeWindowObject *pOwner);
		bool AS3_resizable_get();
		void AS3_resizable_set(bool resizable);
		Stringp AS3_systemChrome_get();
		void AS3_systemChrome_set(Stringp pSystemChrome);
		bool AS3_transparent_get();
		void AS3_transparent_set(bool transparent);
		Stringp AS3_type_get();
		void AS3_type_set(Stringp pType);

		void AS3_constructor();

		//new adds 11
		Stringp m_strRenderMode;
		Stringp AS3_renderMode_get();
		void AS3_renderMode_set(Stringp renderMode);
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowInitOptionsObjectSlots m_slots_NativeWindowInitOptionsObject;
};}
#endif