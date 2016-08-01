#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3ClipboardFormats.h"
namespace avmshell{
ClipboardFormatsClass::ClipboardFormatsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ClipboardFormatsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ClipboardFormatsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ClipboardFormatsObject::ClipboardFormatsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
