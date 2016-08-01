#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3BitmapDataChannel.h"
namespace avmshell{
BitmapDataChannelClass::BitmapDataChannelClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())BitmapDataChannelObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* BitmapDataChannelClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) BitmapDataChannelObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
BitmapDataChannelObject::BitmapDataChannelObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}