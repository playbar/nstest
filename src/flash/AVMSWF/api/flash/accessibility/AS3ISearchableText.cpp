#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ISearchableText.h"
namespace avmshell{
ISearchableTextClass::ISearchableTextClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ISearchableTextClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ISearchableTextObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ISearchableTextObject::ISearchableTextObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}