#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3VoucherAccessInfo.h"
namespace avmshell{
VoucherAccessInfoClass::VoucherAccessInfoClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* VoucherAccessInfoClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) VoucherAccessInfoObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
VoucherAccessInfoObject::VoucherAccessInfoObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AvmBox VoucherAccessInfoObject::AS3_authenticationMethod_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox VoucherAccessInfoObject::AS3_displayName_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}
//
//AvmBox VoucherAccessInfoObject::AS3_domain_get(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}