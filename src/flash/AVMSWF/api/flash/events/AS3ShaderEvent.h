#ifndef _AS3ShaderEvent_
#define _AS3ShaderEvent_

#include "AS3Event.h"
#include "AS3BitmapData.h"

namespace avmplus{namespace NativeID{

class BitmapDataObject;
class ByteArrayObject;

class ShaderEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//COMPLETE : String = "complete"

	Stringp COMPLETE;

private:};
class ShaderEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	BitmapDataObject *m_bitmapData;
	ByteArrayObject *m_byteArray;
	DoubleVectorObject *m_vector;
private:};
}}
namespace avmshell{
	class ShaderEventClass : public ClassClosure//EventClass
	{
	public:
		ShaderEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderEventClassSlots m_slots_ShaderEventClass;
};
class ShaderEventObject : public EventObject
{
	public:
		ShaderEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//BitmapDataObject* AS3_bitmapData_get();
		//void AS3_bitmapData_set(BitmapDataObject *pBitmapData);
		//ByteArrayObject* AS3_byteArray_get();
		//void AS3_byteArray_set(ByteArrayObject *pByteArray);
		//AvmBox AS3_vector_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_vector_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

//		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_constructor(Stringp);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderEventObjectSlots m_slots_ShaderEventObject;
};}
#endif