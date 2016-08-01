#include "StdAfxflash.h"
//#include "avmplus.h"
#include "ShellCore.h"

using namespace avmplus;

#include "../avm2.h"
//#include "AS3/TotalInclude.h"   // hgl 修改文件应由  引用位置                  

#include "AS3ActivityEvent.h"

namespace avmshell{
ActivityEventClass::ActivityEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())ActivityEventObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
}
ScriptObject* ActivityEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) ActivityEventObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

ActivityEventObject* ActivityEventClass::CreateEventObject(int id, bool activating)
{
	Stringp strType = IDToStringType(id);
	if (!strType)
	{
		return NULL;
	}

	ActivityEventObject *pObj = (ActivityEventObject *)createInstance(ivtable(), prototype);
	pObj->AS3_constructor(strType, false, false, activating);
	pObj->nEventID=id;
	return pObj;
}

Stringp ActivityEventClass::IDToStringType(int id)
{
	switch (id)
	{
	case XAS3_EVENT_activity:
		return this->getSlotACTIVITY();
	default:
		{
			EventClass*pClass=((ShellToplevel*)toplevel())->getEventClass();
			return pClass->IDToStringType(id);
		}
		//return EventClass::IDToStringType(id);
	}
}

ActivityEventObject::ActivityEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
{
	m_bActivity=false;
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...
bool ActivityEventObject::AS3_activating_get()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return m_bActivity;
}

void ActivityEventObject::AS3_activating_set(bool ba)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	/*
	enum
	{
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject    
	};
	m_bActivity=AvmThunkUnbox_AvmBool32(argv[argoff1]);

	return kAvmThunkUndefined;
	*/
	//Add your act code here...	
	m_bActivity = ba;
	//return kAvmThunkUndefined;
}

//AS3 contructor function..
void ActivityEventObject::AS3_constructor(Stringp strType,bool bb,bool bc,bool ba)//AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	//Add your act code here...
/*
	EventObject::AS3_constructor(env,argc,argv);

	if (4 == argc)
	{
		enum
		{
			argoff0 = 0
			, argoff1 = argoff0 + AvmThunkArgSize_AvmObject
			, argoff2 = argoff1 + AvmThunkArgSize_AvmString
			, argoff3 = argoff2 + AvmThunkArgSize_AvmBool32
			, argoff4 = argoff3 + AvmThunkArgSize_AvmBool32
		};

		m_bActivity=AvmThunkUnbox_AvmBool32(argv[argoff4]);
	}

	return kAvmThunkUndefined;
*/
	EventObject::AS3_constructor(strType,bb,bc);//env,argc,argv);	
	m_bActivity=ba;//AvmThunkUnbox_AvmBool32(argv[argoff4]);//Modify this please!
}

EventObject* ActivityEventObject::Clone(EventObject*pObj)
{
	if(pObj==NULL)
	{
		Toplevel*t=toplevel();
		ClassClosure *pClass=t->getBuiltinExtensionClass(abcclass_flash_events_ActivityEvent);
		//Atom argv=pClass->atom();
		//Atom atoms[]={pClass->atom(),strType->atom(),bBubbles,bCancelable};
		//Atom atom=pClass->construct(3,atoms);
		//pObj=(EventObject*)AvmCore::atomToScriptObject(atom);	
		pObj = (EventObject *)pClass->createInstance(pClass->ivtable(),pClass->prototype);
	}

	register ActivityEventObject* pEvent = (ActivityEventObject *)pObj;
	pEvent->m_bActivity = m_bActivity;

	return EventObject::Clone(pObj);
}

ActivityEventObject* ActivityEventObject::AS3_clone()//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return (ActivityEventObject *)Clone(NULL)->atom();
}

Stringp ActivityEventObject::AS3_toString()//AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	//core()->intern(
	//Add your act code here...
	return EventObject::AS3_toString();
	//return kAvmThunkUndefined;//Modify this please!
}

}