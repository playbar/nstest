#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IPVersion.h"
namespace avmshell{
IPVersionClass::IPVersionClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IPVersionClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IPVersionObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IPVersionObject::IPVersionObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}