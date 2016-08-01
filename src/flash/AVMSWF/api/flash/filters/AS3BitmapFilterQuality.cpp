#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3BitmapFilterQuality.h"
namespace avmshell{
BitmapFilterQualityClass::BitmapFilterQualityClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BitmapFilterQualityObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BitmapFilterQualityClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BitmapFilterQualityObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
BitmapFilterQualityObject::BitmapFilterQualityObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}