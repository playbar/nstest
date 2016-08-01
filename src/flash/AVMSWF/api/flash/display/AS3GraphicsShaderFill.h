#ifndef _AS3GraphicsShaderFill_
#define _AS3GraphicsShaderFill_

#include "AS3Matrix.h"
#include "AS3Shader.h"

namespace avmplus{namespace NativeID{

class ShaderObject;
class MatrixObject;

class GraphicsShaderFillClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class GraphicsShaderFillObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	ShaderObject *shader;
	MatrixObject *matrix;
private:};
}}
namespace avmshell{
	class GraphicsShaderFillClass : public ClassClosure
	{
	public:
		GraphicsShaderFillClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsShaderFillClassSlots m_slots_GraphicsShaderFillClass;
};
class GraphicsShaderFillObject : public ScriptObject
{
	public:
		GraphicsShaderFillObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//MatrixObject* AS3_matrix_get();
		//void AS3_matrix_set( MatrixObject *pMatrix);
		//ShaderObject* AS3_shader_get();
		//void AS3_shader_set(ShaderObject *pShader);
		//void AS3_constructor(ShaderObject *pShader, MatrixObject *pMatrix);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GraphicsShaderFillObjectSlots m_slots_GraphicsShaderFillObject;
};}
#endif