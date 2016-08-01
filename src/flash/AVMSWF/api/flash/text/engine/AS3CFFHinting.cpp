#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CFFHinting.h"
namespace avmshell{
CFFHintingClass::CFFHintingClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* CFFHintingClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CFFHintingObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CFFHintingObject::CFFHintingObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}
