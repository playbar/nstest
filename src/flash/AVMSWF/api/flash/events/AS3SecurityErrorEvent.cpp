#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3SecurityErrorEvent.h"
namespace avmshell{
SecurityErrorEventClass::SecurityErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//ErrorEventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SecurityErrorEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SecurityErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SecurityErrorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

SecurityErrorEventObject* SecurityErrorEventClass::CreateEventObject(int id, Stringp pText)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, pText->atom()};

	SecurityErrorEventObject *pObj = (SecurityErrorEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pText);
	pObj->nEventID=id;
	return pObj;
}

Stringp SecurityErrorEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_securityError:
		return this->getSlotSECURITY_ERROR();
	default:
		{
			ErrorEventClass*pClass=((ShellToplevel*)toplevel())->getErrorEventClass();
			return pClass->IDToStringType(id);
		//return ErrorEventClass::IDToStringType(id);
		}
	}
}
	
SecurityErrorEventObject::SecurityErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
{
	//Add your construct code here...
//	m_pText = AvmCore::newConstantStringLatin1("");
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void SecurityErrorEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp text)
{
	//Add your act code here...
	return ErrorEventObject::AS3_constructor(type, bBubbles, bCancelable, text);
}

EventObject* SecurityErrorEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_SecurityErrorEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	return ErrorEventObject::Clone(pObj);
}

SecurityErrorEventObject* SecurityErrorEventObject::AS3_clone()
{
	//Add your act code here...
	return (SecurityErrorEventObject *)Clone(NULL);
}

Stringp SecurityErrorEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}