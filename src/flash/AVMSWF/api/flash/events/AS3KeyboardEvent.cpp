#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3KeyboardEvent.h"
namespace avmshell{
KeyboardEventClass::KeyboardEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())KeyboardEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* KeyboardEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) KeyboardEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

KeyboardEventObject* KeyboardEventClass::CreateEventObject(int id, KEYEVENTDATA&data)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

	KeyboardEventObject *pObj = (KeyboardEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, data.nChar,data.nKey,
						  data.nLocaltion,data.bControl,data.bAlt,data.bShift);
		//charCode, keyCode, keyLocation, ctrlKey, altKey, shiftKey);
	pObj->nEventID=id;
	return pObj;
}

Stringp KeyboardEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_keyDown:
		return this->getSlotKEY_DOWN();
	case XAS3_EVENT_keyUp:
		return this->getSlotKEY_UP();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

KeyboardEventObject::KeyboardEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_charCode = 0;
	m_keyCode = 0;
	m_keyLocation = 0;

	m_ctrlKey = false;
	m_altKey = false;
	m_shiftKey = false;
}
//////////////////////////////////////////////////////////
//Native Method start...
bool KeyboardEventObject::AS3_altKey_get()
{
	//Add your act code here...
	return m_altKey;
}

void KeyboardEventObject::AS3_altKey_set(bool altKey)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};
	//m_altKey = AvmThunkUnbox_AvmBool32(argv[argoff1]);

	m_altKey = altKey;
}

uint32_t KeyboardEventObject::AS3_charCode_get()
{
	//Add your act code here...
	return m_charCode;
}

void KeyboardEventObject::AS3_charCode_set(uint32_t charCode)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};
	//m_charCode = AvmThunkUnbox_uint32_t(argv[argoff1]);

	m_charCode = charCode;
}

bool KeyboardEventObject::AS3_ctrlKey_get()
{
	//Add your act code here...
	return m_ctrlKey;
}

void KeyboardEventObject::AS3_ctrlKey_set(bool ctrlKey)
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

uint32_t KeyboardEventObject::AS3_keyCode_get()
{
	//Add your act code here...
	return m_keyCode;
}

void KeyboardEventObject::AS3_keyCode_set(uint32_t keyCode)
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

uint32_t KeyboardEventObject::AS3_keyLocation_get()
{
	//Add your act code here...
	return m_keyLocation;
}

void KeyboardEventObject::AS3_keyLocation_set(uint32_t keyLocation)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};
	//m_keyLocation = AvmThunkUnbox_uint32_t(argv[argoff1]);

	m_keyLocation = keyLocation;
}

bool KeyboardEventObject::AS3_shiftKey_get()
{
	//Add your act code here...
	return m_shiftKey;
}

void KeyboardEventObject::AS3_shiftKey_set(bool shiftKey)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};
	//m_shiftKey = AvmThunkUnbox_AvmBool32(argv[argoff1]);

	m_shiftKey = shiftKey;
}

//AS3 contructor function..
void KeyboardEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, 
										  uint32_t charCode, uint32_t keyCode, uint32_t keyLocation, 
										  bool ctrlKey, bool altKey, bool shiftKey)
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
	//	, argoff5 = argoff4 + AvmThunkArgSize_uint32_t
	//	, argoff6 = argoff5 + AvmThunkArgSize_uint32_t
	//	, argoff7 = argoff6 + AvmThunkArgSize_uint32_t
	//	, argoff8 = argoff7 + AvmThunkArgSize_AvmBool32
	//	, argoff9 = argoff8 + AvmThunkArgSize_AvmBool32
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);

	m_shiftKey		= shiftKey;
	m_altKey		= altKey;
	m_ctrlKey		= ctrlKey;
	m_keyLocation	= keyLocation;
	m_keyCode		= keyCode;
	m_charCode		= charCode;
}

EventObject* KeyboardEventObject::Clone(EventObject*pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_KeyboardEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register KeyboardEventObject *pEvent = (KeyboardEventObject *)pObj;
	pEvent->m_charCode = m_charCode;
	pEvent->m_keyCode = m_keyCode;
	pEvent->m_keyLocation = m_keyLocation;
	pEvent->m_ctrlKey = m_ctrlKey;
	pEvent->m_altKey = m_altKey;
	pEvent->m_shiftKey = m_shiftKey;

	return EventObject::Clone(pObj);
}

KeyboardEventObject* KeyboardEventObject::AS3_clone()
{
	//Add your act code here...
	return (KeyboardEventObject *)Clone(NULL);
}

Stringp KeyboardEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

void KeyboardEventObject::AS3_updateAfterEvent()
{
	//Add your act code here...
}

//new adds
bool KeyboardEventObject::AS3_commandKey_get()
{
	return false;
}
void KeyboardEventObject::AS3_commandKey_set(bool commandKey)
{
}
bool KeyboardEventObject::AS3_controlKey_get()
{
	return false;
}
void KeyboardEventObject::AS3_controlKey_set(bool controlKey)
{
}
//new adds end

}