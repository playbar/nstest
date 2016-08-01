#include "StdAfxflash.h"
#include "splayer.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Mouse.h"
#include "AS3MouseCursor.h"


namespace avmshell{
MouseClass::MouseClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())MouseObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* MouseClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MouseObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MouseObject::MouseObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
void MouseClass::AS3_hide()
{
	//Add your act code here...
	ShellCore*pCore=(ShellCore*)core();
	pCore->GetPlayer()->ShowCursor(XFALSE);
}

void MouseClass::AS3_show()
{
	//Add your act code here...
	ShellCore*pCore=(ShellCore*)core();
	pCore->GetPlayer()->ShowCursor(XTRUE);
}

//new adds
Stringp MouseClass::AS3_cursor_get()
{
	if(m_cursor==NULL)
	{
		MouseCursorClass*pClass=(MouseCursorClass*)((ShellToplevel*)toplevel())->getBuiltinExtensionClass(abcclass_flash_ui_MouseCursor,true);

		m_cursor = pClass->getSlotARROW();
	}
	return m_cursor;
}
void MouseClass::AS3_cursor_set(Stringp cursor)
{
	m_cursor = cursor;
}
bool MouseClass::AS3_supportsCursor_get()
{
	return false;
}
bool MouseClass::AS3_supportsNativeCursor_get()
{
	return false;
}

void MouseClass::AS3_registerCursor(Stringp pName, MouseCursorDataObject *pCursor)
{

}
void MouseClass::AS3_unregisterCursor(Stringp pName)
{

}
//new adds end

}