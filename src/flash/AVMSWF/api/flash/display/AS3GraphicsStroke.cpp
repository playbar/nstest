#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsStroke.h"
namespace avmshell{
GraphicsStrokeClass::GraphicsStrokeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsStrokeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsStrokeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsStrokeObject::GraphicsStrokeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Stringp GraphicsStrokeObject::AS3_caps_get()
//{
//	return NULL;
//}
//void GraphicsStrokeObject::AS3_caps_set(Stringp pCaps)
//{
//
//}
//AvmBox GraphicsStrokeObject::AS3_fill_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//AvmBox GraphicsStrokeObject::AS3_fill_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//Stringp GraphicsStrokeObject::AS3_joints_get()
//{
//	return NULL;
//}
//void GraphicsStrokeObject::AS3_joints_set(Stringp pJoints)
//{
//
//}
//double GraphicsStrokeObject::AS3_miterLimit_get()
//{
//	return 0;
//}
//void GraphicsStrokeObject::AS3_miterLimit_set(double miterLimit)
//{
//
//}
//bool GraphicsStrokeObject::AS3_pixelHinting_get()
//{
//	return false;
//}
//void GraphicsStrokeObject::AS3_pixelHinting_set(bool pixelHinting)
//{
//
//}
//Stringp GraphicsStrokeObject::AS3_scaleMode_get()
//{
//	return NULL;
//}
//void GraphicsStrokeObject::AS3_scaleMode_set(Stringp pScaleMode)
//{
//
//}
//double GraphicsStrokeObject::AS3_thickness_get()
//{
//	return 0;
//}
//void GraphicsStrokeObject::AS3_thickness_set(double thickness)
//{
//
//}
//
////AS3 contructor function..
//AvmBox GraphicsStrokeObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}