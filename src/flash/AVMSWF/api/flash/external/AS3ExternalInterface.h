#ifndef _AS3ExternalInterface_
#define _AS3ExternalInterface_
namespace avmplus{namespace NativeID{
class ExternalInterfaceClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ExternalInterfaceObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	
	class ExternalInterfaceClass : public ClassClosure
	{
	public:
		ExternalInterfaceClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ExternalInterfaceClassSlots m_slots_ExternalInterfaceClass;
		Atom SystemExec(XString8&strKey,ArrayObject*args);
		Atom SystemVar(XString8&strName,ArrayObject*args);
		Atom SystemCall(XString8&strCall,ArrayObject*args);
	public:
		
		void AS3_addCallback(Stringp,FunctionObject*);
		Atom AS3_call(Stringp,ArrayObject*);
		Atom StringAtom(const char*cnt,int l,ArrayObject*args);
		bool AS3_available_get();
		void AS3_available_set(bool b);
		Stringp AS3_objectID_get();
		void AS3_objectID_set(Stringp);

		//new adds
		bool AS3_marshallExceptions_get();
		void AS3_marshallExceptions_set(bool marshallExceptions);
		//new adds end
};
class ExternalInterfaceObject : public ScriptObject
{
	public:
		ExternalInterfaceObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ExternalInterfaceObjectSlots m_slots_ExternalInterfaceObject;
};}
#endif