#ifndef _AS3InvalidSWFError_
#define _AS3InvalidSWFError_

#include "ErrorClass.h"

namespace avmplus{namespace NativeID{
class InvalidSWFErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class InvalidSWFErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class InvalidSWFErrorClass : public ClassClosure
	{
	public:
		InvalidSWFErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InvalidSWFErrorClassSlots m_slots_InvalidSWFErrorClass;
};
class InvalidSWFErrorObject : public ErrorObject
{
	public:
		InvalidSWFErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::InvalidSWFErrorObjectSlots m_slots_InvalidSWFErrorObject;
};}
#endif