#ifndef _AS3SharedObject_
#define _AS3SharedObject_

#include "AS3EventDispatcher.h"
#include "AS3DataIO.h"

namespace avmplus{namespace NativeID{
class SharedObjectClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SharedObjectObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SharedObjectObject;
	class NetConnectionObject;
	class ByteArrayObject;
	class SharedObjectClass : public ClassClosure
	{
	public:
		DWB(EventHashTable*) m_localObjs;
		ObjectEncoding m_nDefaultEncoding;
		SharedObjectClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		SharedObjectObject* AS3_getLocal(Stringp,Stringp,bool);
		SharedObjectObject* AS3_getRemote(Stringp,Stringp,Atom,bool);
		uint32_t AS3_defaultObjectEncoding_get(){return m_nDefaultEncoding;}
		void AS3_defaultObjectEncoding_set(uint32_t v){m_nDefaultEncoding=(ObjectEncoding)v;}
		
		//new adds 11
//#ifdef __APPLE__
		bool m_bPreventBackup;
		bool AS3_preventBackup_get();
		void AS3_preventBackup_set(bool b);
//#endif
		//new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SharedObjectClassSlots m_slots_SharedObjectClass;
};
class SharedObjectObject : public EventDispatcherObject
{
	public:
		SharedObjectObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		virtual void ReleaseData();
	public:		
		DRCWB(ScriptObject*)		m_pClient;
		DRCWB(ScriptObject*)		m_pData;
		XU32						m_nDelay;
		DRCWB(ByteArrayObject*)		m_pBuffer;
		DRCWB(NetConnectionObject*) m_connect;
		//DRCWB(Stringp)				m_strFile;
		DRCWB(Stringp)				m_strName;
		DRCWB(Stringp)				m_strPath;
		//XFile						m_file;
		XString8					m_strFile;
		bool LoadLocal();
		bool LoadNet(Stringp);
		bool SaveLocal();
		bool SaveNet();
		bool SaveToBuffer();
		bool CreateLocalFile(Stringp name,Stringp path);
		//bool CreateConnect(Stringp name,Stringp path);
		
		
		Atom AS3_client_get()
		{	if(m_pClient) m_pClient->atom();
			 return nullObjectAtom;
		}
		void AS3_client_set(Atom atom){m_pClient=core()->atomToScriptObject(atom);}
		Atom AS3_data_get();
		void AS3_fps_set(double);
		uint32_t AS3_objectEncoding_get();
		void AS3_objectEncoding_set(uint32_t);
		uint32_t AS3_size_get();
		void AS3_clear();
		void AS3_close();
		void AS3_connect(NetConnectionObject*,Stringp);
		Stringp AS3_flush(int);
		void AS3_send(ArrayObject*);
		void AS3_setDirty(Stringp);
		void AS3_setProperty(Stringp,Atom);
	private:

		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SharedObjectObjectSlots m_slots_SharedObjectObject;
};
}
#endif
