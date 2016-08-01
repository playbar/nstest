#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3NetStatusEvent.h"
namespace avmshell{
NetStatusEventClass::NetStatusEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())NetStatusEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* NetStatusEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetStatusEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

NetStatusEventObject* NetStatusEventClass::CreateEventObject(int id, ScriptObject *info)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, info->atom()};

	NetStatusEventObject *pObj = (NetStatusEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, info);
	pObj->nEventID=id;
	return pObj;
}

Stringp NetStatusEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_netStatus:
		return this->getSlotNET_STATUS();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

NetStatusEventObject::NetStatusEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pInfo = NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
Atom NetStatusEventObject::AS3_info_get()
{
	//Add your act code here...
	return m_pInfo?m_pInfo->atom():nullObjectAtom;
}

void NetStatusEventObject::AS3_info_set(Atom atom)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	//m_pInfo = AvmThunkUnbox_AvmObject(argv[argoff1]);

	m_pInfo = AvmCore::atomToScriptObject(atom);//pInfo;
}

//AS3 contructor function..
void NetStatusEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, ScriptObject *pInfo)
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

	m_pInfo = pInfo;
}

EventObject* NetStatusEventObject::Clone(EventObject*pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_NetStatusEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register NetStatusEventObject *pEvent = (NetStatusEventObject *)pObj;

	pEvent->m_pInfo = m_pInfo;

	return EventObject::Clone(pObj);
}

NetStatusEventObject* NetStatusEventObject::AS3_clone()
{
	//Add your act code here...
	return (NetStatusEventObject *)Clone(NULL);
}

Stringp NetStatusEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}