#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Geolocation.h"
namespace avmshell{
GeolocationClass::GeolocationClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* GeolocationClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) GeolocationObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
GeolocationObject::GeolocationObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool GeolocationObject::AS3_isSupported_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

bool GeolocationObject::AS3_muted_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

//AS3 contructor function..
//AvmBox GeolocationObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

void GeolocationObject::AS3_setRequestedUpdateInterval(double interval)
{
	//Add your act code here...
}

}