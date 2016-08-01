#ifndef _AS3Shader_
#define _AS3Shader_

#include "AS3ByteArray.h"
#include "AS3ShaderData.h"

namespace avmplus{namespace NativeID{
class ShaderClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ShaderObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ShaderDataObject;
	class ShaderClass : public ClassClosure
	{
	public:
		ShaderClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderClassSlots m_slots_ShaderClass;
};
class ShaderObject : public ScriptObject
{
	public:
		ShaderObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(ShaderDataObject*) m_pData;
		DRC(Stringp)			 m_strHint;
		//void* pData;//Set your data!!
		//void AS3_byteCode_set(ByteArrayObject *pByteCode);
		ShaderDataObject* AS3_data_get();
		void AS3_data_set(ShaderDataObject *pData);
		Stringp AS3_precisionHint_get();
		void AS3_precisionHint_set(Stringp pPrecisionHint);

//		void AS3_constructor(ByteArrayObject *pCode);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderObjectSlots m_slots_ShaderObject;
};}
#endif