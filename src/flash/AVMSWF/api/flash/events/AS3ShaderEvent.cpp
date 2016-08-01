#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ShaderEvent.h"
namespace avmshell{
ShaderEventClass::ShaderEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ShaderEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ShaderEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ShaderEventObject::ShaderEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//BitmapDataObject* ShaderEventObject::AS3_bitmapData_get()
//{
//	return NULL;
//}
//void ShaderEventObject::AS3_bitmapData_set(BitmapDataObject *pBitmapData)
//{
//
//}
//ByteArrayObject* ShaderEventObject::AS3_byteArray_get()
//{
//	return NULL;
//}
//void ShaderEventObject::AS3_byteArray_set(ByteArrayObject *pByteArray)
//{
//
//}
//AvmBox ShaderEventObject::AS3_vector_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//AvmBox ShaderEventObject::AS3_vector_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
//{
//	return kAvmThunkUndefined;
//}
//
////AvmBox ShaderEventObject::AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
////{
////	return kAvmThunkUndefined;
////}
//
//void ShaderEventObject::AS3_constructor(Stringp s)
//{
//
//}
//
//EventObject* ShaderEventObject::AS3_clone()
//{
//	return NULL;
//}
//Stringp ShaderEventObject::AS3_toString()
//{
//	return NULL;
//}

}