#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMContentData.h"
#include "AS3DRMVoucher.h"

#include "AS3DRMVoucherStoreContext.h"
namespace avmshell{
DRMVoucherStoreContextClass::DRMVoucherStoreContextClass(VTable* cvtable):ClassClosure(cvtable)//DRMManagerSessionClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMVoucherStoreContextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMVoucherStoreContextObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMVoucherStoreContextObject::DRMVoucherStoreContextObject(VTable *vtable, ScriptObject* proto, int capacity): DRMManagerSessionObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}