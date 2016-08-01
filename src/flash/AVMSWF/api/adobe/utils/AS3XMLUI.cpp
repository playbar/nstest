#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3XMLUI.h"
namespace avmshell{
XMLUIClass::XMLUIClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())XMLUIObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* XMLUIClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) XMLUIObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
XMLUIObject::XMLUIObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
void XMLUIClass::AS3_accept()
{
	//Add your act code here...
}

void XMLUIClass::AS3_cancel()
{
	//Add your act code here...
}

Stringp XMLUIClass::AS3_getProperty(Stringp name)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void XMLUIClass::AS3_setProperty(Stringp name, Stringp value)
{
	//Add your act code here...
}

}