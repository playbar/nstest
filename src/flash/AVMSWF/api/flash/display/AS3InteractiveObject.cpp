#include "StdAfxflash.h"
#include "ShellCore.h"
#include "sobject.h"

using namespace avmplus;
#include "AS3InteractiveObject.h"
#include "AS3MouseEvent.h"
#include "AS3FocusEvent.h"
#include "AS3KeyboardEvent.h"

#include "avm2.h"

namespace avmshell{
InteractiveObjectClass::InteractiveObjectClass(VTable* cvtable):ClassClosure(cvtable)//DisplayObjectClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())InteractiveObjectObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* InteractiveObjectClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) InteractiveObjectObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
InteractiveObjectObject::InteractiveObjectObject(VTable *vtable, ScriptObject* proto, int capacity): DisplayObjectObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pContextMenu=NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
ContextMenuObject* InteractiveObjectObject::AS3_contextMenu_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	return m_pContextMenu;
}

void InteractiveObjectObject::AS3_contextMenu_set(ContextMenuObject*pMenu)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	m_pContextMenu=pMenu;
}

XBOOL InteractiveObjectObject::AS3_doubleClickEnabled_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsDoubleClicked();
	return XFALSE;
	//return kAvmThunkUndefined;//Modify this please!
}

void InteractiveObjectObject::AS3_doubleClickEnabled_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetDoubleClicked(b);
	//return kAvmThunkUndefined;//Modify this please!
}

XBOOL InteractiveObjectObject::AS3_focusRect_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsFocusRect();
	return XFALSE;	
}

void InteractiveObjectObject::AS3_focusRect_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetFocusRect(b);
	//return kAvmThunkUndefined;//Modify this please!
}

XBOOL InteractiveObjectObject::AS3_mouseEnabled_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsMouseEnabled();
	return XFALSE;
	//return kAvmThunkUndefined;//Modify this please!
}

void InteractiveObjectObject::AS3_mouseEnabled_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetMouseEnabled(b);
	//return kAvmThunkUndefined;//Modify this please!
}

XBOOL InteractiveObjectObject::AS3_tabEnabled_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsTabEnabled();
	return XFALSE;
	//return kAvmThunkUndefined;//Modify this please!
}

void InteractiveObjectObject::AS3_tabEnabled_set(bool b)
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
		m_pObject->SetTabEnabled(b);
}

int InteractiveObjectObject::AS3_tabIndex_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->tabIndex;
	return 0;
	//return kAvmThunkUndefined;//Modify this please!
}

void InteractiveObjectObject::AS3_tabIndex_set(int i)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->tabIndex=i;
//return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
void InteractiveObjectObject::AS3_constructor()
{
	//Add your act code here...
	DisplayObjectObject::AS3_constructor();
	//return kAvmThunkUndefined;//Modify this please!
}

/*int InteractiveObjectObject::MapSystemEvent(Stringp strName,int&nMax)
{
	nMax=XAS3_EVENT_MAX;
	ShellToplevel*t=(ShellToplevel*)toplevel();
	MouseEventClass	*pClass=t->getMouseEventClass();
	FocusEventClass *pFocus=t->getFocusEventClass();
	KeyboardEventClass*pKey=t->getKeyboardEventClass();
	if(strName==pClass->getSlotCLICK())
		return XAS3_EVENT_click;
	else if(strName==pClass->getSlotDOUBLE_CLICK_FORSPACE())
		return XAS3_EVENT_doubleClick;
	else if(strName==pFocus->getSlotFOCUS_IN())
		return XAS3_EVENT_focusIn;
	else if(strName==pFocus->getSlotFOCUS_OUT())
		return XAS3_EVENT_focusOut;
	else if(strName==pKey->getSlotKEY_DOWN())
		return XAS3_EVENT_keyDown;
	else if(strName==pFocus->getSlotKEY_FOCUS_CHANGE())
		return XAS3_EVENT_keyFocusChange;
	else if(strName==pKey->getSlotKEY_UP())
		return XAS3_EVENT_keyUp;
	else if(strName==pClass->getSlotMOUSE_DOWN())
		return XAS3_EVENT_mouseDown;
	else if(strName==pFocus->getSlotMOUSE_FOCUS_CHANGE())
		return XAS3_EVENT_mouseFocusChange;
	else if(strName==
	else
		return DisplayObjectObject::MapSystemEvent(strName,nMax);
}*/

//new adds
AccessibilityImplementationObject* InteractiveObjectObject::AS3_accessibilityImplementation_get()
{
	return NULL;
}
void InteractiveObjectObject::AS3_accessibilityImplementation_set(AccessibilityImplementationObject *pAccessibilityImplementation)
{

}
bool InteractiveObjectObject::AS3_needsSoftKeyboard_get()
{
	return false;
}
void InteractiveObjectObject::AS3_needsSoftKeyboard_set(bool bNeedsSoftKeyboard)
{

}
RectangleObject* InteractiveObjectObject::AS3_softKeyboardInputAreaOfInterest_get()
{
	return NULL;
}
void InteractiveObjectObject::AS3_softKeyboardInputAreaOfInterest_set(RectangleObject *pSoftKeyboardInputAreaOfInterest)
{

}

bool InteractiveObjectObject::AS3_requestSoftKeyboard()
{
	return false;
}
//new adds end

}