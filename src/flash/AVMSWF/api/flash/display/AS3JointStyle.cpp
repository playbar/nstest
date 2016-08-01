#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3JointStyle.h"
namespace avmshell{
JointStyleClass::JointStyleClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())JointStyleObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* JointStyleClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) JointStyleObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
JointStyleObject::JointStyleObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
}