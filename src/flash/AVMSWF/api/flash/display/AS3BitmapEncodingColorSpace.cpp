#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;

#include "AS3BitmapEncodingColorSpace.h"

namespace avmshell
{
	BitmapEncodingColorSpaceClass::BitmapEncodingColorSpaceClass(VTable* cvtable):ClassClosure(cvtable)
	{
		createVanillaPrototype();
		//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextFieldAutoSizeObject(cvtable, toplevel()->objectClass->prototype, 0);
		//Add your construct code here...
	};
	ScriptObject* BitmapEncodingColorSpaceClass::createInstance(VTable* ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) BitmapEncodingColorSpaceObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	BitmapEncodingColorSpaceObject::BitmapEncodingColorSpaceObject(VTable* vtable, ScriptObject* proto, int capacity):ScriptObject(vtable,proto,0)
	{
		//Add your construct code here...
	}
}