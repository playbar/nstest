#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3IOErrorEvent.h"
namespace avmshell{
IOErrorEventClass::IOErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//ErrorEventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())IOErrorEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* IOErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IOErrorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

IOErrorEventObject* IOErrorEventClass::CreateEventObject(int id, Stringp pText)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, pText->atom()};

	IOErrorEventObject *pObj = (IOErrorEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pText);
	pObj->nEventID=id;
	return pObj;
}

Stringp IOErrorEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_ioError:
		return this->getSlotIO_ERROR();
	default:
		{
			ErrorEventClass*pClass=((ShellToplevel*)toplevel())->getErrorEventClass();
			return pClass->IDToStringType(id);
		}
		//return ErrorEventClass::IDToStringType(id);
	}
}

IOErrorEventObject::IOErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): ErrorEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void IOErrorEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp pText)
{
	//Add your act code here...
	ErrorEventObject::AS3_constructor(type, bBubbles, bCancelable, pText);
}

EventObject* IOErrorEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_IOErrorEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	return ErrorEventObject::Clone(pObj);
}

IOErrorEventObject* IOErrorEventObject::AS3_clone()
{
	//Add your act code here...
	return (IOErrorEventObject *)Clone(NULL);
}

Stringp IOErrorEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}