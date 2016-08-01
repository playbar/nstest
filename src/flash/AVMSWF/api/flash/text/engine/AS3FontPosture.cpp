#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FontPosture.h"
namespace avmshell{
FontPostureClass::FontPostureClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* FontPostureClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FontPostureObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FontPostureObject::FontPostureObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}