#ifndef _AS3SecurityDomain_
#define _AS3SecurityDomain_
namespace avmplus{namespace NativeID{
class SecurityDomainClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SecurityDomainObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SecurityDomainClass : public ClassClosure
	{
	public:
		SecurityDomainClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		SecurityDomainObject* AS3_currentDomain_get();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecurityDomainClassSlots m_slots_SecurityDomainClass;
};
class SecurityDomainObject : public ScriptObject
{
	public:
		SecurityDomainObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecurityDomainObjectSlots m_slots_SecurityDomainObject;
};}
#endif