#ifndef _AS3EventDispatcher_
#define _AS3EventDispatcher_

class ScriptPlayer;
#include "RCHashTable.h"
#include "PROPITEM.h"

#define ADDITEM(ITEM)\
	{ITEM->next=root;root=ITEM;}
	//if(!item) {root=item=ITEM;}\
	//else	  {item->next=ITEM;item=ITEM;}
namespace avmshell
{
	class DictionaryObject;
}
namespace avmplus
{
	namespace NativeID
	{
		class EventDispatcherClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your STATIC AS3 slots here!!!

		private:
		};
		class EventDispatcherObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//avmshell::DictionaryObject*	m_pMethods;
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}

/*struct _PROPITEM
{
	XString16 name;
	XString16 value;
	int		  nType;
	_PROPITEM*next;
}*/


namespace avmshell
{
	
	class EventObject;
	class InteractiveObjectObject;
	class ShellCodeContext;
	class URLRequestObject;
	class LoaderContextObject;
	class LoaderInfoObject;
	class EventDispatcherClass : public ClassClosure
	{
		DWB(WeakKeyHashtable*) m_weakKeys;
	public:
		//int	m_nInEvent;
		EventDispatcherClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		//WeakKeyHashtable *GetWeakTable();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EventDispatcherClassSlots m_slots_EventDispatcherClass;
	};

	class FlashObject : public ScriptObject
	{
	public:
		//static XU32 m_nCreateObjects;
		//DWB(HeapHashtable*) m_methodHash;
		FlashObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0){}
		virtual void LaterCallback(){}
		virtual void SetEnter(bool bEnter){}
		virtual bool IsEnter(){return true;}
		virtual void UpdateCreateTime(){}
		virtual ArrayObject* GetFilters(){return NULL;}		
		virtual void ReleaseObject(){}
		virtual bool CallFrame(int iFrame){return false;}
		virtual void SetCodeContext(ShellCodeContext*pCode){}
		virtual void SetObject(void*pObject){}
		//virtual bool AddCallBack(void*,void*);
		//virtual bool RemoveCallBack(void*,void*);
		//virtual void Release();
	};

class WeakMethodClosure : public MethodClosure
	{
		
	public:
		WeakMethodClosure(MethodClosure*method,VTable*ivtable):MethodClosure(ivtable,method->peek_call(),nullObjectAtom)
		{
			Atom atom=method->peek_savedThis();
			_savedThis=AvmCore::genericObjectToAtom(gc()->GetWeakRef(AvmCore::atomToScriptObject(atom)));
		}
	public:
		// argc is args only, argv[0] = receiver(ignored)
		virtual Atom call(int argc,Atom* argv)
		{
			MMgc::GCWeakRef*ref=(MMgc::GCWeakRef*)AvmCore::atomToGenericObject(_savedThis);
			Atom thisAtom=(Atom)ref->get();
			if(thisAtom==0)
				return undefinedAtom;
			argv[0] = thisAtom|kObjectType;
			return _call->coerceEnter(argc, argv);
		}

		virtual ScriptObject* GetThisObject()
		{	MMgc::GCWeakRef*ref=(MMgc::GCWeakRef*)AvmCore::atomToGenericObject(_savedThis);
			return (ScriptObject*)ref->get();
		}

	//protected:
	//	Atom refAtom;
	};

