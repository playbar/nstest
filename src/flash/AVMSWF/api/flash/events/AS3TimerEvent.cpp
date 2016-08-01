#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3TimerEvent.h"
namespace avmshell{
TimerEventClass::TimerEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TimerEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TimerEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return (ScriptObject *)new (core()->GetGC(), ivtable->getExtraSize()) TimerEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

TimerEventObject* TimerEventClass::CreateEventObject(int id)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

	

	TimerEventObject *pObj = (TimerEventObject *)createInstance(ivtable(), prototype);
	//Atom atoms[] = {pObj->atom(),strType->atom(), false, false};
	pObj->AS3_constructor(strType,false,false);//NULL, 3, atoms);
	pObj->nEventID=id;
	return pObj;
}

Stringp TimerEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_timer:
		return this->getSlotTIMER();
	case XAS3_EVENT_timerComplete:
		return this->getSlotTIMER_COMPLETE();
	default:
		//return EventClass::IDToStringType(id);
		{
			EventClass*pClass=((ShellToplevel*)this->toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
	}
}

TimerEventObject::TimerEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void TimerEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable)
{
	//Add your act code here...
//	return EventObject::AS3_constructor();

	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject
	//	, argoff2 = argoff1 + AvmThunkArgSize_AvmString
	//	, argoff3 = argoff2 + AvmThunkArgSize_AvmBool32
	//};

	//Stringp strType = AvmThunkUnbox_AvmString(argv[argoff1]);
	//bool bBubbles = AvmThunkUnbox_AvmBool32(argv[argoff2]);
	//bool bCancelable = AvmThunkUnbox_AvmBool32(argv[argoff3]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);
}

EventObject* TimerEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_TimerEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	return EventObject::Clone(pObj);
}

TimerEventObject* TimerEventObject::AS3_clone()
{
	//Add your act code here...
	return (TimerEventObject *)Clone(NULL);
}

Stringp TimerEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

void TimerEventObject::AS3_updateAfterEvent()
{
	//Add your act code here...
	
}

}