#ifndef _AS3DRMManagerError_
#define _AS3DRMManagerError_

#include "ErrorClass.h"

namespace avmplus{namespace NativeID{
class DRMManagerErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DRMManagerErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DRMManagerErrorClass : public ClassClosure//NativeErrorClass
	{
	public:
		DRMManagerErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMManagerErrorClassSlots m_slots_DRMManagerErrorClass;
};
class DRMManagerErrorObject : public ErrorObject
{
	public:
		DRMManagerErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//int AS3_subErrorID_get();

		//void AS3_constructor(Stringp pMessage, int id, int subErrorID);
		//void AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMManagerErrorObjectSlots m_slots_DRMManagerErrorObject;
};}
#endif