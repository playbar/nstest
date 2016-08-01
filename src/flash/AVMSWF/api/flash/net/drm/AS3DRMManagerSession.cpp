#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMContentData.h"
#include "AS3DRMVoucher.h"

#include "AS3DRMManagerSession.h"
namespace avmshell{
DRMManagerSessionClass::DRMManagerSessionClass(VTable* cvtable):ClassClosure(cvtable)//EventDispatcherClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMManagerSessionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMManagerSessionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMManagerSessionObject::DRMManagerSessionObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
uint32_t DRMManagerSessionObject::AS3_getLastError()
{
	//Add your act code here...
	return 0;//Modify this please!
}

uint32_t DRMManagerSessionObject::AS3_getLastSubErrorID()
{
	//Add your act code here...
	return 0;//Modify this please!
}

void DRMManagerSessionObject::AS3_issueDRMErrorEvent(DRMContentDataObject *pMetadata, int errorID, int subErrorID, Stringp eventType)
{
	//Add your act code here...
}

void DRMManagerSessionObject::AS3_errorCodeToThrow(uint32_t errorCode)
{
	//Add your act code here...
}

}