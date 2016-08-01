#ifndef _AS3Worker_
#define _AS3Worker_

#include "AS3EventDispatcher.h"
#include "AS3WorkerState.h"
#include "AS3WorkerDomain.h"
#include "AS3MessageChannel.h"

namespace avmplus
{
	namespace NativeID
	{
		class WorkerClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

		private:
		};
		class WorkerObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{

	class WorkerObject;
	class WorkerClass : public ClassClosure//EventClass
	{
	public:
		WorkerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		DRCWB(WorkerObject*) current;
		bool isSupported;
		WorkerObject* AS3_current_get();
		bool AS3_isSupported_get();

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WorkerClassSlots m_slots_WorkerClass;
	};
	class WorkerObject : public EventDispatcherObject
	{
	public:
		WorkerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		bool m_bPrimordial;
		Stringp m_strState;

		bool AS3_isPrimordial_get();
		
		Stringp AS3_state_get();
		
		void AS3_addEventListener(Stringp type, FunctionObject* listener, bool useCapture = false, int priority = 0, bool useWeakReference = false);
		MessageChannelObject* AS3_createMessageChannel(WorkerObject* receiver);
		void* AS3_getSharedProperty(Stringp key);
		void AS3_removeEventListener(Stringp type, FunctionObject* listener, bool useCapture = false);
		void AS3_setSharedProperty(Stringp key, void* value);
		void AS3_start();
		bool AS3_terminate();

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WorkerObjectSlots m_slots_WorkerObject;
	};
}
#endif