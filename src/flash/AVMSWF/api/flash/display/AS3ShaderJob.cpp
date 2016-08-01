#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ShaderJob.h"
namespace avmshell{
ShaderJobClass::ShaderJobClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ShaderJobClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ShaderJobObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ShaderJobObject::ShaderJobObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
int ShaderJobObject::AS3_height_get()
{
	return 0;
}
void ShaderJobObject::AS3_height_set(int height)
{

}
double ShaderJobObject::AS3_progress_get()
{
	return 0;
}
ShaderObject* ShaderJobObject::AS3_shader_get()
{
	return NULL;
}
void ShaderJobObject::AS3_shader_set(ShaderObject *pShader)
{

}
Atom ShaderJobObject::AS3_target_get()
{
	return kAvmThunkUndefined;
}
void ShaderJobObject::AS3_target_set(Atom target)
{

}
int ShaderJobObject::AS3_width_get()
{
	return 0;
}
void ShaderJobObject::AS3_width_set(int width)
{

}

//void ShaderJobObject::AS3_constructor(ShaderObject *pShader, Atom target, int width, int height)
//{
//
//}
void ShaderJobObject::AS3_cancel()
{

}
void ShaderJobObject::AS3_start(bool waitForCompletion)
{

}

}