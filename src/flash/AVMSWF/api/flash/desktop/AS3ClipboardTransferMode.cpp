#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ClipboardTransferMode.h"
namespace avmshell{
ClipboardTransferModeClass::ClipboardTransferModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ClipboardTransferModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ClipboardTransferModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ClipboardTransferModeObject::ClipboardTransferModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
