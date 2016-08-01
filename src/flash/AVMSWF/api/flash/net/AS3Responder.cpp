#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Responder.h"
#include "AS3ByteArray.h"
namespace avmshell{
ResponderClass::ResponderClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ResponderObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ResponderClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ResponderObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ResponderObject* ResponderClass::CreateResponder(FunctionObject *pResult, FunctionObject *pStatus)
{
	ResponderObject *pObj = (ResponderObject *)createInstance(ivtable(), prototype);

	pObj->AS3_constructor(pResult, pStatus);

	return pObj;
}

ResponderObject::ResponderObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
//m_inData=((ShellToplevel*)toplevel())->getByteArrayClass()->CreateArray(0);
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void ResponderObject::AS3_constructor(FunctionObject *pResult, FunctionObject *pStatus)
{
	//Add your act code here...
	m_pResultCall=pResult;
	m_pStatusCall=pStatus;
}

}