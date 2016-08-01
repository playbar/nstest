#ifndef _AS3WorkerState_
#define _AS3WorkerState_

namespace avmplus
{
	namespace NativeID
	{
		class WorkerStateClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			//NEW:String = "new"
			//RUNNING:String = "running"
			//TERMINATED:String = "terminated"

			Stringp NEW;
			Stringp RUNNING;
			Stringp TERMINATED;
		private:
		};
		class WorkerStateObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class WorkerStateClass : public ClassClosure//EventClass
	{
	public:
		WorkerStateClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:

		inline Stringp getSlotNEW(){return m_slots_WorkerStateClass.NEW;}
		inline Stringp getSlotRUNNING(){return m_slots_WorkerStateClass.RUNNING;}
		inline Stringp getSlotTERMINATED(){return m_slots_WorkerStateClass.TERMINATED;}

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WorkerStateClassSlots m_slots_WorkerStateClass;
	};
	class WorkerStateObject : public ScriptObject
	{
	public:
		WorkerStateObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WorkerStateObjectSlots m_slots_WorkerStateObject;
	};
}
#endif