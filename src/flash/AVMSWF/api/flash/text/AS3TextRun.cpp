#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextRun.h"
namespace avmshell{
TextRunClass::TextRunClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextRunClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextRunObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextRunObject::TextRunObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
