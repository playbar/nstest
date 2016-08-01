#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FontLookup.h"
namespace avmshell{
FontLookupClass::FontLookupClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* FontLookupClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FontLookupObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FontLookupObject::FontLookupObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}