#ifndef _AS3WorkerDomain_
#define _AS3WorkerDomain_

#include "AS3Worker.h"

namespace avmplus
{
	namespace NativeID
	{
		class WorkerDomainClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!


		private:
		};
		class WorkerDomainObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!


		private:
		};
	}
}
namespace avmshell{
	class WorkerDomainObject;
	class ByteArrayObject;
	class WorkerObject;
	class WorkerDomainClass : public ClassClosure//EventClass
	{
	public:
		WorkerDomainClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		//DRCWB(WorkerDomainObject*) m_pCurrent;
		bool m_bSupported;
	
	public:
		WorkerDomainObject* AS3_current_get();
		bool AS3_isSupported_get();


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WorkerDomainClassSlots m_slots_WorkerDomainClass;
	};
	class WorkerDomainObject : public ScriptObject
	{
	public:
		WorkerDomainObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		

		WorkerObject* AS3_createWorker(ByteArrayObject* swf, bool giveAppPrivileges = false);
		ObjectVectorObject* AS3_listWorkers();


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WorkerDomainObjectSlots m_slots_WorkerDomainObject;
	};
}
#endif