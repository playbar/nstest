#ifndef _AS3ScriptTimeoutError_
#define _AS3ScriptTimeoutError_

#include "../core/ErrorClass.h"

namespace avmplus{namespace NativeID{
class ScriptTimeoutErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
class ScriptTimeoutErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
}}
namespace avmshell{
	class ScriptTimeoutErrorClass : public ClassClosure
	{
	public:
		ScriptTimeoutErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ScriptTimeoutErrorClassSlots m_slots_ScriptTimeoutErrorClass;
};
class ScriptTimeoutErrorObject : public ErrorObject
{
	public:
		ScriptTimeoutErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ScriptTimeoutErrorObjectSlots m_slots_ScriptTimeoutErrorObject;
};}
#endif