#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3NationalDigitsType.h"
namespace avmshell{
NationalDigitsTypeClass::NationalDigitsTypeClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NationalDigitsTypeClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NationalDigitsTypeObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NationalDigitsTypeObject::NationalDigitsTypeObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}