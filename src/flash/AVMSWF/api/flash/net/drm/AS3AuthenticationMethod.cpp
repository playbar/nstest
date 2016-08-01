#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3AuthenticationMethod.h"
namespace avmshell{
AuthenticationMethodClass::AuthenticationMethodClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* AuthenticationMethodClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) AuthenticationMethodObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
AuthenticationMethodObject::AuthenticationMethodObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
