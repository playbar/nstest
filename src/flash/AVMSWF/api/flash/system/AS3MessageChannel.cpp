#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3MessageChannel.h"
namespace avmshell
{
	MessageChannelClass::MessageChannelClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* MessageChannelClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) MessageChannelObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	MessageChannelObject::MessageChannelObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_bMessageAvailable = false;
		m_strState = ((ShellToplevel*)toplevel())->getMessageChannelStateClass()->getSlotOPEN();
	}
	//////////////////////////////////////////////////////////

	bool MessageChannelObject::AS3_messageAvailable_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_bMessageAvailable;//false;
	}
	Stringp MessageChannelObject::AS3_state_get()
	{
		//Add your construct code here...

		return m_strState;//Stringp("open");//NULL;
	}

	void MessageChannelObject::AS3_addEventListener(Stringp type, FunctionObject* listener, bool useCapture, int priority, bool useWeakReference)
	{
		//Add your construct code here...
		//LOGWHERE();
		return EventDispatcherObject::AS3_addEventListener(type, listener, useCapture, priority, useWeakReference);
	}
	void MessageChannelObject::AS3_close()
	{
		//Add your construct code here...
		//LOGWHERE();
		m_strState = ((ShellToplevel*)toplevel())->getMessageChannelStateClass()->getSlotCLOSED();
	}
	void* MessageChannelObject::AS3_receive(bool blockUntilReceived)
	{
		//Add your construct code here...
		LOGWHERE();

		return NULL;
	}
	void MessageChannelObject::AS3_removeEventListener(Stringp type, FunctionObject* listener, bool useCapture)
	{
		//Add your construct code here...
		//LOGWHERE();
		EventDispatcherObject::AS3_removeEventListener(type, listener, useCapture);

	}
	void MessageChannelObject::AS3_send(void* arg, int queueLimit)
	{
		//Add your construct code here...
		LOGWHERE();

	}
	Stringp MessageChannelObject::AS3_toString()
	{
		//Add your construct code here...
		LOGWHERE();

		return NULL;//AvmCore::atomToString(toString());
	}

}