#ifndef _AS3ApplicationDomain_
#define _AS3ApplicationDomain_
namespace avmplus{namespace NativeID{
class ApplicationDomainClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ApplicationDomainObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ApplicationDomainObject;
	class LoaderObject;
	class ByteArrayObject;
	class ApplicationDomainClass : public ClassClosure
	{
	public:
		ApplicationDomainClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ApplicationDomainObject* CreateAppDomain(ApplicationDomainObject*pParent,
											     ShellCodeContext*code,Toplevel*pTop);
		ApplicationDomainObject* AS3_currentDomain_get();		
		XUINT AS3_MIN_DOMAIN_MEMORY_LENGTH_get();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ApplicationDomainClassSlots m_slots_ApplicationDomainClass;
};
class ApplicationDomainObject : public ScriptObject
{
	public:
		
		ApplicationDomainObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		DRCWB(ApplicationDomainObject*) m_pParent;
		DRCWB(ByteArrayObject*)			m_domainMemory;
		//DRCWB(LoaderObject*)			m_pLoader;
		DWB(DomainEnv*)					m_pDomainEnv;
		//DWB(ShellCodeContext*)			m_pCodeContext;
		//DWB(Toplevel*) m_pDomainToplevel;
		
		ApplicationDomainObject* AS3_parentDomain_get(){return m_pParent;}
		void Init(ApplicationDomainObject*);
		void AS3_constructor(ApplicationDomainObject*pParent);
		Atom AS3_getDefinition(Stringp);
		Atom FindDefinition(Stringp);
		bool AS3_hasDefinition(Stringp);
		
		ByteArrayObject* AS3_domainMemory_get();
        void AS3_domainMemory_set(ByteArrayObject*);

		//new adds 11
		Stringp AS3_getQualifiedDefinitionNames(){ 
			LOGWHERE();
			return NULL; // 完全限定名称。
		}
        //new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		ScriptObject* finddef(const Multiname& multiname,bool bThrow);
		ScriptObject* findvec(const Stringp name,bool bThrow);
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ApplicationDomainObjectSlots m_slots_ApplicationDomainObject;
};}
#endif
