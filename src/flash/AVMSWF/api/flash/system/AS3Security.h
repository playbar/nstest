#ifndef _AS3Security_
#define _AS3Security_
namespace avmplus{namespace NativeID{
class SecurityClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	//Stringp LOCAL_TRUSTED = "localTrusted";
	//Stringp LOCAL_WITH_FILE = "localWithFile"; 
	//Stringp LOCAL_WITH_NETWORK = "localWithNetwork"; 
	//Stringp REMOTE = "remote";

	//APPLICATION : String = "application"

	Stringp LOCAL_TRUSTED;
	Stringp LOCAL_WITH_FILE; 
	Stringp LOCAL_WITH_NETWORK; 
	Stringp REMOTE;

	//new adds
	Stringp APPLICATION;
	//new adds end
private:};
class SecurityObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

private:};
}}
namespace avmshell{
	class SecurityClass : public ClassClosure
	{
	public:
		static int m_nSandType;
		enum 
		{	
			  XAS3_LOCAL_TRUSTED=0,
			  XAS3_LOCAL_WITH_FILE=1,
			  XAS3_LOCAL_WITH_NETWORK=2,
			  XAS3_REMOTE=3,
			  XAS3_APPLICATION=4
		};
	public:
		bool m_bExactSetting;
		SecurityClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		bool AS3_exactSettings_get()
		{	return m_bExactSetting;
		}
		void AS3_exactSettings_set(bool b)
		{
			m_bExactSetting=b;
		}
		Stringp AS3_sandboxType_get()
		{
			Stringp s=((Stringp*)&m_slots_SecurityClass)[m_nSandType];
			return s;
		}
		void AS3_allowDomain(ArrayObject*);
		void AS3_allowInsecureDomain(ArrayObject*);
		void AS3_loadPolicyFile(Stringp);
		void AS3_showSettings(Stringp);

		//new adds
		Stringp AS3_pageDomain_get();
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecurityClassSlots m_slots_SecurityClass;
};
class SecurityObject : public ScriptObject
{
	public:
		SecurityObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:		
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SecurityObjectSlots m_slots_SecurityObject;
};}
#endif