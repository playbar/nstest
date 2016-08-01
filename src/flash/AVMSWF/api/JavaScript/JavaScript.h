#ifndef _JavaScript_
#define _JavaScript_

//Delete JavaScript for BUGS

/*namespace avmplus{namespace NativeID{
class WindowClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class WindowObjectSlots{
friend class SlotOffsetsAndAsserts;
public:

private:};

class LocationClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class LocationObjectSlots{
friend class SlotOffsetsAndAsserts;
public:

private:};

class NavigatorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NavigatorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:

private:};

}}
namespace avmshell{
	class WindowObject;
	class LocationObject;
	//////////JavaScript Location
	class LocationClass : public ClassClosure
	{
	public:
		LocationClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		LocationObject* CreateLocation(WindowObject*);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LocationClassSlots m_slots_LocationClass;
	};
	class LocationObject : public ScriptObject
	{
		public:
			LocationObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		public:	
			DRCWB(WindowObject*) m_pWindow;
			Stringp AS3_hash_get();
			void    AS3_hash_set(Stringp);
			Stringp AS3_host_get();
			void	AS3_host_set(Stringp);
			Stringp AS3_hostname_get();
			void	AS3_hostname_set(Stringp);
			Stringp AS3_href_get();
			void	AS3_href_set(Stringp);
			Stringp AS3_pathname_get();
			void	AS3_pathname_set(Stringp);
			Stringp AS3_port_get();
			void	AS3_port_set(Stringp);
			Stringp AS3_protocol_get();
			void	AS3_protocol_set(Stringp);
			Stringp AS3_search_get();
			void    AS3_search_set(Stringp);
			void	AS3_assign(Stringp);
			void	AS3_reload();
			void	AS3_replace(Stringp);
			Stringp AS3_toString();
		
		private:
#ifdef _SYMBIAN
	public:
#endif
			friend class avmplus::NativeID::SlotOffsetsAndAsserts;
			avmplus::NativeID::LocationObjectSlots m_slots_LocationObject;
	};
	///////////JavaScript Navigator
	class NavigatorClass : public ClassClosure
	{
	public:
		NavigatorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NavigatorClassSlots m_slots_NavigatorClass;
	};
	class NavigatorObject : public ScriptObject
	{
		public:
			NavigatorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		public:		
			DRCWB(Stringp) m_strUserAgent;
			Stringp AS3_appCodeName_get();
			Stringp AS3_appMinorVersion_get();
			Stringp AS3_appName_get();
			Stringp AS3_appVersion_get();
			Stringp AS3_browserLanguage_get();
			bool    AS3_cookieEnabled_get();
			Stringp AS3_cpuClass_get();
			bool	AS3_onLine_get();
			Stringp AS3_platform_get();
			Stringp AS3_systemLanguage_get();
			Stringp AS3_userAgent_get();
			Stringp AS3_userLanguage_get();
			bool AS3_javaEnabled();
			bool AS3_taintEnabled();
		private:
#ifdef _SYMBIAN
	public:
#endif
			friend class avmplus::NativeID::SlotOffsetsAndAsserts;
			avmplus::NativeID::NavigatorObjectSlots m_slots_NavigatorObject;
	};
	//////////JavaScript Window
	class WindowClass : public ClassClosure
	{
	public:
		WindowClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		WindowObject* CreateDomWindow(void*pDom);
		DRCWB(NavigatorObject*) m_pNavigator;
		DRCWB(WindowObject*)	m_pWindow;
		static WindowObject* AS3_window_get(MethodEnv*env);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WindowClassSlots m_slots_WindowClass;
};
class WindowObject : public ScriptObject
{
	public:
		virtual void Release();
		WindowObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:		
		DRCWB(LocationObject*)  m_pLocation;
		
		//DRCWB(NavigatorObject*) m_
		LocationObject* AS3_location_get();
		NavigatorObject* AS3_navigator_get();
		void	Connect(void*);
		void	AS3_location_set(Stringp str);
		void	AS3_constructor();
		void	SetDom(void*pDom){m_pDom=pDom;}
		void*	GetDom(){return m_pDom;}
		//NavigatorObject* AS3_Navigator_get();
	private:
		void * m_pDom;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::WindowObjectSlots m_slots_WindowObject;
};
}*/
#endif