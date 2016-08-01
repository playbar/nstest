#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3ErrorEvent.h"
namespace avmshell{
ErrorEventClass::ErrorEventClass(VTable* cvtable):ClassClosure(cvtable)//TextEventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ErrorEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* ErrorEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ErrorEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ErrorEventObject* ErrorEventClass::CreateEventObject(int id, Stringp pText)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, pText->atom()};

	ErrorEventObject *pObj = (ErrorEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pText);
	pObj->nEventID=id;
	return pObj;
}

Stringp ErrorEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_error:
		return this->getSlotERROR_FORSPACE();
	default:
		{
			TextEventClass*pClass=((ShellToplevel*)toplevel())->getTextEventClass();
			return pClass->IDToStringType(id);
		}
		//return TextEventClass::IDToStringType(id);
	}
}

ErrorEventObject::ErrorEventObject(VTable *vtable, ScriptObject* proto, int capacity): TextEventObject(vtable, proto, 0)
{
	//Add your construct code here...
//	m_pText = AvmCore::newConstantStringLatin1("");		//Inherit from text event
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void ErrorEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp pText)
{
	//Add your act code here...
	return TextEventObject::AS3_constructor(type, bBubbles, bCancelable, pText);
}

EventObject* ErrorEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_ErrorEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	return TextEventObject::Clone(pObj);
}

ErrorEventObject* ErrorEventObject::AS3_clone()
{
	//Add your act code here...
	return (ErrorEventObject *)Clone(NULL);
}

Stringp ErrorEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

//new adds
int ErrorEventObject::AS3_errorID_get()
{
	return 0;
}
//new adds end


}