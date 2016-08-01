#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TextLineCreationResult.h"
namespace avmshell{
TextLineCreationResultClass::TextLineCreationResultClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TextLineCreationResultClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextLineCreationResultObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TextLineCreationResultObject::TextLineCreationResultObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}