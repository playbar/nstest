#ifndef _AS3XMLSocket_
#define _AS3XMLSocket_

#include "AS3EventDispatcher.h"
#include "XClient.h"

namespace avmplus{namespace NativeID{
class XMLSocketClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class XMLSocketObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class XMLSocketObject;

	class XMLSocketClass : public ClassClosure
	{
	public:
		XMLSocketClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		XMLSocketObject *CreateXMLSocket(Stringp host, uint32_t port);

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLSocketClassSlots m_slots_XMLSocketClass;
};
class XMLSocketObject : public EventDispatcherObject
{
	public:
		XMLSocketObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		virtual bool OnRecieve(int nSize);
		virtual bool OnClose(const char*msg);
		virtual XU8* GetInBuffer(int l)
		{
			m_data.SetSize(l);
			return m_data.GetData();
			//return m_cache.GetInput(l);
		}
	public:
		
	public:
		bool	AS3_connected_get();
		void	AS3_constructor(Stringp host, uint32_t port);
		void	AS3_close();
		void	AS3_connect(Stringp host, uint32_t port);
		void	AS3_send(Atom pObject);

		//new adds
		int AS3_timeout_get();
		void AS3_timeout_set(int timeout);
		//new adds end

	private:
		XU8Array m_data;
		XU32	m_nLoaded;
		XClient*m_pClient;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLSocketObjectSlots m_slots_XMLSocketObject;
};}
#endif