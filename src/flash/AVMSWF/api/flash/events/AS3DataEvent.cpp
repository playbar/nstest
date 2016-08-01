#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;

#include "../avm2.h"
//#include "../AS3/TotalInclude.h"

#include "AS3DataEvent.h"
namespace avmshell{
DataEventClass::DataEventClass(VTable* cvtable):ClassClosure(cvtable)//TextEventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())DataEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* DataEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) DataEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

DataEventObject* DataEventClass::CreateEventObject(int id, Stringp pData)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

//	Atom atoms[] = {strType->atom(), false, false, pData->atom()};

	DataEventObject *pObj = (DataEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, pData);
	pObj->nEventID=id;
	return pObj;
}

Stringp DataEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_data:
		return this->getSlotDATA();
	case XAS3_EVENT_uploadCompleteData:
		return this->getSlotUPLOAD_COMPLETE_DATA();
	default:
		{
			TextEventClass*pClass=((ShellToplevel*)toplevel())->getTextEventClass();
			return pClass->IDToStringType(id);
		}
		//return TextEventClass::IDToStringType(id);
	}
}

DataEventObject::DataEventObject(VTable *vtable, ScriptObject* proto, int capacity): TextEventObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_pData = core()->newConstantStringLatin1("");
}
//////////////////////////////////////////////////////////
//Native Method start...
Stringp DataEventObject::AS3_data_get()
{
	//Add your act code here...
	return m_pData;
}

void DataEventObject::AS3_data_set(Stringp pData)
{
	//Add your act code here...
	//enum
	//{
	//	argoff0 = 0
	//	, argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	//};

	m_pData = pData;
}

//AS3 contructor function..
void DataEventObject::AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp pData)
{
	//Add your act code here...
//	TextEventObject::AS3_constructor(env, argc, argv);

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

	m_pData = pData;
}

EventObject* DataEventObject::Clone(EventObject *pObj)
{
	if (NULL == pObj)
	{
		ClassClosure *pClass = toplevel()->getBuiltinExtensionClass(abcclass_flash_events_DataEvent);
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(), pClass->prototype);
	}

	register DataEventObject *pEvent = (DataEventObject *)pObj;

	pEvent->m_pData = m_pData;

	return TextEventObject::Clone(pObj);
}

DataEventObject* DataEventObject::AS3_clone()
{
	//Add your act code here...
	return (DataEventObject*)Clone(NULL);
}

Stringp DataEventObject::AS3_toString()
{
	//Add your act code here...
	return EventObject::AS3_toString();
}

}