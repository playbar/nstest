#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3Worker.h"
namespace avmshell
{
	WorkerClass::WorkerClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
		current = NULL;//(WorkerObject*)createInstance(ivtable(),prototype);
		isSupported = true;
	};
	ScriptObject* WorkerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) WorkerObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	WorkerObject::WorkerObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_bPrimordial = true;
		m_strState = ((ShellToplevel*)toplevel())->getWorkerStateClass()->getSlotRUNNING();
	}
	//////////////////////////////////////////////////////////

	WorkerObject* WorkerClass::AS3_current_get()
	{
		//LOGWHERE();
		if(!current)
			current=(WorkerObject*)createInstance(ivtable(),prototype);
		return current;
	}
	bool WorkerClass::AS3_isSupported_get()
	{
		//LOGWHERE();
		return isSupported;//true;
	}

	bool WorkerObject::AS3_isPrimordial_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_bPrimordial;//true;
	}
	Stringp WorkerObject::AS3_state_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strState;//Stringp("running");//NULL;
	}
	
	void WorkerObject::AS3_addEventListener(Stringp type, FunctionObject* listener, bool useCapture, int priority, bool useWeakReference)
	{
		//Add your construct code here..
		//LOGWHERE();
		EventDispatcherObject::AS3_addEventListener(type, listener, useCapture, priority, useWeakReference);
	}
	MessageChannelObject* WorkerObject::AS3_createMessageChannel(WorkerObject* receiver)
	{
		//Add your construct code here..
		//LOGWHERE();
		MessageChannelClass* pClass = ((ShellToplevel*)toplevel())->getMessageChannelClass();
		MessageChannelObject* pMC = (MessageChannelObject*)pClass->createInstance(pClass->ivtable(), pClass->prototype);

		return pMC;
	}
	void* WorkerObject::AS3_getSharedProperty(Stringp key)
	{
		//Add your construct code here..
		LOGWHERE();

		return NULL;
	}
	void WorkerObject::AS3_removeEventListener(Stringp type, FunctionObject* listener, bool useCapture )
	{
		//Add your construct code here..
		//LOGWHERE();
		EventDispatcherObject::AS3_removeEventListener(type, listener, useCapture);
	}
	void WorkerObject::AS3_setSharedProperty(Stringp key, void* value)
	{
		//Add your construct code here..
		LOGWHERE();

	}
	void WorkerObject::AS3_start()
	{
	//Add your construct code here..
		LOGWHERE();
	//	m_strState = ((ShellToplevel*)toplevel())->getWorkerStateClass()->getSlotRUNNING();

	}
	bool WorkerObject::AS3_terminate()
	{
		//Add your construct code here..
		LOGWHERE();
		WorkerStateClass* pClass = ((ShellToplevel*)toplevel())->getWorkerStateClass();
		if(m_strState->equals(pClass->getSlotTERMINATED()))return false;
		m_strState = pClass->getSlotTERMINATED();
		return true;

	}

}