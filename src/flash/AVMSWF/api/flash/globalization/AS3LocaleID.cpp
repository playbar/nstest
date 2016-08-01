#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3LocaleID.h"
namespace avmshell{
LocaleIDClass::LocaleIDClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* LocaleIDClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) LocaleIDObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
LocaleIDObject::LocaleIDObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ObjectVectorObject* LocaleIDClass::AS3_determinePreferredLocales(ObjectVectorObject *pWant, ObjectVectorObject *pHave, Stringp keyword)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp LocaleIDObject::AS3_lastOperationStatus_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp LocaleIDObject::AS3_name_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

//AS3 contructor function..
void LocaleIDObject::AS3_constructor(Stringp name)
{
	//Add your act code here...
}

ScriptObject* LocaleIDObject::AS3_getKeysAndValues()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp LocaleIDObject::AS3_getLanguage()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp LocaleIDObject::AS3_getRegion()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp LocaleIDObject::AS3_getScript()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp LocaleIDObject::AS3_getVariant()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

bool LocaleIDObject::AS3_isRightToLeft()
{
	//Add your act code here...
	return false;//Modify this please!
}

}