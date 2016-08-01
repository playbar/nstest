#ifndef _AS3MemoryError_
#define _AS3MemoryError_

#include "../core/ErrorClass.h"

namespace avmplus{namespace NativeID{
class MemoryErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
class MemoryErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
}}
namespace avmshell{
	class MemoryErrorClass : public ClassClosure
	{
	public:
		MemoryErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MemoryErrorClassSlots m_slots_MemoryErrorClass;
};
class MemoryErrorObject : public ErrorObject
{
	public:
		MemoryErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MemoryErrorObjectSlots m_slots_MemoryErrorObject;
};}
#endif