#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3GraphicsTrianglePath.h"
namespace avmshell{
GraphicsTrianglePathClass::GraphicsTrianglePathClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GraphicsTrianglePathClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GraphicsTrianglePathObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GraphicsTrianglePathObject::GraphicsTrianglePathObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//Stringp GraphicsTrianglePathObject::AS3_culling_get()
//{
//	//Add your act code here...
//	return NULL;//Modify this please!
//}
//
//void GraphicsTrianglePathObject::AS3_culling_set(Stringp pCulling)
//{
//	//Add your act code here...
//}
//
//AvmBox GraphicsTrianglePathObject::AS3_indices_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox GraphicsTrianglePathObject::AS3_indices_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox GraphicsTrianglePathObject::AS3_uvtData_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox GraphicsTrianglePathObject::AS3_uvtData_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox GraphicsTrianglePathObject::AS3_vertices_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox GraphicsTrianglePathObject::AS3_vertices_set(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
////AS3 contructor function..
//AvmBox GraphicsTrianglePathObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}