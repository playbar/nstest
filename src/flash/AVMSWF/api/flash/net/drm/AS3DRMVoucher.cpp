#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMVoucher.h"
namespace avmshell{
DRMVoucherClass::DRMVoucherClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMVoucherClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMVoucherObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMVoucherObject::DRMVoucherObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_strLicenseID = NULL;
	m_strPolicyID = NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
DRMPlaybackTimeWindowObject* DRMVoucherObject::AS3_createDRMPlaybackTimeWindow(double length, double start, double end)
{
	return NULL;
}
}