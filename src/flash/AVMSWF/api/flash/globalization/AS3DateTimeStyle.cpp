#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DateTimeStyle.h"
namespace avmshell{
DateTimeStyleClass::DateTimeStyleClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DateTimeStyleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DateTimeStyleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DateTimeStyleObject::DateTimeStyleObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}