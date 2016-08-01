#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3ContextMenuEvent.h"
namespace avmshell{
ContextMenuEventClass::ContextMenuEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ContextMenuEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ContextMenuEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ContextMenuEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ContextMenuEventObject* ContextMenuEventClass::CreateEventObject(int id, InteractiveObjectObject *pMouseTarget, InteractiveObjectObject *pContextMenuOwner)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}


//	Atom atoms[] = {strType->atom(), false, false, pMouseTarget->atom(), pContextMenuOwner->atom()};

	ContextMenuEventObject *pObj = (ContextMenuEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pMouseTarget, pContextMenuOwner);
	pObj->nEventID=id;
	return pObj;
}

Stringp ContextMenuEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_menuItemSelect:
		return this->getSlotMENU_ITEM_SELECT();
	case XAS3_EVENT_menuSelect:
		return this->getSlotMENU_SELECT();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

ContextMenuEventObject::ContextMenuEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pContextMenuOwner = NULL;
	m_pMouseTarget = NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
InteractiveObjectObject* ContextMenuEventObject::AS3_contextMenuOwner_get()
{
	//Add your act code here...
	return m_pContextMenuOwner;
}

void ContextMenuEventObject::AS3_contextMenuOwner_set(InteractiveObjectObject *pContextMenuOwner)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject
	//};

	//m_pContextMenuOwner = (InteractiveObjectObject *)AvmThunkUnbox_AvmObject(argv[argoff1]);

	m_pContextMenuOwner = pContextMenuOwner;
}

InteractiveObjectObject* ContextMenuEventObject::AS3_mouseTarget_get()
{
	//Add your act code here...
	return m_pMouseTarget;
}

void ContextMenuEventObject::AS3_mouseTarget_set(InteractiveObjectObject *pMouseTarget)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject
	//};

	//m_pMouseTarget = (InteractiveObjectObject *)AvmThunkUnbox_AvmObject(argv[argoff1]);

	m_pMouseTarget = pMouseTarget;
}

//AS3 contructor function..
void ContextMenuEventObject::AS3_constructor(Stringp strType, bool bBubbles, bool bCancelable, 
											 InteractiveObjectObject *pContextMenuOwner, InteractiveObjectObject *pMouseTarget)
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
	//	, argoff5 = argoff4 + AvmThunkArgSize_AvmObject
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	//EventObject::AS3_constructor(strType, bBubbles, bCancelable);

	//m_pContextMenuOwner =(InteractiveObjectObject *) AvmThunkUnbox_AvmObject(argv[argoff5]);
	//m_pMouseTarget = (InteractiveObjectObject *)AvmThunkUnbox_AvmObject(argv[argoff4]);

	EventObject::AS3_constructor(strType, bBubbles, bCancelable);

	m_pContextMenuOwner = pContextMenuOwner;
	m_pMouseTarget = pMouseTarget;
}

EventObject* ContextMenuEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_ContextMenuEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register ContextMenuEventObject *pEvent = (ContextMenuEventObject *)pObj;

	pEvent->m_pMouseTarget = m_pMouseTarget;
	pEvent->m_pContextMenuOwner = m_pContextMenuOwner;

	return EventObject::Clone(pObj);
}

ContextMenuEventObject* ContextMenuEventObject::AS3_clone()
{
	//Add your act code here...
	return (ContextMenuEventObject *)Clone(NULL);
}

Stringp ContextMenuEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

//new adds
bool ContextMenuEventObject::AS3_isMouseTargetInaccessible_get()
{
	return m_isMouseTargetInaccessible;
}
void ContextMenuEventObject::AS3_isMouseTargetInaccessible_set(bool isMouseTargetInaccessible)
{
	m_isMouseTargetInaccessible = isMouseTargetInaccessible;
}
//new adds end

}