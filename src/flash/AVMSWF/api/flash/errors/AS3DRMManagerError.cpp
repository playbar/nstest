#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3DRMManagerError.h"
namespace avmshell{
DRMManagerErrorClass::DRMManagerErrorClass(VTable* cvtable):ClassClosure(cvtable)//NativeErrorClass(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* DRMManagerErrorClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DRMManagerErrorObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
DRMManagerErrorObject::DRMManagerErrorObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorObject(vtable, proto)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//int DRMManagerErrorObject::AS3_subErrorID_get()
//{
//	//Add your act code here...
//	return 0;//Modify this please!
//}
//
////AS3 contructor function..
//void DRMManagerErrorObject::AS3_constructor(Stringp pMessage, int id, int subErrorID)
//{
//	//Add your act code here...
//}
//
//void DRMManagerErrorObject::AS3_toString()
//{
//	//Add your act code here...
//}

}