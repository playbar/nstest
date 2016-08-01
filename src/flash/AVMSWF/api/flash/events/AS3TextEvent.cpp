#include "StdAfxflash.h"
#include "ShellCore.h"
#include "splayer.h"
using namespace avmplus;

#include "avm2.h"

#include "AS3TextEvent.h"
namespace avmshell{
TextEventClass::TextEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())TextEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* TextEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) TextEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

TextEventObject* TextEventClass::CreateEventObject(int id, Stringp pText)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}


	TextEventObject *pObj = (TextEventObject *)createInstance(ivtable(), prototype);
	//	Atom atoms[] = {trType->atom(), false, false, pText->atom()};
	//  pObj->(NULL,3,atoms);
	pObj->AS3_constructor(strType, false, false, pText);
	pObj->nEventID=id;
	return pObj;
}

Stringp TextEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_link:
		return this->getSlotLINK();
	case XAS3_EVENT_textInput:
		return this->getSlotTEXT_INPUT();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

TextEventObject::TextEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pText = core()->kEmptyString;//NULL;//core()->newConstantStringLatin1("");
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp TextEventObject::AS3_text_get()
{
	//Add your act code here...
	return m_pText;
}

void TextEventObject::AS3_text_set(Stringp pText)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	m_pText = pText;
}

//AS3 contructor function..
void TextEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp pText)
{
	//Add your act code here...
//	EventObject::AS3_constructor(env, argc, argv);

//	register uint32_t argNum = argc > 4 ? 4 : argc;

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

	m_pText = pText;
}

EventObject* TextEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_TextEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register TextEventObject *pEvent = (TextEventObject *)pObj;

	pEvent->m_pText = m_pText;

	return EventObject::Clone(pObj);
}

TextEventObject* TextEventObject::AS3_clone()
{
	//Add your act code here...
	return (TextEventObject *)Clone(NULL);
}

Stringp TextEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}