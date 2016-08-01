#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3URLRequestMethod.h"
namespace avmshell{
URLRequestMethodClass::URLRequestMethodClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())URLRequestMethodObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* URLRequestMethodClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) URLRequestMethodObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
/*URLRequestMethodObject* URLRequestMethodClass::CreateURLRequestMethod()
{
	URLRequestMethodObject *pObj = (URLRequestMethodObject *)createInstance(ivtable(), prototype);

	return pObj;
}*/

URLRequestMethodObject::URLRequestMethodObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}