#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"

//#include "../AS3/TotalInclude.h"

#include "AS3FullScreenEvent.h"
namespace avmshell{
FullScreenEventClass::FullScreenEventClass(VTable* cvtable):ClassClosure(cvtable)//ActivityEventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())FullScreenEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* FullScreenEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) FullScreenEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

FullScreenEventObject* FullScreenEventClass::CreateEventObject(int id, bool fullScreen, bool interactive)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, fullScreen};

	FullScreenEventObject *pObj = (FullScreenEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, fullScreen, interactive);
	pObj->nEventID=id;
	return pObj;
}

Stringp FullScreenEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_fullscreen:
		return this->getSlotFULL_SCREEN();
	case XAS3_EVENT_fullScreenInteractiveAccepted:
		return getSlotFULL_SCREEN_INTERACTIVE_ACCEPTED();
	default:
		{
			ActivityEventClass*pClass=((ShellToplevel*)toplevel())->getActivityEventClass();
			return pClass->IDToStringType(id);
		}
		//return ActivityEventClass::IDToStringType(id);
	}
}

FullScreenEventObject::FullScreenEventObject(VTable *vtable, ScriptObject* proto, int capacity): ActivityEventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_isFullScreen = false;
	m_isInteractive = false;
}
//////////////////////////////////////////////////////////
//Native Method start...
bool FullScreenEventObject::AS3_fullScreen_get()
{
	//Add your act code here...
	return m_isFullScreen;//Modify this please!
}

//AS3 contructor function..
void FullScreenEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, bool isFullScreen, bool isInteractive)
{
	//Add your act code here...
//	ActivityEventObject::AS3_constructor(env,argc,argv);
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
	//m_isFullScreen=AvmThunkUnbox_AvmBool32(argv[argoff4]);

	EventObject::AS3_constructor(type, bBubbles, bCancelable);

	m_isFullScreen = isFullScreen;
	m_isInteractive = isInteractive;
}

EventObject* FullScreenEventObject::Clone(EventObject*pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_FullScreenEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register FullScreenEventObject *p = (FullScreenEventObject *)pObj;
	p->m_isFullScreen = m_isFullScreen;
	p->m_isInteractive = m_isInteractive;
	
	return ActivityEventObject::Clone(pObj);
}

FullScreenEventObject* FullScreenEventObject::AS3_clone()
{
	//Add your act code here...
	return (FullScreenEventObject *)Clone(NULL);
}

Stringp FullScreenEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}
//new adds 11
bool FullScreenEventObject::AS3_interactive_get()
{
	//LOGWHERE();
	
	return m_isInteractive;
}
//new adds 11 end
}