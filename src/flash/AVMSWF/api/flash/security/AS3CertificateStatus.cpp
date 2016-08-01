#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CertificateStatus.h"
namespace avmshell{
CertificateStatusClass::CertificateStatusClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* CertificateStatusClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CertificateStatusObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CertificateStatusObject::CertificateStatusObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
