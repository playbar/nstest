#ifndef _AS3ShaderParameterType_
#define _AS3ShaderParameterType_
namespace avmplus{namespace NativeID{
class ShaderParameterTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//BOOL : String = "bool"
//BOOL2 : String = "bool2"
//BOOL3 : String = "bool3"
//BOOL4 : String = "bool4"
//FLOAT : String = "float"
//FLOAT2 : String = "float2"
//FLOAT3 : String = "float3"
//FLOAT4 : String = "float4"
//INT : String = "int"
//INT2 : String = "int2"
//INT3 : String = "int3"
//INT4 : String = "int4"
//MATRIX2X2 : String = "matrix2x2"
//MATRIX3X3 : String = "matrix3x3"
//MATRIX4X4 : String = "matrix4x4"

	Stringp BOOL;
	Stringp BOOL2;
	Stringp BOOL3;
	Stringp BOOL4;
	Stringp FLOAT;
	Stringp FLOAT2;
	Stringp FLOAT3;
	Stringp FLOAT4;
	Stringp INT;
	Stringp INT2;
	Stringp INT3;
	Stringp INT4;
	Stringp MATRIX2X2;
	Stringp MATRIX3X3;
	Stringp MATRIX4X4;

private:};
class ShaderParameterTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ShaderParameterTypeClass : public ClassClosure
	{
	public:
		ShaderParameterTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderParameterTypeClassSlots m_slots_ShaderParameterTypeClass;
};
class ShaderParameterTypeObject : public ScriptObject
{
	public:
		ShaderParameterTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ShaderParameterTypeObjectSlots m_slots_ShaderParameterTypeObject;
};}
#endif