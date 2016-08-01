#ifndef _AS3Responder_
#define _AS3Responder_

#include "../core/FunctionClass.h"

namespace avmplus{namespace NativeID{
class ResponderClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ResponderObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ResponderObject;
	class ByteArrayObject;
	class ResponderClass : public ClassClosure
	{
	public:
		ResponderObject* CreateResponder(FunctionObject *pResult, FunctionObject *pStatus);

	public:
		ResponderClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ResponderClassSlots m_slots_ResponderClass;
};
class ResponderObject : public ScriptObject
{
	public:
		ResponderObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		XU32 m_nCreateTime;
		//DRCWB(ByteArrayObject*)m_inData;
		DRCWB(FunctionObject*) m_pResultCall;
		DRCWB(FunctionObject*) m_pStatusCall;
		void AS3_constructor(FunctionObject *pResult, FunctionObject *pStatus);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ResponderObjectSlots m_slots_ResponderObject;
};}
#endif