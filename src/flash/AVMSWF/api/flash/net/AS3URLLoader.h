#ifndef _AS3URLLoader_
#define _AS3URLLoader_

#include "AS3URLRequest.h"
#include "AS3EventDispatcher.h"
namespace avmplus{namespace NativeID{
class URLLoaderClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class URLLoaderObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class URLLoaderObject;
	class ByteArrayObject;
	class URLLoaderClass : public ClassClosure
	{
	public:
		URLLoaderObject *CreateURLLoader(URLRequestObject *pRequest);

	public:
		URLLoaderClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLLoaderClassSlots m_slots_URLLoaderClass;
};
class URLLoaderObject : public EventDispatcherObject
{
	public:
		enum {LD_BINARY,LD_TEXT,LD_VAR};
		URLLoaderObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		int		    m_nDataFormat;
		bool		m_bCanceled;
		uint32_t	m_bytesLoaded;
		uint32_t	m_bytesTotal;
		DRCWB(Atom)	m_pData;
		DRCWB(ByteArrayObject*) m_buffer;
		void*		m_pClient;
		Stringp		m_dataFormat;
#ifdef _WINEMU
		DRC(Stringp) m_strURL;
#endif
//#if (__CORE_VERSION__>=0x02077000)
		virtual void SetClient(void*pClient){m_pClient=pClient;};
//#endif
	public:
		virtual bool  StreamInNew(void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo);
		virtual int StreamInWrite(void* buffer,int	  length,void*gcEnter,void*pClient);
		virtual bool StreamInDestroy(XBOOL bOK,const char*msg,void*pClient);
		uint32_t	AS3_bytesLoaded_get();
		void		AS3_bytesLoaded_set(uint32_t bytesLoaded);
		uint32_t	AS3_bytesTotal_get();
		void		AS3_bytesTotal_set(uint32_t bytesTotal);
		Atom		AS3_data_get();
		void		AS3_data_set(Atom atom);
		Stringp		AS3_dataFormat_get();
		void		AS3_dataFormat_set(Stringp dataFormat);
		void		AS3_constructor(URLRequestObject *pRequest);
		void		AS3_close();
		void		AS3_load(URLRequestObject *pRequest);

		//new adds
		void AS3_addEventListener(Stringp type, FunctionObject *listener, bool useCapture, int priority, bool useWeakReference)
		{
			return EventDispatcherObject::AS3_addEventListener(type, listener, useCapture, priority, useWeakReference);
		}
		//new adds end

	private:
//#if (__CORE_VERSION__>=0x02075000)
		XU32	    m_nLoadID;
//#endif
		bool		m_bLoading;
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::URLLoaderObjectSlots m_slots_URLLoaderObject;
};}
#endif