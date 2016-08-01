#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3RevocationCheckSettings.h"
namespace avmshell{
RevocationCheckSettingsClass::RevocationCheckSettingsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* RevocationCheckSettingsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) RevocationCheckSettingsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
RevocationCheckSettingsObject::RevocationCheckSettingsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}