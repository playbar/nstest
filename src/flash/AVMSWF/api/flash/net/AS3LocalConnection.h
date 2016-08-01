#ifndef _AS3LocalConnection_
#define _AS3LocalConnection_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class LocalConnectionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class LocalConnectionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class LocalConnectionObject;

	class LocalConnectionClass : public ClassClosure
	{
	public:
		LocalConnectionObject* CreateLocalConnection();

	public:
		LocalConnectionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	//new adds
	//private:

	//	bool m_isSupported;
	//public:
	//	bool AS3_isSupported_get();
	//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LocalConnectionClassSlots m_slots_LocalConnectionClass;
};
class LocalConnectionObject : public EventDispatcherObject
{
	public:
		LocalConnectionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		DRCWB(ScriptObject*)m_pClient;
		DRCWB(Stringp)		m_domain;

		//new adds
		bool m_isPerUser;
		//new adds end

		bool SendMethod(const char*name,const char*strDomain,const char*method,ArrayObject*args);
	public:
		Atom			AS3_client_get();
		void			AS3_client_set(Atom atom);
		Stringp			AS3_domain_get();
		void			AS3_constructor();
		void			AS3_allowDomain(ArrayObject*);
		void			AS3_allowInsecureDomain(ArrayObject*);
		void			AS3_close();
		void			AS3_connect(Stringp pConnectionName);
		void			AS3_send(Stringp,Stringp,ArrayObject*);
		
		//new adds
		bool AS3_isPerUser_get();
		void AS3_isPerUser_set(bool isPerUser);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LocalConnectionObjectSlots m_slots_LocalConnectionObject;
};}
#endif