#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DeleteObjectSample.h"
namespace avmshell{
DeleteObjectSampleClass::DeleteObjectSampleClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DeleteObjectSampleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DeleteObjectSampleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DeleteObjectSampleObject::DeleteObjectSampleObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}