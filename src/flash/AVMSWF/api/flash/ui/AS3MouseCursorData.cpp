#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3MouseCursorData.h"
namespace avmshell{
MouseCursorDataClass::MouseCursorDataClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* MouseCursorDataClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MouseCursorDataObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MouseCursorDataObject::MouseCursorDataObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ObjectVectorObject* MouseCursorDataObject::AS3_data_get()
{
	return NULL;
}
void MouseCursorDataObject::AS3_data_set(ObjectVectorObject *pData)
{
}
double MouseCursorDataObject::AS3_frameRate_get()
{
	return 0;
}
void MouseCursorDataObject::AS3_frameRate_set(double frameRate)
{

}
PointObject* MouseCursorDataObject::AS3_hotSpot_get()
{
	return NULL;
}
void MouseCursorDataObject::AS3_hotSpot_set(PointObject *pHotSpot)
{

}

//void MouseCursorDataObject::AS3_constructor()
//{
//
//}

}