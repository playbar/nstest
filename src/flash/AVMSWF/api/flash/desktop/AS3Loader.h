#ifndef _AS3Loader_
#define _AS3Loader_

#include "AS3DisplayObjectContainer.h"
#include "AS3UncaughtErrorEvents.h"

namespace avmplus{namespace NativeID{
class LoaderClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class LoaderObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}

namespace avmshell{
	class LoaderObject;
	class LoaderInfoObject;
	//class LoaderContextObject;
	class LoaderContextObject;
	class DisplayObjectObject;
	class URLRequestObject;
	class ByteArrayObject;
	class UncaughtErrorEventsObject;
	class LoaderClass : public ClassClosure
	{
	public:
		LoaderClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		LoaderObject* CreateLoader();
		DWB(RCHashTable*) m_pByteObjects;
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LoaderClassSlots m_slots_LoaderClass;
};
class LoaderObject : public DisplayObjectContainerObject
{
	public:
		void CheckAS3();
		LoaderObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void Release();
		virtual void InitObject();
	public:
#ifdef _WINEMU
		XFile m_file;
#endif
		//DWB(ShellCodeContext*) m_codeContext;
		//void* pData;//Set your data!!
		//bool m_bNewPlayer;
		//DRCWB(ApplicationDomainObject*) m_pAppDomain;
		//DRCWB(SecurityDomainObject*)	m_pSecurityDomain;
		//LoaderObject*					m_pLoader;
		DRCWB(LoaderInfoObject*)		m_pContentLoaderInfo;
		DRCWB(ByteArrayObject*)			m_pByteData;
		DRCWB(DisplayObjectObject*)			m_pContent;
		DRCWB(UncaughtErrorEventsObject*) m_pErrorEvents;
		DRCWB(Stringp)					m_url;
		XStream*						m_pStream;
		bool						    m_bImage;
		bool							m_bCache;
		bool							m_bFirst;
		bool							m_bLoading;
		bool							m_bConnecting;
		bool							m_bPush;
		//void*							m_pCache;
		ScriptPlayer*m_pPlayer;
		//bool		 m_bContent;
		DisplayObjectObject* AS3_content_get();
		LoaderInfoObject* AS3_contentLoaderInfo_get();
		void*			  m_pClient;
		virtual void SetClient(void*pClient){m_pClient=pClient;m_nLoadID=0;};
		DisplayObjectObject* AS3_root_get();
		void AS3_constructor();
		void AS3_close();
		void AS3_load(URLRequestObject* url,LoaderContextObject*);
		void AS3_loadBytes(ByteArrayObject*,LoaderContextObject*);
		virtual void LoadBytes();
		void PushData(void*pData,int l,void*gcEnter,bool bAttach,bool bStream);
		void AS3_unload();
		void CacheComplete(void*,const char*msg);
		virtual class ScriptPlayer* GetPlayer();		

		virtual bool  StreamInNew(void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo);
		virtual int StreamInWrite(void* buffer,int	  length,void*gcEnter,void*pClient);
		virtual bool StreamInDestroy(XBOOL bOK,const char*msg,void*pClient);

		UncaughtErrorEventsObject* AS3_uncaughtErrorEvents_get();
		void AS3_loadFilePromise(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void unload(bool bAS3=false);

	private:
		XU32 m_nLoadID;
		XBOOL m_bStoped;
		void Close();
		bool CreateObject();
		bool CreateContent(LoaderContextObject*code);
		//bool CreateLoaderContext();
		//bool CreateCodeContext(LoaderContextObject*);
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::LoaderObjectSlots m_slots_LoaderObject;
};
}
#endif

