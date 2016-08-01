#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3FocusEvent.h"
namespace avmshell{
FocusEventClass::FocusEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())FocusEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* FocusEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FocusEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

FocusEventObject* FocusEventClass::CreateEventObject(int id, InteractiveObjectObject *pRelatedObject, bool shiftKey, uint32_t keyCode)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}


	FocusEventObject *pObj = (FocusEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pRelatedObject, shiftKey, keyCode);
	pObj->nEventID=id;
	return pObj;
}

Stringp FocusEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_focusIn:
		return this->getSlotFOCUS_IN();
	case XAS3_EVENT_focusOut:
		return this->getSlotFOCUS_OUT();
	case XAS3_EVENT_keyFocusChange:
		return this->getSlotKEY_FOCUS_CHANGE();
	case XAS3_EVENT_mouseFocusChange:
		return this->getSlotMOUSE_FOCUS_CHANGE();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

FocusEventObject::FocusEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pRelatedObject = NULL;
	m_shiftKey = false;
	m_keyCode = 0;
}
//////////////////////////////////////////////////////////
//Native Method start...
uint32_t FocusEventObject::AS3_keyCode_get()
{
	//Add your act code here...
	return m_keyCode;
}

void FocusEventObject::AS3_keyCode_set(uint32_t keyCode)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};
	//m_keyCode = AvmThunkUnbox_uint32_t(argv[argoff1]);

	m_keyCode = keyCode;
}

InteractiveObjectObject* FocusEventObject::AS3_relatedObject_get()
{
	//Add your act code here...
	return m_pRelatedObject;
}

void FocusEventObject::AS3_relatedObject_set(InteractiveObjectObject *pRelateObject)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};
	//m_pRelatedObject = (InteractiveObjectObject *)AvmThunkUnbox_AvmObject(argv[argoff1]);

	m_pRelatedObject = pRelateObject;
}

bool FocusEventObject::AS3_shiftKey_get()
{
	//Add your act code here...
	return m_shiftKey;
}

void FocusEventObject::AS3_shiftKey_set(bool shiftKey)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};
	//m_keyCode = AvmThunkUnbox_AvmBool32(argv[argoff1]);

	m_shiftKey = shiftKey;
}

//AS3 contructor function..
void FocusEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, InteractiveObjectObject *pRelatedObject, bool shiftKey, uint32_t keyCode)
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
	//	, argoff6 = argoff5 + AvmThunkArgSize_AvmBool32
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	//EventObject::AS3_constructor(strType, bBubbles, bCancelable);

	//m_keyCode = AvmThunkUnbox_uint32_t(argv[argoff6]);
	//m_shiftKey = AvmThunkUnbox_AvmBool32(argv[argoff5]);
	//m_pRelatedObject = (InteractiveObjectObject *)AvmThunkUnbox_AvmObject(argv[argoff4]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);

	m_keyCode = keyCode;
	m_shiftKey = shiftKey;
	m_pRelatedObject = pRelatedObject;
}

EventObject* FocusEventObject::Clone(EventObject*pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_FocusEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register FocusEventObject *pEvent = (FocusEventObject *)pObj;
	pEvent->m_pRelatedObject = m_pRelatedObject;
	pEvent->m_shiftKey = m_shiftKey;
	pEvent->m_keyCode = m_keyCode;

	return EventObject::Clone(pObj);
}

FocusEventObject* FocusEventObject::AS3_clone()
{
	//Add your act code here...
	return (FocusEventObject *)Clone(NULL);
}

Stringp FocusEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

//new adds
Stringp FocusEventObject::AS3_direction_get()
{
	return NULL;
}
void FocusEventObject::AS3_direction_set(Stringp pDirection)
{

}
bool FocusEventObject::AS3_isRelatedObjectInaccessible_get()
{
	return m_isRelatedObjectInaccessible;
}
void FocusEventObject::AS3_isRelatedObjectInaccessible_set(bool isRelatedObjectInaccessible)
{
	m_isRelatedObjectInaccessible = isRelatedObjectInaccessible;
}
//new adds end

}