#ifndef _AS3NetConnection_
#define _AS3NetConnection_

#include "AS3EventDispatcher.h"
#include "AS3DataIO.h"
namespace avmplus{namespace NativeID{
class NetConnectionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetConnectionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class ResponderObject;
	class ByteArrayObject;
	class NetConnectionClass : public ClassClosure
	{
	public:
		ObjectEncoding m_nDefaultEncoding;
		NetConnectionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		uint32_t AS3_defaultObjectEncoding_get();
		void AS3_defaultObjectEncoding_set(uint32_t);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetConnectionClassSlots m_slots_NetConnectionClass;
};
class NetConnectionObject : public EventDispatcherObject
{
//#if (__CORE_VERSION__>=0x02080000)
	protected:
		bool m_bDating;
		DRC(Stringp) m_callCmds;
		DRCWB(ResponderObject*) m_callRes;
		DRCWB(ArrayObject*) m_callArgs;
		virtual void LaterCallback()
		{
			if(m_callCmds!=NULL||m_callRes!=NULL)
				AS3_call(m_callCmds,m_callRes,m_callArgs);
			m_callCmds=NULL;
			m_callRes=NULL;
			m_callArgs=NULL;
		}
//#endif
	public:
		NetConnectionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		virtual bool  StreamInNew(void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo);
		virtual int   StreamInWrite(void* buffer,int	  length,void*gcEnter,void*pClient);
		virtual bool  StreamInDestroy(XBOOL bOK,const char*msg,void*pClient);
	public:
		enum
		{
			NC_CALL_BADVERSION=0,//"NetConnection.Call.BadVersion" "error" �Բ���ʶ��ĸ�ʽ�������ݰ� 
			NC_CALL_FAILED=1,//"NetConnection.Call.Failed" "error" NetConnection.call �����޷����÷������˵ķ�������� 
			NC_CALL_PROHIBITED=2,//"NetConnection.Call.Prohibited" "error" Action Message Format (AMF) ������ȫԭ�����ֹ�� ������ AMF URL �� SWF ����ͬһ���򣬻����� AMF ������û������ SWF �ļ�����Ĳ����ļ���  
		    NC_CONNECT_CLOSED=3,//"NetConnection.Connect.Closed" "status" �ɹ��ر����ӡ� 
		    NC_CONNECT_FAILED=4,//"NetConnection.Connect.Failed" "error" ���ӳ���ʧ�ܡ� 
			NC_CONNECT_SUCCESS=5,//"NetConnection.Connect.Success" "status" ���ӳ��Գɹ��� 
			NC_CONNECT_REJECTED=6,//"NetConnection.Connect.Rejected" "error" ���ӳ���û�з���Ӧ�ó����Ȩ�ޡ� 
			NC_CONNECT_APPSHUTDOWN=7,//"NetConnection.Connect.AppShutdown" "error" ���ڹر�ָ����Ӧ�ó��� 
			NC_CONNECT_INVALIDAPP=8,//"NetConnection.Connect.InvalidApp" "error" ����ʱָ����Ӧ�ó�������Ч�� 
		};
		ScriptObject* CreateStatusObject(int nCode);
		bool	    m_bConnected;
		bool		m_bCalling;
		void*		m_pClient;
		void*		m_pLoaderInfo;
		//DRCWB(Atom) m_pClient;
		DRCWB(ByteArrayObject*) m_outData;
		DRCWB(ByteArrayObject*) m_inData;
		DRC(Stringp)			m_strURL;
		DRC(Stringp)			m_strProto;
		DWB(RCHashTable*)		m_resHash;
		DRCWB(ArrayObject*)	    m_headers;

		//new adds
		DRC(Stringp)	m_farID;
		DRC(Stringp)	m_farNonce;
		double			m_httpIdleTimeout;
		uint32_t		m_maxPeerConnections;
		DRC(Stringp)	m_nearID;
		DRC(Stringp)	m_nearNonce;
		DRC(Stringp)	m_protocol;
		DRCWB(ArrayObject*)	m_unconnectedPeerStreams;
		//new adds end

		//DWB(HeapHashtable*)		m_hashTable;
		//XString8Array			m_strHeaders;
		Atom AS3_client_get();
		void AS3_client_set(Atom atom);
		bool AS3_connected_get();
		Stringp AS3_connectedProxyType_get();
		uint32_t AS3_objectEncoding_get();
		void AS3_objectEncoding_set(uint32_t);
		Stringp AS3_proxyType_get();
		void AS3_proxyType_set(Stringp);
		Stringp AS3_uri_get();
		bool AS3_usingTLS_get();
		void AS3_constructor();
		void AS3_addHeader(Stringp,bool,Atom);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_call(Stringp,ResponderObject*,ArrayObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_close();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_connect(Stringp,ArrayObject*);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		Stringp registerResponse(ResponderObject*);
		ResponderObject* callbackResponse(Stringp,bool&isResult);

		//new adds
		Stringp AS3_farID_get();
		Stringp AS3_farNonce_get();
		double AS3_httpIdleTimeout_get();
		void AS3_httpIdleTimeout_set(double httpIdleTimeout);
		uint32_t AS3_maxPeerConnections_get();
		void AS3_maxPeerConnections_set(uint32_t maxPeerConnections);
		Stringp AS3_nearID_get();
		Stringp AS3_nearNonce_get();
		Stringp AS3_protocol_get();
		ArrayObject* AS3_unconnectedPeerStreams_get();
		//new adds end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetConnectionObjectSlots m_slots_NetConnectionObject;
};}
#endif
