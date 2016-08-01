#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SignatureStatus.h"
namespace avmshell{
SignatureStatusClass::SignatureStatusClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SignatureStatusClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SignatureStatusObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SignatureStatusObject::SignatureStatusObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}