#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3JPEGLoaderContext.h"
namespace avmshell{
JPEGLoaderContextClass::JPEGLoaderContextClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* JPEGLoaderContextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) JPEGLoaderContextObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
JPEGLoaderContextObject::JPEGLoaderContextObject(VTable *vtable, ScriptObject* proto, int capacity): LoaderContextObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//double JPEGLoaderContextObject::AS3_deblockingFilter_get()
//{
//	return 0;
//}
//void JPEGLoaderContextObject::AS3_deblockingFilter_set(double deblockingFilter)
//{
//
//}
//
//void JPEGLoaderContextObject::AS3_constructor(double deblockingFilter, bool checkPolicyFile, 
//											  ApplicationDomainObject *pApplicationDomain, SecurityDomainObject *pSecurityDomain)
//{
//
//}

}