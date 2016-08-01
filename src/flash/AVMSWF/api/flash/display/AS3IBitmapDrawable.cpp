#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3IBitmapDrawable.h"
namespace avmshell{
IBitmapDrawableClass::IBitmapDrawableClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* IBitmapDrawableClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IBitmapDrawableObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
IBitmapDrawableObject::IBitmapDrawableObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}