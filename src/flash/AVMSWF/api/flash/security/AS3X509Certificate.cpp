#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3X509Certificate.h"
namespace avmshell{
X509CertificateClass::X509CertificateClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* X509CertificateClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) X509CertificateObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
X509CertificateObject::X509CertificateObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
uint32_t X509CertificateObject::AS3_version_get()
{
	//Add your act code here...
	return 0;//Modify this please!
}

Stringp X509CertificateObject::AS3_serialNumber_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X509CertificateObject::AS3_signatureAlgorithmOID_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

ByteArrayObject* X509CertificateObject::AS3_signatureAlgorithmParams_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

X500DistinguishedNameObject* X509CertificateObject::AS3_issuer_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

DateObject* X509CertificateObject::AS3_validNotBefore_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

DateObject* X509CertificateObject::AS3_validNotAfter_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

X500DistinguishedNameObject* X509CertificateObject::AS3_subject_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X509CertificateObject::AS3_subjectPublicKeyAlgorithmOID_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X509CertificateObject::AS3_subjectPublicKey_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X509CertificateObject::AS3_issuerUniqueID_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp X509CertificateObject::AS3_subjectUniqueID_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

ByteArrayObject* X509CertificateObject::AS3_encoded_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

}