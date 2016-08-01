#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CapsStyle.h"
namespace avmshell{
CapsStyleClass::CapsStyleClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())CapsStyleObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* CapsStyleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CapsStyleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CapsStyleObject::CapsStyleObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}