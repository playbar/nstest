#ifndef _AS3StackOverflowError_
#define _AS3StackOverflowError_

#include "../core/ErrorClass.h"

namespace avmplus{namespace NativeID{
class StackOverflowErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
class StackOverflowErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
}}
namespace avmshell{
	class StackOverflowErrorClass : public ClassClosure
	{
	public:
		StackOverflowErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StackOverflowErrorClassSlots m_slots_StackOverflowErrorClass;
};
class StackOverflowErrorObject : public ErrorObject
{
	public:
		StackOverflowErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::StackOverflowErrorObjectSlots m_slots_StackOverflowErrorObject;
};}
#endif