#ifndef _AS3IllegalOperationError_
#define _AS3IllegalOperationError_

#include "../core/ErrorClass.h"

namespace avmplus{namespace NativeID{
class IllegalOperationErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
class IllegalOperationErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	//int32_t space;
private:};
}}
namespace avmshell{
	class IllegalOperationErrorClass : public ClassClosure
	{
	public:
		IllegalOperationErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IllegalOperationErrorClassSlots m_slots_IllegalOperationErrorClass;
};
class IllegalOperationErrorObject : public ErrorObject
{
	public:
		IllegalOperationErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IllegalOperationErrorObjectSlots m_slots_IllegalOperationErrorObject;
};}
#endif