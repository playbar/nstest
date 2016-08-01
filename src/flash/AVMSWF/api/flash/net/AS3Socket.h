#ifndef _AS3Socket_
#define _AS3Socket_

#include "XClient.h"
#include "XSocket.h"

#include "AS3SocketFile.h"
#include "AS3ByteArray.h"

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class SocketClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class SocketObjectSlots{
friend class SlotOffsetsAndAsserts;
public:

//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SocketObject;
	
	class SocketClass : public ClassClosure//EventDispatcherClass
	{
	public:
		SocketObject * CreateSocket(Stringp host, int32_t port);

	public:
		SocketClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SocketClassSlots m_slots_SocketClass;
};

class SocketObject : public EventDispatcherObject
{
	public:
		SocketObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		void ReleaseClient();
		virtual XU8* GetInBuffer(int l){return m_cache.GetInput(l);}
		virtual bool OnConnected()
		{
			if(!m_pClient)
				return false;
			return  EventDispatcherObject::OnConnected();
				//OnEvent(XAS3_EVENT_connect,false,pObject);
		}
		virtual bool OnRecieve(int nSize)
		{
			//return false;
			if(!m_pClient)
				return false;
			return EventDispatcherObject::OnRecieve(nSize);//OnProgressEvent(XAS3_EVENT_socketData,nSize,0)

		}
		virtual bool OnClose(const char*msg);

	//new adds
		bool m_bNeedRelease;
	public:
		Stringp m_pLocalAddress;
		int m_localPort;
		Stringp m_pRemoteAddress;
		int m_remotePort;
//		uint32_t m_timeout;
	//new adds end

//	public:
//		XU32 m_nTimeout;

//		void* pData;//Set your data!!
		uint32_t	AS3_bytesAvailable_get();
		bool		AS3_connected_get();

		Stringp		AS3_endian_get();
		void		AS3_endian_set(Stringp endianSet);

		uint32_t	AS3_objectEncoding_get();
		void		AS3_objectEncoding_set(uint32_t objectEncoding);

		void		AS3_constructor(Stringp host, int32_t port);

		void		AS3_close();
		void		AS3_connect(Stringp host, int32_t port);
		void		AS3_flush();

		bool		AS3_readBoolean();
		int32_t		AS3_readByte();
		void		AS3_readBytes(ByteArrayObject* byteArray, uint32_t offset, uint32_t length);
		double		AS3_readDouble();
		double		AS3_readFloat();
		int32_t		AS3_readInt();
		Stringp		AS3_readMultiByte(uint32_t length, Stringp charSet);
		Atom		AS3_readObject();
		int32_t		AS3_readShort();
		uint32_t	AS3_readUnsignedByte();
		uint32_t	AS3_readUnsignedInt();
		uint32_t	AS3_readUnsignedShort();
		Stringp		AS3_readUTF();
		Stringp		AS3_readUTFBytes(uint32_t length);

		void		AS3_writeBoolean(bool data);
		void		AS3_writeByte(int32_t data);
		void		AS3_writeBytes(ByteArrayObject *data, uint32_t offset, uint32_t length);
		void		AS3_writeDouble(double data);
		void		AS3_writeFloat(double data);
		void		AS3_writeInt(int32_t data);
		void		AS3_writeMultiByte(Stringp data, Stringp charSet);
		void		AS3_writeObject(Atom);
		void		AS3_writeShort(int32_t data);
		void		AS3_writeUnsignedInt(uint32_t data);
		void		AS3_writeUTF(Stringp data);
		void		AS3_writeUTFBytes(Stringp data);
		//XU32		AS3_timeout_get(){return m_nTimeout;}
		//void		AS3_timeout_set(XU32 nTimeout){m_nTimeout=nTimeout;}
		//new adds
		Stringp AS3_localAddress_get();
		int AS3_localPort_get();
		Stringp AS3_remoteAddress_get();
		int AS3_remotePort_get();
		uint32_t AS3_timeout_get();
		void AS3_timeout_set(uint32_t timeout);
		//new adds end

		//new adds 11
		uint32_t AS3_bytesPending_get();
		//new adds 11 end
	private:
		bool		CheckDomainArea(Stringp host){return true;}
	public:
		//void		ReceiveData(void *pData, int length);
	public:
		//void		OnConnect();
		//void		OnSocketData(int l);
		//void		OnClose();
		//void		OnError(const char*msg);

	public:
		SocketFile	m_cache;

	private:
		int		    m_nTimeout;
		XU32		m_nLoaded;
		XClient		*m_pClient;

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SocketObjectSlots m_slots_SocketObject;
};}
#endif