#include "StdAfxflash.h"
#include "avmplus.h"

using namespace avmplus;
#include "AS3NewObjectSample.h"

namespace avmshell{
NewObjectSampleClass::NewObjectSampleClass(VTable* cvtable):ClassClosure(cvtable)//SampleClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NewObjectSampleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NewObjectSampleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NewObjectSampleObject::NewObjectSampleObject(VTable *vtable, ScriptObject* proto, int capacity): SampleObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Atom NewObjectSampleObject::AS3_object_get()
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

double NewObjectSampleObject::AS3_size_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

}