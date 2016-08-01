#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ByteArray.h"
#include "AS3DRMVoucher.h"
#include "AS3DRMContentData.h"

#include "AS3DRMManager.h"
namespace avmshell{
DRMManagerClass::DRMManagerClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
	m_nNetWorkIdleTimeout = 120000;
};
ScriptObject* DRMManagerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMManagerObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMManagerObject::DRMManagerObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...


//new adds 11
double DRMManagerClass::AS3_networkIdleTimeout_get()
{
	//LOGWHERE();
	return m_nNetWorkIdleTimeout;//0;
}
void DRMManagerClass::AS3_networkIdleTimeout_set(double d)
{
	//LOGWHERE();
	m_nNetWorkIdleTimeout = d;
}

//new adds 11 end


//new adds 11

void DRMManagerObject::AS3_addToDeviceGroup(DRMDeviceGroupObject* pDeviceGroup, bool forceRefresh)
{
	LOGWHERE();

}

void DRMManagerObject::AS3_removeFromDeviceGroup(DRMDeviceGroupObject* pDeviceGroup)
{
	LOGWHERE();

}
void DRMManagerObject::AS3_resetDRMVouchers()
{
	LOGWHERE();

}
void DRMManagerObject::AS3_returnVoucher(Stringp inServerURL, bool immediateCommit, Stringp licenseID, Stringp policyID)
{
	LOGWHERE();

}
void DRMManagerObject::AS3_storeVoucher(ByteArrayObject* pVoucher)
{
	LOGWHERE();

}
//new adds 11 end
}