#ifndef _AS3Proxy_
#define _AS3Proxy_
namespace avmplus{namespace NativeID{
class ProxyClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ProxyObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ProxyClass : public ClassClosure
	{
	public:
		ProxyClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ProxyClassSlots m_slots_ProxyClass;
};
class ProxyObject : public ScriptObject
{
	public:
		ProxyObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	protected:
		// callFunction(Stringp func,const Multiname*name,int argc,Atom*argv);
		inline MethodEnv* getMethod(Stringp name)const;
		Atom		  callFunction(MethodEnv* func,const Stringp name,int argc,Atom*argv)const;
		DRCWB(Namespacep) m_pNamespace;
		//DWB(HeapHashtable*) m_hashTable;
	public:
//#if (__CORE_VERSION__>=0x02070100)
		Atom createLocalObject(Atom name)const;
//#endif
		virtual Atom callProperty(const Multiname* name, int argc, Atom* argv);
		virtual bool deleteAtomProperty(Atom name);
		virtual Atom getDescendants(const Multiname* name) const;
		virtual Atom getAtomProperty(Atom name) const;
		virtual void setAtomProperty(Atom name,Atom value);
		virtual bool hasAtomProperty(Atom name) const;
		virtual Atom nextValue(int index);
		virtual int	 nextNameIndex(int index);
		virtual Atom nextName(int index);
		virtual Atom getUintProperty(uint32_t index) const;
		virtual void setUintProperty(uint32_t index,Atom value);
		virtual bool hasUintProperty(uint32_t index) const;
		virtual Atom getMultinameProperty(const Multiname* name) const;
		virtual void setMultinameProperty(const Multiname* name, Atom value);
		virtual bool deleteMultinameProperty(const Multiname* name);
		virtual bool hasMultinameProperty(const Multiname* name) const;
		/*AvmBox AS3_callProperty(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_deleteProperty(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getDescendants(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getProperty(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_hasProperty(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isAttribute(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_nextName(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_nextNameIndex(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_nextValue(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_setProperty(AvmMethodEnv env, uint32_t argc, AvmBox* argv);*/
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ProxyObjectSlots m_slots_ProxyObject;
};}
#endif