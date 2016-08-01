#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ContextMenuBuiltInItems.h"
namespace avmshell{
ContextMenuBuiltInItemsClass::ContextMenuBuiltInItemsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ContextMenuBuiltInItemsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ContextMenuBuiltInItemsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ContextMenuBuiltInItemsObject* ContextMenuBuiltInItemsClass::CreateItems()
{
	ContextMenuBuiltInItemsObject*pObject=
		(ContextMenuBuiltInItemsObject*)createInstance(ivtable(),prototype);
	return pObject;
}

ContextMenuBuiltInItemsObject::ContextMenuBuiltInItemsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}