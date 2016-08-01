#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3StringTools.h"
namespace avmshell{
StringToolsClass::StringToolsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* StringToolsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StringToolsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
StringToolsObject::StringToolsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ObjectVectorObject* StringToolsClass::AS3_getAvailableLocaleIDNames()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp StringToolsObject::AS3_actualLocaleIDName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp StringToolsObject::AS3_lastOperationStatus_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp StringToolsObject::AS3_requestedLocaleIDName_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

//AS3 contructor function..
void StringToolsObject::AS3_constructor(Stringp requestedLocaleIDName)
{
	//Add your act code here...
}

Stringp StringToolsObject::AS3_toLowerCase(Stringp s)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

Stringp StringToolsObject::AS3_toUpperCase(Stringp s)
{
	//Add your act code here...
	return NULL;//Modify this please!
}

}