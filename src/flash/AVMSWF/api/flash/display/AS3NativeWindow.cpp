#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3NativeWindow.h"
namespace avmshell{
NativeWindowClass::NativeWindowClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeWindowClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeWindowObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeWindowObject::NativeWindowObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pInitOptions = NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
bool NativeWindowObject::AS3_isSupported_get()
{
	return false;
}
bool NativeWindowObject::AS3_supportsMenu_get()
{
	return false;
}
bool NativeWindowObject::AS3_supportsNotification_get()
{
	return false;
}
bool NativeWindowObject::AS3_supportsTransparency_get()
{
	return false;
}
PointObject* NativeWindowObject::AS3_systemMaxSize_get()
{
	return NULL;
}
PointObject* NativeWindowObject::AS3_systemMinSize_get()
{
	return NULL;
}
bool NativeWindowObject::AS3_active_get()
{
	return false;
}
bool NativeWindowObject::AS3_alwaysInFront_get()
{
	return false;
}
void NativeWindowObject::AS3_alwaysInFront_set(bool alwaysInFront)
{

}
RectangleObject* NativeWindowObject::AS3_bounds_get()
{
	return NULL;
}
void NativeWindowObject::AS3_bounds_set(RectangleObject *pBounds)
{

}
bool NativeWindowObject::AS3_closed_get()
{
	return false;
}
Stringp NativeWindowObject::AS3_displayState_get()
{
	return NULL;
}
double NativeWindowObject::AS3_height_get()
{
	return 0;
}
void NativeWindowObject::AS3_height_set(double height)
{

}
bool NativeWindowObject::AS3_maximizable_get()
{
	return false;
}
PointObject* NativeWindowObject::AS3_maxSize_get()
{
	return NULL;
}
void NativeWindowObject::AS3_maxSize_set(PointObject *pMaxSize)
{

}
NativeMenuObject* NativeWindowObject::AS3_menu_get()
{
	return NULL;
}
void NativeWindowObject::AS3_menu_set(NativeMenuObject *pMenu)
{

}
bool NativeWindowObject::AS3_minimizable_get()
{
	return false;
}
PointObject* NativeWindowObject::AS3_minSize_get()
{
	return NULL;
}
void NativeWindowObject::AS3_minSize_set(PointObject *pMinSize)
{

}
NativeWindowObject* NativeWindowObject::AS3_owner_get()
{
	return NULL;
}
bool NativeWindowObject::AS3_resizable_get()
{
	return false;
}
StageObject* NativeWindowObject::AS3_stage_get()
{
	return NULL;
}
Stringp NativeWindowObject::AS3_systemChrome_get()
{
	return NULL;
}
Stringp NativeWindowObject::AS3_title_get()
{
	return NULL;
}
void NativeWindowObject::AS3_title_set(Stringp pTitle)
{

}
bool NativeWindowObject::AS3_transparent_get()
{
	return false;
}
Stringp NativeWindowObject::AS3_type_get()
{
	return NULL;
}
bool NativeWindowObject::AS3_visible_get()
{
	return false;
}
void NativeWindowObject::AS3_visible_set(bool visible)
{

}
double NativeWindowObject::AS3_width_get()
{
	return 0;
}
void NativeWindowObject::AS3_width_set(double width)
{

}
double NativeWindowObject::AS3_x_get()
{
	return 0;
}
void NativeWindowObject::AS3_x_set(double x)
{

}
double NativeWindowObject::AS3_y_get()
{
	return 0;
}
void NativeWindowObject::AS3_y_set(double y)
{

}

void NativeWindowObject::AS3_constructor(NativeWindowInitOptionsObject *pInitOptions)
{
	m_pInitOptions = pInitOptions;
}
void NativeWindowObject::AS3_activate()
{

}
void NativeWindowObject::AS3_close()
{

}
PointObject* NativeWindowObject::AS3_globalToScreen(PointObject *pGlobalPoint)
{
	return NULL;
}
AvmBox NativeWindowObject::AS3_listOwnedWindows(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return kAvmThunkUndefined;
}
void NativeWindowObject::AS3_maximize()
{

}
void NativeWindowObject::AS3_minimize()
{

}
void NativeWindowObject::AS3_notifyUser(Stringp pType)
{

}
bool NativeWindowObject::AS3_orderInBackOf(NativeWindowObject *pWindow)
{
	return false;
}
bool NativeWindowObject::AS3_orderInFrontOf(NativeWindowObject *pWindow)
{
	return false;
}
bool NativeWindowObject::AS3_orderToBack()
{
	return false;
}
bool NativeWindowObject::AS3_orderToFront()
{
	return false;
}
void NativeWindowObject::AS3_restore()
{

}
bool NativeWindowObject::AS3_startMove()
{
	return false;
}
bool NativeWindowObject::AS3_startResize(Stringp pEdgeOrCorner)
{
	return false;
}

//new adds 11
Stringp NativeWindowObject::AS3_renderMode_get()
{
//	LOGWHERE();
	if(m_pInitOptions)
		return m_pInitOptions->AS3_renderMode_get();//Stringp("auto");//
	return ((ShellToplevel*)toplevel())->getNativeWindowRenderModeClass()->getSlotAUTO();
}
	
//new adds 11 end
}