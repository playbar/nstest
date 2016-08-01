#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ShaderFilter.h"
namespace avmshell{
ShaderFilterClass::ShaderFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ShaderFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ShaderFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ShaderFilterObject::ShaderFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
int ShaderFilterObject::AS3_bottomExtension_get()
{
	return 0;
}
void ShaderFilterObject::AS3_bottomExtension_set(int bottomExtension)
{

}
int ShaderFilterObject::AS3_leftExtension_get()
{
	return 0;
}
void ShaderFilterObject::AS3_leftExtension_set(int leftExtension)
{

}
int ShaderFilterObject::AS3_rightExtension_get()
{
	return 0;
}
void ShaderFilterObject::AS3_rightExtension_set(int rightExtension)
{

}
ShaderObject* ShaderFilterObject::AS3_shader_get()
{
	return NULL;
}
void ShaderFilterObject::AS3_shader_set(ShaderObject *pShader)
{

}
int ShaderFilterObject::AS3_topExtension_get()
{
	return 0;
}
void ShaderFilterObject::AS3_topExtension_set(int topExtension)
{

}

void ShaderFilterObject::AS3_constructor(ShaderObject *pShader)
{

}

}