#ifndef _AS3OutputProgressEvent_
#define _AS3OutputProgressEvent_

#include "AS3Event.h"

namespace avmplus{namespace NativeID{
class OutputProgressEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//OUTPUT_PROGRESS : String = "outputProgress"

	Stringp OUTPUT_PROGRESS;

private:};
class OutputProgressEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	double m_bytesPending;
	double m_bytesTotal;
private:};
}}
namespace avmshell{
	class OutputProgressEventClass : public ClassClosure//EventClass
	{
	public:
		OutputProgressEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::OutputProgressEventClassSlots m_slots_OutputProgressEventClass;
};
class OutputProgressEventObject : public EventObject
{
	public:
		OutputProgressEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//double AS3_bytesPending_get();
		//void AS3_bytesPending_set(double bytesPending);
		//double AS3_bytesTotal_get();
		//void AS3_bytesTotal_set(double bytesTotals);

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, double bytesPending, double bytesTotal);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::OutputProgressEventObjectSlots m_slots_OutputProgressEventObject;
};}
#endif