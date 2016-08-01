#ifndef _AS3ShaderData_
#define _AS3ShaderData_

#include "AS3ByteArray.h"

namespace avmplus{namespace NativeID{
class ShaderDataClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ShaderDataObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ShaderDataClass : public ClassClosure
	{
	public:
		ShaderDataClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderDataClassSlots m_slots_ShaderDataClass;
};
class ShaderDataObject : public ScriptObject
{
	public:
		ShaderDataObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//void AS3_constructor(ByteArrayObject *pByteCode);

		void AS3_setByteCode(ByteArrayObject *pByteCode);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderDataObjectSlots m_slots_ShaderDataObject;
};}
#endif