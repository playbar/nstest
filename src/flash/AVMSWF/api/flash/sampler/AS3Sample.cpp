#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Sample.h"
namespace avmshell{
SampleClass::SampleClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SampleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SampleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SampleObject::SampleObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}