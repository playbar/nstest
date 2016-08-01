#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3StatusEvent.h"
namespace avmshell{
StatusEventClass::StatusEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())StatusEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* StatusEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) StatusEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

StatusEventObject* StatusEventClass::CreateEventObject(int id, Stringp pCode, Stringp pLevel)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, pCode->atom(), pLevel->atom()};

	StatusEventObject *pObj = (StatusEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pCode, pLevel);
	pObj->nEventID=id;
	return pObj;
}

Stringp StatusEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_status:
		return this->getSlotSTATUS();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

StatusEventObject::StatusEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto,0)
{
	//Add your construct code here...
	m_code = core()->newConstantStringLatin1("");
	m_level = core()->newConstantStringLatin1("");
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp StatusEventObject::AS3_code_get()
{
	//Add your act code here...
	return m_code;
}

void StatusEventObject::AS3_code_set(Stringp code)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	m_code = code;
}

Stringp StatusEventObject::AS3_level_get()
{
	//Add your act code here...
	return m_level;
}

void StatusEventObject::AS3_level_set(Stringp level)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	m_level = level;
}

//AS3 contructor function..
void StatusEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp code, Stringp level)
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
	//	, argoff5 = argoff4 + AvmThunkArgSize_AvmString
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);

	m_level = level;
	m_code = code;
}

EventObject* StatusEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_StatusEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register StatusEventObject *pEvent = (StatusEventObject *)pObj;

	pEvent->m_code = m_code;
	pEvent->m_level = m_level;

	return EventObject::Clone(pObj);
}

StatusEventObject* StatusEventObject::AS3_clone()
{
	//Add your act code here...
	return (StatusEventObject *)Clone(NULL);
}

Stringp StatusEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}