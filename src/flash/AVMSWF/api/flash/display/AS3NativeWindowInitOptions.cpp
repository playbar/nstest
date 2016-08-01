#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3NativeWindowInitOptions.h"
namespace avmshell{
NativeWindowInitOptionsClass::NativeWindowInitOptionsClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//Add your construct code here...
};
ScriptObject* NativeWindowInitOptionsClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NativeWindowInitOptionsObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
NativeWindowInitOptionsObject::NativeWindowInitOptionsObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_strRenderMode = ((ShellToplevel*)toplevel())->getNativeWindowRenderModeClass()->getSlotAUTO();
}
//////////////////////////////////////////////////////////
//Native Method start...
bool NativeWindowInitOptionsObject::AS3_maximizable_get()
{
	return false;
}
void NativeWindowInitOptionsObject::AS3_maximizable_set(bool maximizable)
{

}
bool NativeWindowInitOptionsObject::AS3_minimizable_get()
{
	return false;
}
void NativeWindowInitOptionsObject::AS3_minimizable_set(bool minimizable)
{

}
NativeWindowObject* NativeWindowInitOptionsObject::AS3_owner_get()
{
	return NULL;
}
void NativeWindowInitOptionsObject::AS3_owner_set(NativeWindowObject *pOwner)
{

}
bool NativeWindowInitOptionsObject::AS3_resizable_get()
{
	return false;
}
void NativeWindowInitOptionsObject::AS3_resizable_set(bool resizable)
{

}
Stringp NativeWindowInitOptionsObject::AS3_systemChrome_get()
{
	return NULL;
}
void NativeWindowInitOptionsObject::AS3_systemChrome_set(Stringp pSystemChrome)
{

}
bool NativeWindowInitOptionsObject::AS3_transparent_get()
{
	return false;
}
void NativeWindowInitOptionsObject::AS3_transparent_set(bool transparent)
{

}
Stringp NativeWindowInitOptionsObject::AS3_type_get()
{
	return NULL;
}
void NativeWindowInitOptionsObject::AS3_type_set(Stringp pType)
{

}

void NativeWindowInitOptionsObject::AS3_constructor()
{

}
//new adds 11
Stringp NativeWindowInitOptionsObject::AS3_renderMode_get()
{
//	LOGWHERE();
	return m_strRenderMode;//NULL;
}
void NativeWindowInitOptionsObject::AS3_renderMode_set(Stringp renderMode)
{

	//LOGWHERE();
	m_strRenderMode = renderMode;
}
//new adds 11 end

}