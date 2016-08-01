#ifndef _AS3RemoteNotificationEvent_
#define _AS3RemoteNotificationEvent_

#include "AS3Event.h"
#include "AS3DisplayObject.h"
#include "AS3Stage.h"
#include "AS3ThrottleType.h"

namespace avmplus
{
	namespace NativeID
	{
		class RemoteNotificationEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//NOTIFICATION:String = "notification"
			//TOKEN:String = "token"

			Stringp NOTIFICATION;
			Stringp TOKEN;
		private:
		};
		class RemoteNotificationEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class RemoteNotificationEventClass : public ClassClosure//EventClass
	{
	public:
		RemoteNotificationEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		inline Stringp getSlotNOTIFICATION(){return m_slots_RemoteNotificationEventClass.NOTIFICATION;}
		inline Stringp getSlotTOKEN(){return m_slots_RemoteNotificationEventClass.TOKEN;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::RemoteNotificationEventClassSlots m_slots_RemoteNotificationEventClass;
	};
	class RemoteNotificationEventObject : public EventObject
	{
	public:
		RemoteNotificationEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(ScriptObject*) m_pData;
		Stringp m_strTokenID;
		ScriptObject* AS3_data_get();
		Stringp AS3_tokenId_get();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::RemoteNotificationEventObjectSlots m_slots_RemoteNotificationEventObject;
	};
}
#endif