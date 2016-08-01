#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3Screen.h"
namespace avmshell{
ScreenClass::ScreenClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* ScreenClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ScreenObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
ScreenObject::ScreenObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
ScreenObject* ScreenClass::AS3_mainScreen_get()
{
	return NULL;
}
ArrayObject* ScreenClass::AS3_screens_get()
{
	return NULL;
}
RectangleObject* ScreenObject::AS3_bounds_get()
{
	return NULL;
}
int ScreenObject::AS3_colorDepth_get()
{
	return 0;
}
RectangleObject* ScreenObject::AS3_visibleBounds_get()
{
	return NULL;
}

ArrayObject* ScreenObject::AS3_getScreensForRectangle(RectangleObject *pRect)
{
	return NULL;
}

}