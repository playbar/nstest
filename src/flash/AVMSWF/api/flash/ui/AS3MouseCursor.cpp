#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MouseCursor.h"
namespace avmshell{
MouseCursorClass::MouseCursorClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MouseCursorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MouseCursorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MouseCursorObject::MouseCursorObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}