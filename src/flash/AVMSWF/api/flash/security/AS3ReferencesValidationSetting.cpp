#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ReferencesValidationSetting.h"
namespace avmshell{
ReferencesValidationSettingClass::ReferencesValidationSettingClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ReferencesValidationSettingClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ReferencesValidationSettingObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ReferencesValidationSettingObject::ReferencesValidationSettingObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}