#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3FontWeight.h"
namespace avmshell{
FontWeightClass::FontWeightClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* FontWeightClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FontWeightObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
FontWeightObject::FontWeightObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}