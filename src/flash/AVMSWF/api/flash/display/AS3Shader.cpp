#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Shader.h"
#include "AS3ShaderPrecision.h"

namespace avmshell{
ShaderClass::ShaderClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ShaderClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ShaderObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ShaderObject::ShaderObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	ShaderPrecisionClass*pClass=(ShaderPrecisionClass*)((ShellToplevel*)toplevel())->getBuiltinExtensionClass(abcclass_flash_display_ShaderPrecision,false);
	if(pClass)
		m_strHint=pClass->getFULL();
}
//////////////////////////////////////////////////////////
//Native Method start...
//void ShaderObject::AS3_byteCode_set(ByteArrayObject *pByteCode)
//{
//
//}
ShaderDataObject* ShaderObject::AS3_data_get()
{
	return m_pData;
}
void ShaderObject::AS3_data_set(ShaderDataObject *pData)
{
	m_pData=pData;
}
Stringp ShaderObject::AS3_precisionHint_get()
{
	return m_strHint;
}
void ShaderObject::AS3_precisionHint_set(Stringp pPrecisionHint)
{
	m_strHint=pPrecisionHint;
}

//void ShaderObject::AS3_constructor(ByteArrayObject *pCode)
//{
//
//}

}