class EventDispatcherObject : public FlashObject
{
#ifdef _WINEMU
	public:
		XFile m_file;
#endif
	public:
		inline int FindSlotOf(XU32* pDatas,XU32 data,int nSize)
		{
			int is=0,ie=nSize-1,im=(is+ie)>>1;
			while(im>is)
			{
				if(data>pDatas[im])
					ie=im;
				else if(data<pDatas[im])
					is=im;
				else return im;
				im=(is+ie)>>1;
			}
			return pDatas[ie]==data?ie:-1;
		}
#ifdef _CHAINEVENT
		inline ObjectChain* GetSystemEvent(int id)
		{
			if(!m_pEventInfo||!m_pEventInfo->pSysChains)
				return NULL;
			id=m_pEventInfo->pSysChains[id];
			if(id<=0) return NULL;
			return m_pEventInfo->chains->getAt(id-1);
				//m_pEventInfo->chains.getData()[id-1];
			//int nOffset=m_pEventInfo->pSystemChains[id];
		}
		inline ObjectChain* GetChain(EventObject*pEvent);		
#else
		inline ArrayObject* GetSystemEvent(int id)
		{
			if(!m_pEventInfo||!m_pEventInfo->pSysChains)
				return NULL;
			id=m_pEventInfo->pSysChains[id];
			if(id<=0) return NULL;
			return (ArrayObject*)AvmCore::atomToScriptObject(m_pEventInfo->chains->getUintProperty(id-1));
				//m_pEventInfo->chains.getData()[id-1];
			//int nOffset=m_pEventInfo->pSystemChains[id];
		}
		inline ArrayObject* GetChain(EventObject*pEvent);		
#endif
	private:
		class EventInfo:public MMgc::GCObject
		{
		public:
			//List<ObjectChain*> chains;
#ifdef _CHAINEVENT
			DWB(ChainArray*)  chains;
			ObjectChain*	   pChain;
#else
			DRCWB(ArrayObject*) chains;
			ArrayObject*		pChain;
#endif
			DRCWB(Stringp)	     pName;
			DWB(EventHashTable*) pTable;
//#if (__CORE_VERSION__>=0x02070200)
			int					 iSystemCount;
//#else
//			DWB(RCHashTable*)	pSystem;
//#endif
			//DWB(RCHashTable*)	pRCObjects;
			//XU32			   nCreateTime;
			int				   nChains;
			XU16*			   pSysChains;
			EventInfo():pName(NULL),
						pChain(NULL),
						nChains(0),
						chains(NULL),
						pSysChains(NULL)
			{

			}
#ifdef _CHAINEVENT
#else
			int NewObject(ScriptObject*pObject,Toplevel*top)
			{				
				{
					int i,iCount=chains->get_length();
					ArrayObject*pNew=top->arrayClass->newArray(1);
					pNew->setUintProperty(0,pObject->atom());
					for(i=0;i<iCount;i++)
					{
						Atom atom=chains->getUintProperty(i);
						if(AvmCore::isNullOrUndefined(atom))
						{
							chains->setUintProperty(i,pNew->atom());
							return i;
						}
					}
					Atom atom=pNew->atom();
					chains->push(&atom,1);
					return iCount;
				}
			}
			ArrayObject* GetChain(int index)
			{
				return (ArrayObject*)AvmCore::atomToScriptObject(chains->getUintProperty(index));
			}
			ArrayObject* RemoveObject(int index,ScriptObject*pObject)
			{
				if(chains==NULL) return NULL;
				ArrayObject*pChains=(ArrayObject*)AvmCore::atomToScriptObject(chains->getUintProperty(index));
				if(pChains)
				{
					pChains=RemoveChain(pChains,pObject);
					if(pChains)
						chains->setUintProperty(index,pChains->atom());
					else
						chains->setUintProperty(index,nullObjectAtom);
				}
				return pChains;
			}
#endif
			//ObjectChain**	   pSystemChains;
		};
	public:
		virtual void UpdateCreateTime();
		//int a,b,c,d;
		//DWB(MultinameHashtable*) m_eventsBinding;
		//DWB(EventHashTable*) m_pEventTable;
		//struct _XSObject*		m_pObject;
		//XU32 m_nCreateTime;
		virtual void SetCodeContext(ShellCodeContext*pCode);
//#if (__CORE_VERSION__>=0x02077000)
		virtual void SetClient(void*pClient){};
//#endif
		DWB(EventInfo*)		 m_pEventInfo;
		DWB(ShellCodeContext*)m_codeContext;
		//DRCWB(LoaderInfoObject*)m_loaderInfo;
		DRCWB(ScriptObject*) m_pEventObject;
		//DWB(WeakKeyHashtable*)  m_methods;	
		//DRCWB(DictionaryObject*) m_methods;
		//DRCWB(ArrayObject*)  m_methods;
		//DRCWB(ArrayObject*)  m_weekMethods;
		bool				 m_bEventObject;
		//DRCWB(Namespacep) m_pNamespaceObject;
		//Namespacep CreateNamespace();		
	public:
#ifdef _ANALY
		AS3PROPITEM* CreateItem(Stringp name,Atom atom,int type);
		virtual AS3PROPITEM* GetChildren();
#endif
		bool hasMouseEvent();
		bool hasKeyEvent();
		bool hasAnyEvent();
		bool hasEvent(int id,bool bParent=false);
		virtual void LoadBytes(){}
		virtual XU8* GetInBuffer(int l){return NULL;}
		virtual bool OnClose(const char*msg);
		virtual bool OnConnected();
		virtual bool OnRecieve(int nSize);
		virtual void ReleaseData(){}
		virtual bool CheckEventMask(int nCmd);
		virtual EventDispatcherObject* GetParentObject(bool bSeek){return NULL;}
		virtual ScriptPlayer* GetPlayer(){return NULL;}
		virtual bool  StreamInNew(void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo)
		{return false;}
		virtual int StreamInWrite(void* buffer,int	  length,void*gcEnter,void*pClient){return -4;}		
		virtual bool StreamInDestroy(XBOOL bOK,const char*msg,void*pClient){return false;}
		void FreeEventInfo();
		EventDispatcherObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		void RemoveSystem();
		virtual void Release();
	public:
//#if (__CORE_VERSION__>=0x02075000)
		XU32 LoadURL(URLRequestObject*url,LoaderContextObject*pContext);
//#else
//		bool LoadURL(URLRequestObject*url,LoaderContextObject*pContext);
//#endif
		bool OnEvent(Stringp strType,bool bLater);
		bool OnEvent(int nType,bool b,ScriptObject*pTarget=NULL,bool bLater=false);
		bool OnTimerEvent(int nType,ScriptObject*pTarget=NULL);
		bool OnMouseEvent(int nType,MOUSEEVENTDATA&data);
		bool OnFocusEvent(int nType,int nKey,bool bShift,ScriptObject*pout);
		bool OnKeyEvent(int nType,KEYEVENTDATA&data);
		bool OnTextEvent(int nType,XU16*s16,XU8*s8);
		bool OnSyncEvent(ArrayObject*objs);
		bool OnStatusEvent(Stringp strCode,int level);
		bool OnSecurityErrorEvent(const char*txt);
		bool OnProgressEvent(int nType,int loaded,int total);
		bool OnNetStatusEvent(ScriptObject*obj);
		bool OnIOErrorEvent(const char*txt);
		bool OnIMEEvent(const char*txt);
		bool OnHTTPStatusEvent(int nCode);
		bool OnFullScreenEvent(bool b,bool bb);
		bool OnErrorEvent(const char*txt);
		bool OnDataEvent(int nType,const char*data);
		bool OnContextMenuEvent(int nType,InteractiveObjectObject*owner,InteractiveObjectObject*mouseTarget);
		bool OnAsyncErrorEvent(const char*,ErrorObject*obj);
		bool OnActivityEvent(bool);
	public:
		//void PushMethod(ScriptObject*obj,bool bWeekRef);
		//void RemoveMethod(ScriptObject*obj);
		MethodClosure* SetMethod(MethodClosure*method,bool bWeakRef,bool bCreate=true);
		MethodClosure* GetMethod(Stringp strType,MethodClosure*method);

	public:
		//void* pData;//Set your data!!
		void AS3_constructor(Atom obj);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//void AS3_addEventListener(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_addEventListener(Stringp strType,ScriptObject*method,bool useCapture,int priotity,bool useWeekRef);
		bool AS3_dispatchEvent(EventObject*pEvent);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool DispatchEvent(EventObject*pEvent);
		bool CaptureEvent(EventObject*pEvent);
		bool AS3_hasEventListener(Stringp pEvent);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_removeEventListener(Stringp strType,ScriptObject*method,bool);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		bool AS3_willTrigger(Stringp pEvent);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::EventDispatcherObjectSlots m_slots_EventDispatcherObject;
	};
}
#endif
