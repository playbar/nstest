#ifndef _AS3Private_
#define _AS3Private_

namespace avmplus{namespace NativeID{
class DomainClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DomainObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell
{
	class ByteArrayObject;
	class DomainClass : public ClassClosure
	{
	public:
		DomainClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DomainClassSlots m_slots_DomainClass;
	};
	class DomainObject : public ScriptObject
	{
		public:
			DomainObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		public:
			//SBitmapCore				 m_sbitmap;
			void AS3_constructor();		
			void AS3_domainMemory_set(ByteArrayObject*);
			ByteArrayObject* AS3_domainMemory_get();
		private:
			DWB(Domain*) m_pDomain;
#ifdef _SYMBIAN
	public:
#endif
			friend class avmplus::NativeID::SlotOffsetsAndAsserts;
			avmplus::NativeID::DomainObjectSlots m_slots_DomainObject;
	};
}

#endif