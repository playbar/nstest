#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3CameraUI.h"
namespace avmshell{
CameraUIClass::CameraUIClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* CameraUIClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) CameraUIObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
CameraUIObject::CameraUIObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool CameraUIObject::AS3_isSupported_get()
{
	//Add your act code here...
	return false;//Modify this please!
}

//AS3 contructor function..
void CameraUIObject::AS3_constructor()
{
	//Add your act code here...
}

void CameraUIObject::AS3_launch(Stringp pRequestedMediaType)
{
	//Add your act code here...
}

}
