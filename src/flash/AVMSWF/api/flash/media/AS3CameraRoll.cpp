#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CameraRoll.h"
namespace avmshell{
CameraRollClass::CameraRollClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* CameraRollClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CameraRollObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CameraRollObject::CameraRollObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool CameraRollObject::AS3_supportsAddBitmapData_get()
{
	return false;
}
bool CameraRollObject::AS3_supportsBrowseForImage_get()
{
	return false;
}

void CameraRollObject::AS3_constructor()
{

}
void CameraRollObject::AS3_addBitmapData(BitmapDataObject *pBitmapData)
{

}
void CameraRollObject::AS3_browseForImage()
{

}

}
