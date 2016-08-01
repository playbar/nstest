#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ByteArray.h"

#include "AS3Clipboard.h"
namespace avmshell{
ClipboardClass::ClipboardClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ClipboardClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ClipboardObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ClipboardObject::ClipboardObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
