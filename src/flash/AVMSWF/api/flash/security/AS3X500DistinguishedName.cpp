#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3X500DistinguishedName.h"
namespace avmshell{
X500DistinguishedNameClass::X500DistinguishedNameClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* X500DistinguishedNameClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) X500DistinguishedNameObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
X500DistinguishedNameObject::X500DistinguishedNameObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp X500DistinguishedNameObject::AS3_commonName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X500DistinguishedNameObject::AS3_organizationName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X500DistinguishedNameObject::AS3_organizationalUnitName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X500DistinguishedNameObject::AS3_localityName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X500DistinguishedNameObject::AS3_stateOrProvinceName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X500DistinguishedNameObject::AS3_countryName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X500DistinguishedNameObject::AS3_toString()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

}