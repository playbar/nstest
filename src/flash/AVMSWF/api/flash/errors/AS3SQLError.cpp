#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3SQLError.h"
namespace avmshell{
SQLErrorClass::SQLErrorClass(VTable* cvtable):ClassClosure(cvtable)//NativeErrorClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* SQLErrorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SQLErrorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SQLErrorObject::SQLErrorObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorObject(vtable, proto)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* SQLErrorObject::AS3_detailArguments_get()
{
	return NULL;
}
int SQLErrorObject::AS3_detailID_get()
{
	return 0;
}
Stringp SQLErrorObject::AS3_details_get()
{
	return NULL;
}
Stringp SQLErrorObject::AS3_operation_get()
{
	return NULL;
}

void SQLErrorObject::AS3_constructor(Stringp pOperation, Stringp pDetails, String pMessage, int id, int detailID, ArrayObject *pDetailArgs)
{

}
void SQLErrorObject::AS3_toString()
{

}

}