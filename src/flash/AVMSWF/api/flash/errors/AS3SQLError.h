#ifndef _AS3SQLError_
#define _AS3SQLError_

#include "ErrorClass.h"

namespace avmplus{namespace NativeID{
class SQLErrorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SQLErrorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SQLErrorClass : public ClassClosure//NativeErrorClass
	{
	public:
		SQLErrorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLErrorClassSlots m_slots_SQLErrorClass;
};
class SQLErrorObject : public ErrorObject
{
	public:
		SQLErrorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		ArrayObject* AS3_detailArguments_get();
		int AS3_detailID_get();
		Stringp AS3_details_get();
		Stringp AS3_operation_get();

		void AS3_constructor(Stringp pOperation, Stringp pDetails, String pMessage, int id, int detailID, ArrayObject *pDetailArgs);
		void AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SQLErrorObjectSlots m_slots_SQLErrorObject;
};}
#endif