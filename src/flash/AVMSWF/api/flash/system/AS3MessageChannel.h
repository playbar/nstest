#ifndef _AS3MessageChannel_
#define _AS3MessageChannel_

#include "AS3EventDispatcher.h"
#include "AS3MessageChannelState.h"

namespace avmplus
{
	namespace NativeID
	{
		class MessageChannelClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

		private:
		};
		class MessageChannelObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class MessageChannelClass : public ClassClosure//EventClass
	{
	public:
		MessageChannelClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MessageChannelClassSlots m_slots_MessageChannelClass;
	};
	class MessageChannelObject : public EventDispatcherObject
	{
	public:
		MessageChannelObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:

		bool m_bMessageAvailable;
		Stringp m_strState;

		bool AS3_messageAvailable_get();
		Stringp AS3_state_get();

		void AS3_addEventListener(Stringp type, FunctionObject* listener, bool useCapture, int priority, bool useWeakReference);
		void AS3_close();
		void* AS3_receive(bool blockUntilReceived = false);
		void AS3_removeEventListener(Stringp type, FunctionObject* listener, bool useCapture);
		void AS3_send(void* arg, int queueLimit);
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MessageChannelObjectSlots m_slots_MessageChannelObject;
	};
}
#endif