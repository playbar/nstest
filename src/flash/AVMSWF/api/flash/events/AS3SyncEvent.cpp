#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"

using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3SyncEvent.h"
namespace avmshell{
SyncEventClass::SyncEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SyncEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SyncEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SyncEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

SyncEventObject* SyncEventClass::CreateEventObject(int id, ArrayObject *pChangeList)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

	Atom atoms[] = {strType->atom(), false, false, pChangeList->atom()};

	SyncEventObject *pObj = (SyncEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pChangeList);
	pObj->nEventID=id;
	return pObj;
}

Stringp SyncEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_sync:
		return this->getSlotSYNC();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
			//return EventClass::IDToStringType(id);
		}
	}
}

SyncEventObject::SyncEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pChangeList = NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* SyncEventObject::AS3_changeList_get()
{
	//Add your act code here...
	return m_pChangeList;
}

void SyncEventObject::AS3_changeList_set(ArrayObject *pChangeList)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	m_pChangeList = pChangeList;
}

//AS3 contructor function..
void SyncEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, ArrayObject *pChangeList)
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
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);

	m_pChangeList = pChangeList;
}

EventObject* SyncEventObject::Clone(EventObject* pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_SyncEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register SyncEventObject *pEvent = (SyncEventObject *)pObj;

	pEvent->m_pChangeList = m_pChangeList;

	return EventObject::Clone(pObj);
}

SyncEventObject* SyncEventObject::AS3_clone()
{
	//Add your act code here...
	return (SyncEventObject *)Clone(NULL);
}

Stringp SyncEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}