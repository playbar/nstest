#ifndef _AS3EOFError_
#define _AS3EOFError_

#include "AS3IOError.h"

namespace avmplus{namespace NativeID{
class EOFErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//int32_t space;
	int32_t length;
	Stringp name; 
private:};
class EOFErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

private:};
}}
namespace avmshell{
	class EOFErrorClass : public ClassClosure
	{
	public:
		EOFErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EOFErrorClassSlots m_slots_EOFErrorClass;
};
class EOFErrorObject : public IOErrorObject
{
	public:
		EOFErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EOFErrorObjectSlots m_slots_EOFErrorObject;
};}
#endif