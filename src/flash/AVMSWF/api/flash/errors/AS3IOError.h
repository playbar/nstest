#ifndef _AS3IOError_
#define _AS3IOError_

#include "../core/ErrorClass.h"

namespace avmplus{namespace NativeID{
class IOErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
class IOErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:

private:};
}}
namespace avmshell{
	class IOErrorClass : public ClassClosure
	{
	public:
		IOErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IOErrorClassSlots m_slots_IOErrorClass;
};
class IOErrorObject : public ErrorObject
{
	public:
		IOErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IOErrorObjectSlots m_slots_IOErrorObject;
};}
#endif