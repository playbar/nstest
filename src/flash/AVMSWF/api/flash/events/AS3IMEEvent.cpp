#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3IMEEvent.h"
namespace avmshell{
IMEEventClass::IMEEventClass(VTable* cvtable):ClassClosure(cvtable)//TextEventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())IMEEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* IMEEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) IMEEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

IMEEventObject* IMEEventClass::CreateEventObject(int id, Stringp pText)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, pText->atom()};

	IMEEventObject *pObj = (IMEEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pText);
	pObj->nEventID=id;
	return pObj;
}

Stringp IMEEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_imeComposition:
		return this->getSlotIME_COMPOSITION();
	default:
		{
			TextEventClass*pClass=((ShellToplevel*)toplevel())->getTextEventClass();
			return pClass->IDToStringType(id);
		}
		//return TextEventClass::IDToStringType(id);
	}
}

IMEEventObject::IMEEventObject(VTable *vtable, ScriptObject* proto, int capacity): TextEventObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void IMEEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp text)
{
	//Add your act code here...
	return TextEventObject::AS3_constructor(type, bBubbles, bCancelable, text);
}

EventObject* IMEEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_IMEEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	return TextEventObject::Clone(pObj);
}

IMEEventObject* IMEEventObject::AS3_clone()
{
	//Add your act code here...
	return (IMEEventObject *)Clone(NULL);
}

Stringp IMEEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

//new adds
AvmBox IMEEventObject::AS3_imeClient_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return kAvmThunkUndefined;
}
AvmBox IMEEventObject::AS3_imeClient_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	return kAvmThunkUndefined;
}
//new adds end

}