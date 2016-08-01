#include "StdAfxflash.h"
#include "ShellCore.h"
#include "splayer.h"
using namespace avmplus;

#include "avm2.h"
#include "AS3MouseEvent.h"
namespace avmshell{
MouseEventClass::MouseEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())MouseEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* MouseEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MouseEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

MouseEventObject* MouseEventClass::CreateEventObject(int id, void*d)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

	//Atom atoms[] = {
	//					strType->atom(), false, false, core()->doubleToAtom(localX), core()->doubleToAtom(localY),
	//					relatedObject->atom(), ctrlKey, altKey, shiftKey, buttonDown, delta 
	//			   };
	MOUSEEVENTDATA&data=*(MOUSEEVENTDATA*)d;
	MouseEventObject *pObj = (MouseEventObject *)createInstance(ivtable(), prototype);
	//pObj->AS3_constructor(NULL, 11, atoms);
	pObj->nEventID=id;
	pObj->strType=strType;
	//pObj->pTarget=(ScriptObject*)data.pTarget;
	pObj->m_pRelatedObject=(InteractiveObjectObject*)data.pRelatedObject;
	pObj->m_buttonDown=data.buttonDown;
	pObj->m_delta=data.delta;
	pObj->m_localX=data.localX;
	pObj->m_localY=data.localY;
	pObj->m_stageX=data.stageX;
	pObj->m_stageY=data.stageY;
	pObj->nEventID=id;
	pObj->bWithTarget=data.bWithTarget;
	pObj->nCreateTime=((ShellCore*)core())->GetPlayer()->m_nRunTime;
	((ShellCore*)core())->GetPlayer()->m_nRunTime++;
	return pObj;
}

Stringp MouseEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_click:
		return this->getSlotCLICK();
	case XAS3_EVENT_doubleClick:
		return this->getSlotDOUBLE_CLICK_FORSPACE();
	case XAS3_EVENT_mouseDown:
		return this->getSlotMOUSE_DOWN();
	case XAS3_EVENT_mouseMove:
		return this->getSlotMOUSE_MOVE();
	case XAS3_EVENT_mouseOut:
		return this->getSlotMOUSE_OUT();
	case XAS3_EVENT_mouseOver:
		return this->getSlotMOUSE_OVER();
	case XAS3_EVENT_mouseUp:
		return this->getSlotMOUSE_UP();
	case XAS3_EVENT_mouseWheel:
		return this->getSlotMOUSE_WHEEL();
	case XAS3_EVENT_rollOut:
		return this->getSlotROLL_OUT();
	case XAS3_EVENT_rollOver:
		return this->getSlotROLL_OVER();
	case XAS3_EVENT_contextMenu:
		return getSlotCONTEXT_MENU();
	case XAS3_EVENT_middleClick:
		return getSlotMIDDLE_CLICK();
	case XAS3_EVENT_middleMouseDown:
		return getSlotMIDDLE_MOUSE_DOWN();
	case XAS3_EVENT_middleMouseUp:
		return getSlotMIDDLE_MOUSE_UP();
	case XAS3_EVENT_rightClick:
		return getSlotRIGHT_CLICK();
	case XAS3_EVENT_rightMouseDown:
		return getSlotRIGHT_MOUSE_DOWN();
	case XAS3_EVENT_rightMouseUp:
		return getSlotRIGHT_MOUSE_UP();
	case XAS3_EVENT_releaseOutside:
		return getSlotRELEASE_OUTSIDE();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

MouseEventObject::MouseEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_altKey = false;
	m_buttonDown = false;
	m_ctrlKey = false;
	m_shiftKey = false;

	m_delta = 0;

	//m_localX = 0.0;
	//m_localY = 0.0;
	m_stageX = 0.0;
	m_stageY = 0.0;

	m_pRelatedObject = NULL;

	m_nMovementX = 0;
	m_nMovementX = 0;
}
//////////////////////////////////////////////////////////
//Native Method start...
bool MouseEventObject::AS3_altKey_get()
{
	//Add your act code here...
	return m_altKey;
}

void MouseEventObject::AS3_altKey_set(bool altKey)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};
	//
	//m_altKey = AvmThunkUnbox_AvmBool32(argv[argoff1]);

	m_altKey = altKey;
}

bool MouseEventObject::AS3_buttonDown_get()
{
	//Add your act code here...
	return m_buttonDown;
}

void MouseEventObject::AS3_buttonDown_set(bool buttonDown)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	//m_buttonDown = AvmThunkUnbox_AvmBool32(argv[argoff1]);

	m_buttonDown = buttonDown;
}

bool MouseEventObject::AS3_ctrlKey_get()
{
	//Add your act code here...
	return m_ctrlKey;
}

void MouseEventObject::AS3_ctrlKey_set(bool ctrlKey)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	//m_ctrlKey = AvmThunkUnbox_AvmBool32(argv[argoff1]);

	m_ctrlKey = ctrlKey;
}

int32_t MouseEventObject::AS3_delta_get()
{
	//Add your act code here...
	return m_delta;
}

void MouseEventObject::AS3_delta_set(int32_t delta)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	//m_altKey = AvmThunkUnbox_AvmBool32(argv[argoff1]);

	m_delta = delta;
}

double MouseEventObject::AS3_localX_get()
{
	//Add your act code here...
	/*DisplayObjectObject*pObj=(DisplayObjectObject*)(ScriptObject*)this->pCurrentTarget;
	if(pObj&&pObj->m_pObject)
	{
		ShellCore*c=(ShellCore*)core();
		XSWFPlayer*player=c->GetPlayer();
		double x,y;
		player->GetMousePos(pObj->m_pObject,x,y);
		return x;
	}
	return 0;*/
	return m_localX;//core()->doubleToAtom(m_localX);
}

