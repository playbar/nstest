#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SQLColumnNameStyle.h"
namespace avmshell{
SQLColumnNameStyleClass::SQLColumnNameStyleClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SQLColumnNameStyleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SQLColumnNameStyleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SQLColumnNameStyleObject::SQLColumnNameStyleObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}