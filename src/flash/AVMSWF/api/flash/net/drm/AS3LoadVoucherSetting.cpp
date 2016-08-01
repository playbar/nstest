#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3LoadVoucherSetting.h"
namespace avmshell{
LoadVoucherSettingClass::LoadVoucherSettingClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* LoadVoucherSettingClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LoadVoucherSettingObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
LoadVoucherSettingObject::LoadVoucherSettingObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}