#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ShaderParameter.h"
namespace avmshell{
ShaderParameterClass::ShaderParameterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ShaderParameterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ShaderParameterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ShaderParameterObject::ShaderParameterObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
int ShaderParameterObject::AS3_index_get()
{
	return 0;
}
Stringp ShaderParameterObject::AS3_type_get()
{
	return NULL;
}
ArrayObject* ShaderParameterObject::AS3_value_get()
{
	return NULL;
}
void ShaderParameterObject::AS3_value_set(ArrayObject *pValue)
{

}

//void ShaderParameterObject::AS3_constructor()
//{
//
//}

}