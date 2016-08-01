#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ShaderInput.h"
namespace avmshell{
ShaderInputClass::ShaderInputClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ShaderInputClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ShaderInputObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ShaderInputObject::ShaderInputObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
int ShaderInputObject::AS3_channels_get()
{
	return 0;
}
int ShaderInputObject::AS3_height_get()
{
	return 0;
}
void ShaderInputObject::AS3_height_set(int height)
{

}
int ShaderInputObject::AS3_index_get()
{
	return 0;
}
Atom ShaderInputObject::AS3_input_get()
{
	return kAvmThunkUndefined;
}
void ShaderInputObject::AS3_input_set(Atom input)
{

}
int ShaderInputObject::AS3_width_get()
{
	return 0;
}
void ShaderInputObject::AS3_width_set(int width)
{

}

void ShaderInputObject::AS3_constructor()
{

}
}