void MouseEventObject::AS3_localX_set(double localX)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	//m_localX = AvmThunkUnbox_double(argv[argoff1]);

	m_localX = localX;
}

double MouseEventObject::AS3_localY_get()
{
	//Add your act code here...
	return m_localY;
	//return core()->doubleToAtom(m_localY);
}

void MouseEventObject::AS3_localY_set(double localY)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	//m_localY = AvmThunkUnbox_double(argv[argoff1]);

	m_localY = localY;
}

InteractiveObjectObject* MouseEventObject::AS3_relatedObject_get()
{
	//Add your act code here...
	if(m_pRelatedObject!=NULL)
		int v=0;
	return m_pRelatedObject;
}

void MouseEventObject::AS3_relatedObject_set(InteractiveObjectObject *pRelatedObject)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	//m_pRelatedObject = (InteractiveObjectObject *)AvmThunkUnbox_AvmObject(argv[argoff1]);
	//if(pRelatedObject!=NULL)
	//	int v=0;

	m_pRelatedObject = pRelatedObject;
}

bool MouseEventObject::AS3_shiftKey_get()
{
	//Add your act code here...
	return m_shiftKey;
}

void MouseEventObject::AS3_shiftKey_set(bool shiftKey)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	//m_localX = AvmThunkUnbox_AvmBool32(argv[argoff1]);

	m_shiftKey = shiftKey;
}

double MouseEventObject::AS3_stageX_get()
{
	//Add your act code here...
	return m_stageX;
}

double MouseEventObject::AS3_stageY_get()
{
	//Add your act code here...
	return m_stageY;
}

//AS3 contructor function..
void MouseEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, 
									   double localX, double localY, 
									   InteractiveObjectObject *pRelatedObject, 
									   bool ctrlkey, bool altKey, bool shiftKey, bool buttonDown, 
									   int32_t delta)
{
	//Add your act code here...
//	EventObject::AS3_constructor(env, argc, argv);

	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject
	//	, argoff2 = argoff1 + AvmThunkArgSize_AvmString
	//	, argoff3 = argoff2 + AvmThunkArgSize_AvmBool32
	//	, argoff4 = argoff3 + AvmThunkArgSize_AvmBool32
	//	, argoff5 = argoff4 + AvmThunkArgSize_double
	//	, argoff6 = argoff5 + AvmThunkArgSize_double
	//	, argoff7 = argoff6 + AvmThunkArgSize_AvmObject
	//	, argoff8 = argoff7 + AvmThunkArgSize_AvmBool32
	//	, argoff9 = argoff8 + AvmThunkArgSize_AvmBool32
	//	, argoff10 = argoff9 + AvmThunkArgSize_AvmBool32
	//	, argoff11 = argoff10 + AvmThunkArgSize_AvmBool32
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);

	m_delta = delta;
	m_buttonDown = buttonDown;
	m_shiftKey = shiftKey;
	m_altKey = altKey;
	m_ctrlKey = ctrlkey;
	m_pRelatedObject = pRelatedObject;
	m_localY = localY;
	m_localX = localX;
}

EventObject* MouseEventObject::Clone(EventObject*pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_MouseEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register MouseEventObject *pEvent = (MouseEventObject *)pObj;
	pEvent->m_localX = m_localX;
	pEvent->m_localY = m_localY;
	pEvent->m_stageX = m_stageX;
	pEvent->m_stageY = m_stageY;
	pEvent->m_altKey = m_altKey;
	pEvent->m_ctrlKey = m_ctrlKey;
	pEvent->m_shiftKey = m_shiftKey;
	pEvent->m_buttonDown = m_buttonDown;
	pEvent->m_pRelatedObject = m_pRelatedObject;
	pEvent->m_delta = m_delta;

	return EventObject::Clone(pObj);
}

MouseEventObject* MouseEventObject::AS3_clone()
{
	//Add your act code here...
	return (MouseEventObject *)Clone(NULL);
}

Stringp MouseEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

void MouseEventObject::AS3_updateAfterEvent()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->display.m_bInvalidate=true;
}

//new adds
int MouseEventObject::AS3_clickCount_get()
{
	return 0;
}
bool MouseEventObject::AS3_commandKey_get()
{
	return false;
}
void MouseEventObject::AS3_commandKey_set(bool commandKey)
{

}
bool MouseEventObject::AS3_controlKey_get()
{
	return false;
}
void MouseEventObject::AS3_controlKey_set(bool controlKey)
{

}
bool MouseEventObject::AS3_isRelatedObjectInaccessible_get()
{
	return m_isRelatedObjectInaccessible;
}
void MouseEventObject::AS3_isRelatedObjectInaccessible_set(bool isRelatedObjectInaccessible)
{
	m_isRelatedObjectInaccessible = isRelatedObjectInaccessible;
}
//new adds end

//new adds 11
double MouseEventObject::AS3_movementX_get()
{
	//LOGWHERE();
	return m_nMovementX;//0;
}
void MouseEventObject::AS3_movementX_set(double x)
{

	//LOGWHERE();
	m_nMovementX = x;
}
double MouseEventObject::AS3_movementY_get()
{
	//LOGWHERE();
	return m_nMovementY;//0;
}
void MouseEventObject::AS3_movementY_set(double y)
{
	//LOGWHERE();
	m_nMovementY = y;
}
//new adds 11 end

}