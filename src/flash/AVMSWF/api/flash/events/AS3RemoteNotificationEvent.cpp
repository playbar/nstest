#include "StdAfxflash.h"
#include "avmplus.h"
using namespace avmplus;
#include "AS3RemoteNotificationEvent.h"
namespace avmshell
{
	RemoteNotificationEventClass::RemoteNotificationEventClass(VTable* cvtable):ClassClosure(cvtable)//EventClass(cvtable)
	{
		createVanillaPrototype();
		//Add your construct code here...
	};
	ScriptObject* RemoteNotificationEventClass::createInstance(VTable *ivtable, ScriptObject* prototype)
	{
		return new (core()->GetGC(), ivtable->getExtraSize()) RemoteNotificationEventObject(ivtable, prototype, 0);
		//AS3 'new' opt...Add your init code here...
	}
	RemoteNotificationEventObject::RemoteNotificationEventObject(VTable *vtable, ScriptObject* proto, int capacity): EventObject(vtable, proto, 0)
	{
		//Add your construct code here...
		m_pData = NULL;
		m_strTokenID = NULL;
	}
	//////////////////////////////////////////////////////////

	ScriptObject* RemoteNotificationEventObject::AS3_data_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_pData;//NULL;
	}
	Stringp RemoteNotificationEventObject::AS3_tokenId_get()
	{
		//Add your construct code here...
		//LOGWHERE();

		return m_strTokenID;//NULL;
	}
}