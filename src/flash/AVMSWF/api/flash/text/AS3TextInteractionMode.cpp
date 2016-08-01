#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextInteractionMode.h"
namespace avmshell{
TextInteractionModeClass::TextInteractionModeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextInteractionModeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextInteractionModeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextInteractionModeObject::TextInteractionModeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}