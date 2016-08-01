#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsShaderFill.h"
namespace avmshell{
GraphicsShaderFillClass::GraphicsShaderFillClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsShaderFillClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsShaderFillObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsShaderFillObject::GraphicsShaderFillObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//MatrixObject* GraphicsShaderFillObject::AS3_matrix_get()
//{
//	return NULL;
//}
//void GraphicsShaderFillObject::AS3_matrix_set( MatrixObject *pMatrix)
//{
//
//}
//ShaderObject* GraphicsShaderFillObject::AS3_shader_get()
//{
//	return NULL;
//}
//void GraphicsShaderFillObject::AS3_shader_set(ShaderObject *pShader)
//{
//
//}
//void GraphicsShaderFillObject::AS3_constructor(ShaderObject *pShader, MatrixObject *pMatrix)
//{
//
//}

}