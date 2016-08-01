#ifndef _AS3URLStream_
#define _AS3URLStream_

#include "AS3URLRequest.h"
#include "AS3ByteArray.h"
#include "AS3EventDispatcher.h"
#include "AS3SocketFile.h"

namespace avmplus{namespace NativeID{
class URLStreamClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class URLStreamObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class URLStreamObject;

	class URLStreamClass : public ClassClosure
	{
	public:
		URLStreamClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		URLStreamObject *CreateURLStream();

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLStreamClassSlots m_slots_URLStreamClass;
};
class URLStreamObject : public EventDispatcherObject
{
	public:
		URLStreamObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
	public:
		virtual bool  StreamInNew(void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo);
		virtual int StreamInWrite(void* buffer,int	  length,void*gcEnter,void*pClient);
		virtual bool StreamInDestroy(XBOOL bOK,const char*msg,void*pClient);
		int			 m_nLoadID;
	public:
		uint32_t	AS3_bytesAvailable_get();
		bool		AS3_connected_get();
		Stringp		AS3_endian_get();
		void		AS3_endian_set(Stringp endian);
		uint32_t	AS3_objectEncoding_get();
		void		AS3_objectEncoding_set(uint32_t objectEncoding);
		void		AS3_close();
		void		AS3_load(URLRequestObject *pRequest);
		bool		AS3_readBoolean();
		int			AS3_readByte();
		void		AS3_readBytes(ByteArrayObject *pBytes, uint32_t offset, uint32_t length);
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
	private:
		uint32_t	m_nTotalBytes,m_nLoadedBytes;
		bool		m_bConnected;
		void*		m_pClient;
		SocketFile	m_data;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLStreamObjectSlots m_slots_URLStreamObject;
};}
#endif