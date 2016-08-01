#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DisplacementMapFilter.h"
namespace avmshell{
DisplacementMapFilterClass::DisplacementMapFilterClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())DisplacementMapFilterObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* DisplacementMapFilterClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DisplacementMapFilterObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DisplacementMapFilterObject::DisplacementMapFilterObject(VTable *vtable, ScriptObject* proto, int capacity): BitmapFilterObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...

}