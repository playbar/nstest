#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ByteArray.h"

#include "AS3DRMContentData.h"
namespace avmshell{
DRMContentDataClass::DRMContentDataClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMContentDataClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMContentDataObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMContentDataObject::DRMContentDataObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...


}