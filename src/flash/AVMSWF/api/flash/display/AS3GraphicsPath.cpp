#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsPath.h"
namespace avmshell{
GraphicsPathClass::GraphicsPathClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsPathClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsPathObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsPathObject::GraphicsPathObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AvmBox GraphicsPathObject::AS3_commands_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//AvmBox GraphicsPathObject::AS3_commands_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//AvmBox GraphicsPathObject::AS3_data_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//AvmBox GraphicsPathObject::AS3_data_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//Stringp GraphicsPathObject::AS3_winding_get()
//{
//	return NULL;
//}
//void GraphicsPathObject::AS3_winding_set(Stringp pWinding)
//{
//
//}
//AvmBox GraphicsPathObject::AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//void GraphicsPathObject::AS3_curveTo(double controlX, double controlY, double anchorX, double anchorY)
//{
//
//}
//void GraphicsPathObject::AS3_lineTo(double x, double y)
//{
//
//}
//void GraphicsPathObject::AS3_moveTo(double x, double y)
//{
//
//}
//void GraphicsPathObject::AS3_wideLineTo(double x, double y)
//{
//
//}
//void GraphicsPathObject::AS3_wideMoveTo(double x, double y)
//{
//
//}

}