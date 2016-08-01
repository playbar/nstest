#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DateTimeNameStyle.h"
namespace avmshell{
DateTimeNameStyleClass::DateTimeNameStyleClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DateTimeNameStyleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DateTimeNameStyleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DateTimeNameStyleObject::DateTimeNameStyleObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}