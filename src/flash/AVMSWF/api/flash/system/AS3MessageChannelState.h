#ifndef _AS3MessageChannelState_
#define _AS3MessageChannelState_

namespace avmplus
{
	namespace NativeID
	{
		class MessageChannelStateClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			//CLOSED:String = "closed"
			//CLOSING:String = "closing"
			//OPEN:String = "open"
			Stringp CLOSED;
			Stringp CLOSING;
			Stringp OPEN;

		private:
		};
		class MessageChannelStateObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class MessageChannelStateClass : public ClassClosure//EventClass
	{
	public:
		MessageChannelStateClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		inline Stringp getSlotCLOSED(){return m_slots_MessageChannelStateClass.CLOSED;}
		inline Stringp getSlotCLOSING(){return m_slots_MessageChannelStateClass.CLOSING;}
		inline Stringp getSlotOPEN(){return m_slots_MessageChannelStateClass.OPEN;}

	public:

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MessageChannelStateClassSlots m_slots_MessageChannelStateClass;
	};
	class MessageChannelStateObject : public ScriptObject
	{
	public:
		MessageChannelStateObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MessageChannelStateObjectSlots m_slots_MessageChannelStateObject;
	};
}
#endif