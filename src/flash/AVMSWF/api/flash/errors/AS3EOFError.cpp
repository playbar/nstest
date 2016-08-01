#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3EOFError.h"
namespace avmshell{
EOFErrorClass::EOFErrorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())EOFErrorObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* EOFErrorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) EOFErrorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
EOFErrorObject::EOFErrorObject(VTable *vtable, ScriptObject* proto, int capacity): IOErrorObject(vtable, proto, 0)
{
	int v=sizeof(EOFErrorObject);
	int v1=sizeof(IOErrorObject);
	int vv=0;
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
AvmBox EOFErrorObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

}