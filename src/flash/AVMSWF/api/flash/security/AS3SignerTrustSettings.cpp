#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SignerTrustSettings.h"
namespace avmshell{
SignerTrustSettingsClass::SignerTrustSettingsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SignerTrustSettingsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SignerTrustSettingsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SignerTrustSettingsObject::SignerTrustSettingsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}