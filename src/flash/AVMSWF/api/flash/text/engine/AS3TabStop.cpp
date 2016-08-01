#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3TabStop.h"
namespace avmshell{
TabStopClass::TabStopClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* TabStopClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TabStopObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
TabStopObject::TabStopObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp TabStopObject::AS3_alignment_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void TabStopObject::AS3_alignment_set(Stringp alignment)
{
	//Add your act code here...
}

Stringp TabStopObject::AS3_decimalAlignmentToken_get()
{
	//Add your act code here...
	return NULL;//Modify this please!
}

void TabStopObject::AS3_decimalAlignmentToken_set(Stringp decimalAlignmentToken)
{
	//Add your act code here...
}

double TabStopObject::AS3_position_get()
{
	//Add your act code here...
	return 0.0;//Modify this please!
}

void TabStopObject::AS3_position_set(double position)
{
	//Add your act code here...
}

//AS3 contructor function..
//AvmBox TabStopObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
//{
//	//Add your act code here...
//	return kAvmThunkUndefined;//Modify this please!
//}

}