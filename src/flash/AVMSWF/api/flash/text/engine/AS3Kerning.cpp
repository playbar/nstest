#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Kerning.h"
namespace avmshell{
KerningClass::KerningClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* KerningClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) KerningObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
KerningObject::KerningObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}