#include "StdAfxflash.h"
#include "sobject.h"
#include "splayer.h"
#include "sdisplay.h"
#include "avmplus.h"
 
#include "ShellCore.h"
#include "flexInit.h"

#include "AS3Event.h"
#include "AS3DisplayObject.h"
#include "AS3LoaderInfo.h"
#include "AS3ApplicationDomain.h"
#include "AS3Keyboard.h"


/*#include "AS3ActivityEvent.h"
#include "AS3AsyncErrorEvent.h"
#include "AS3ContextMenuEvent.h"
#include "AS3DataEvent.h"
#include "AS3ErrorEvent.h"
#include "AS3FocusEvent.h"
#include "AS3FullScreenEvent.h"
#include "AS3HTTPStatusEvent.h"
#include "AS3IMEEvent.h"
#include "AS3IOErrorEvent.h"
#include "AS3KeyboardEvent.h"
#include "AS3MouseEvent.h"
#include "AS3NetStatusEvent.h"
#include "AS3ProgressEvent.h"
#include "AS3SecurityErrorEvent.h"
#include "AS3StatusEvent.h"
#include "AS3SyncEvent.h"
#include "AS3TextEvent.h"
#include "AS3TimerEvent.h"*/

#include "AS3Timer.h"
#include "AS3Bitmap.h"
#include "AS3Graphics.h"
#include "AS3SimpleButton.h"
#include "AS3ByteArray.h"
#include "AS3SecurityDomain.h"
#include "AS3Loader.h"
#include "AS3LocalConnection.h"
#include "AS3Video.h"
#include "AS3DisplayObjectContainer.h"
#include "AS3Stage.h"

#include "avm2.h"
#include "XFastAlloc.h"


//#define CODEENTER(ARG) ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)ARG);
#define CODEENTER(ARG)
//#define GCEnter() MMGC_GCENTER(m_pGC)
#define GCEnter() //m_pCore->setLimitStack

namespace avmshell
{	
	bool m_bRunning=false;
	bool m_bDestroying=false;

#ifdef _WINEMU
	void TraceOut(ShellCore*pCore,Exception*e,const char*s1,const char*s2)
	{
		XSWFPlayer*player=pCore->GetPlayer();
		//ErrorObject*obj=AvmCore::atomToScriptObject(e->atom);
		Stringp s;
		if(e)
			s=pCore->string(e->atom);
		else
			s=pCore->kEmptyString;
		StUTF16String s16(s);
		
		if(s1||s2)
		{
			XString16 out(s16.c_str(),s16.length());
			out+=XString16("(");
			if(s1)
				out+=XString16(s1);
			if(s2)
			{
				if(s1)
					out+=XString16(" ");
				out+=XString16(s2);
			}
			out+=XString16(")");
			player->Trace(out);
		}
		else player->Trace(s16.c_str());
#ifdef DEBUGGER
#ifdef _DEBUG//DEBUGGER
		if(e)
		{
			avmplus::StackTrace*trace=e->getStackTrace();
			Stringp s=trace->format(pCore);
			StUTF16String st(s);
			player->Trace(st.c_str());
		}
#endif		
#endif
	}
	void TraceInt(ShellCore*pCore,Exception*e,const char*s1,int i2)
	{
		XString8 s("ID:");
		s.FromInt(i2);
		TraceOut(pCore,e,s1,s);
	}
#endif

	//XAVM2* m_pAVM2=NULL;
	XAVM2* InitAVM2(void*player,bool bFlex)
	{
		//if(m_pAVM2==NULL)
		XAVM2* m_pAVM2=new XAVM2();
		if(m_pAVM2)
		{
			XSWFPlayer*p=(XSWFPlayer*)player;
			int nFlex=0;
			if(p->m_pDomFilter)
				nFlex=p->m_pDomFilter->nFlex;
			//	bFlex=p->m_pDomFilter->bFlex;
			//else
			//	bFlex=false;
			m_pAVM2->InitAVM2((XSWFPlayer*)player,nFlex);
			//m_pAVM2->OnKeyDown(0);
		}
		m_bRunning=true;
		return m_pAVM2;
		//else
		//	return -1;
	}
	void ReleaseAVM2(XAVM2*m_pAVM2)//void)
	{
		//if(m_pAVM2)
		{
			m_bRunning=false;
			m_pAVM2->ReleaseAVM2();
			delete m_pAVM2;
			//m_pAVM2=NULL;
		}
	}

	int XAVM2::HandleABC(const void* pCode,int nLength,int nFlags,void*pContext)
	{
		//MMgc::GC *gc=m_pCore->GetGC();
		//MMGC_GCENTER(gc);
		//m_pCore->SetRoot(root);
		ShellCodeContext*p=(ShellCodeContext*)pContext;
		if(p&&p->m_pLoaderInfo)
			this->PushObject(p->m_pLoaderInfo);
		//else
		//	int v=0;
		PoolObject*pool = m_pCore->handleABC(pCode,nLength,nFlags,p);//!=NULL;
		if(pool==NULL) return -1;
		//if(pool->getClassCount()<=1)
		//	return 0;
		//return 1;
		return pool->getClassCount();
	}
	
	XAVM2::XAVM2()
	{
		m_pFrame=NULL;
		m_pGC=NULL;
		//m_pEnter=NULL;
		m_pCore=NULL;
//		m_pGCEnter=NULL;
		m_pSettings=new ShellCoreSettings;
		m_pConfig=new MMgc::GCHeapConfig;
		m_pPoolObj=NULL;
		m_flexPlayer=NULL;
	}
	XAVM2::~XAVM2()
	{
		if(m_pSettings)
			delete m_pSettings;
		if(m_pConfig)
			delete m_pConfig;
		//if(m_pGCEnter)
		//	delete m_pGCEnter;
		//m_pGCEnter=NULL;
		m_pConfig=NULL;
		m_pSettings=NULL;
		ReleaseAVM2();
	}

	int XAVM2::InitAVM2(XSWFPlayer*player,int nFlex)
	{
		if(m_pGC!=NULL||m_pFrame!=NULL||m_pCore!=NULL)
			return -1;
		MMgc::GCHeap::EnterLockInit();
		//MMgc::GCHeapConfig conf;
		//conf.verbose = AvmCore::DEFAULT_VERBOSE_ON;
		MMgc::GCHeap::Init(*m_pConfig);//conf);

		MMgc::GCHeap::EnterLock();
		if(MMgc::GCHeap::ShouldNotEnter())
		{
			MMgc::GCHeap::EnterRelease();
			return OUT_OF_MEMORY;
		}
		//MMgc::EnterFrame _ef;
		//if(m_pFrame==NULL)
		m_pFrame=new MMgc::EnterFrame();
		//int v=sizeof(MMgc::EnterFrame);
		//m_pGCEnter=new MMgc::GCAutoEnter(NULL);
		if(m_pFrame==NULL) return OUT_OF_MEMORY;
		MMgc::GCHeap::EnterRelease();
		m_pFrame->status = VMPI_setjmpNoUnwind(m_pFrame->jmpbuf);
		m_pGC=mmfx_new( MMgc::GC(MMgc::GCHeap::GetGCHeap(), MMgc::GC::kIncrementalGC));
		//m_pEnter=new MMgc::GCAutoEnter(m_pGC);
		MMGC_GCENTER(m_pGC);

		m_pCore=new ShellCore(player,m_pGC);

		if(!m_pCore->setup(*m_pSettings)) 
			return OUT_OF_MEMORY;

		if(nFlex)
		{
			player->m_pAVM2=this;
			m_flexPlayer=FLEX::InitFlex(player,this,m_pCore->GetShellContext(),nFlex);
		}

		if(!player->display.root.pASObject)
		{
			ShellToplevel*t=(ShellToplevel*)m_pCore->GetToplevel();
			StageObject*p=t->getStageClass()->CreateStage(XNULL);
			//p->IncrementRef();
			player->display.root.pASObject=p;
			t->PushObject(p);			
			//player->display.root.pASObject=pStage=p;
		}
		//ShellCodeContext*pCode=new (m_pCore->GetGC())ShellCodeContext(m_pCore,NULL,m_pCore->GetGC());
		/*XFile file;
		file.Open("e:\\dat.dat",XFile::XREAD|XFile::XBINARY);
		int l=file.GetLength();
		ByteArrayObject*p=((ShellToplevel*)m_pCore->GetToplevel())->getByteArrayClass()->CreateArray(l);
		file.Seek(0,XFile::XSEEK_SET);
		file.Read(p->GetByteArray().GetBuffer(),l);
		file.Close();
		p->AS3_uncompress();*/
		//FindClass("flash.accessibility.Accessibility");//flash.accessibility.Accessibility");

		return 0;
	}

	//MMgc::GC* XAVM2::GetGC(){return m_pCore->GetGC();}
	void XAVM2::ReleaseData()
	{
		if(!m_pCore) return;
		if(m_flexPlayer)
			FLEX::ReleaseFlex(m_flexPlayer);
		//ContextEnter(m_pCore,(ShellCodeContext*)pContext);
		RCHashTable*hash=((ShellToplevel*)m_pCore->GetToplevel())->GetReleaseObjects();
		if(!hash) return;
		for(int i=0;i<hash->numQuads;i++)
		{
			EventDispatcherObject*pObj=(EventDispatcherObject*)hash->valueAt(i);
			if(!pObj||pObj==hash->m_pDump) continue;
			pObj->ReleaseData();
		}
		hash->clear();
	}
	void XAVM2::ReleaseAVM2(void)
	{
		if(!m_pCore) return;

		//if(m_pGCEnter)
		//	delete m_pGCEnter;
		//m_pGCEnter=NULL;
		//XSWFPlayer*player=m_pCore->GetPlayer();
		
		//if(m_pEnter)
		//	delete m_pEnter;
		//m_pEnter=NULL;
		XSWFPlayer*player=m_pCore->GetPlayer();
		if(m_pCore) 
		{
			m_pCore->Release();
			delete m_pCore;
		}
		m_pCore=NULL;


#ifdef _WINEMU
		XVector<ShellToplevel::XXObjects::OBJITEM*> &objs=ShellToplevel::m_allObjects.objs;
#endif	
		//player->Trace(XString16("Release"));

		if(m_pGC) 
			mmfx_delete(m_pGC);
		
		m_pGC=NULL;
		
		if(m_pFrame)
			delete m_pFrame;
		m_pFrame=NULL;
	#ifdef AVMPLUS_WITH_JNI
		// This surely does not belong here?
		if (Java::startup_options) delete Java::startup_options;
	#endif /* AVMPLUS_WITH_JNI */
	
		
		MMgc::GCHeap::Destroy();
		MMgc::GCHeap::EnterLockDestroy();

//		chainAlloc.ReleaseData();


#ifdef _WINEMU
		if(objs.GetSize())
		{
			GCDebugMsg(false,"GC Class memory leaks!");
			int i,iCount=objs.GetSize();
			for(i=0;i<iCount;i++)
			{
				ShellToplevel::XXObjects::OBJITEM*pObj=objs[i];
				if(!pObj) continue;
				GCDebugMsg(false,"GC Class(%s) memory leaks!",pObj->strName);
			}
		}
		objs.RemoveAll();		
#endif


	}

	avmplus::ScriptObject* XAVM2::FindClass(const char* strName,void*pContext,bool bMust)
	{		
		//bool bTrace=pContext!=NULL;
		GCEnter();
		//if(strstr(strName,"WelcomeIcon_WelcomeScreenGraphic"))
		//	int v=0;
		TRY(m_pCore,kCatchAction_ReportAsError)
		{
			return m_pCore->FindClass(strName,(ShellCodeContext*)pContext,false);
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"Find Class",strName);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return NULL;
			//AvmAssert(false);
		}
		END_CATCH
		END_TRY
		return NULL;
	}
	
	void XAVM2::RemoveSystem(void*p)
	{
		EventDispatcherObject*pObj=(EventDispatcherObject*)p;
		pObj->RemoveSystem();
	}

	bool XAVM2::ClearMember(void*p,const char*name)
	{
//#if (__CORE_VERSION__>=0x02075000)
//#else
//		return true;
//#endif
		//if(strcmp(name,"closeDoor")==0)
		//	int v=0;
		ScriptObject*pRet=(ScriptObject*)p;
		Traits* t=pRet->traits();
		if(!pRet->vtable||!t->itraits) return false;
		Stringp str=m_pCore->internStringUTF8(name);
		
		
		Binding b=t->getTraitsBindings()->findBinding(str,m_pCore->findPublicNamespace());
		//ScriptObject* ctor = pRet->getSlotObject(m_pCore->bindingToSlotId(b));
		//Traits* traits = this->traits();
		if(b==BIND_NONE) 
		{
			if(pRet->traits()->needsHashtable())
			{
				if(pRet->hasAtomProperty(str->atom()))
				{
					pRet->setAtomProperty(str->atom(),nullObjectAtom);
					return true;
				}
			}
			return false;//getSlotObject
		}
		
		pRet->coerceAndSetSlotAtom(m_pCore->bindingToSlotId(b),nullObjectAtom);
		return true;
	}

	bool XAVM2::SetMember(void*p,const char* strName,void*pObject)
	{
		//return false;
		//if(strcmp(strName,"window_mc")==0)
		//	int v=0;
		if ( p == NULL )
		{
			return false;
		}
		ScriptObject*pRet=(ScriptObject*)p;
		Stringp str=m_pCore->internStringUTF8(strName);
		Atom value=((ScriptObject*)pObject)->atom();
		//Atom aa=pRet->hasAtomProperty(str->atom());
		
		Traits* t=pRet->traits();
		
		return ShellCore::_setProperty(pRet,str->atom(),value,m_pCore->findPublicNamespace(),t->getTraitsBindings());
		/*Binding b=t->getTraitsBindings()->findBinding(str,m_pCore->findPublicNamespace());
		//ScriptObject* ctor = pRet->getSlotObject(m_pCore->bindingToSlotId(b));
		//Traits* traits = this->traits();
		if(b==BIND_NONE) 
		{
			if(pRet->traits()->needsHashtable())
			{
				pRet->setAtomProperty(str->atom(),((ScriptObject*)pObject)->atom());
				return true;
			}
			return false;//getSlotObject
		}
		if((AvmCore::bindingKind(b) & 6) == BKIND_VAR)
			pRet->coerceAndSetSlotAtom(m_pCore->bindingToSlotId(b),((ScriptObject*)pObject)->atom());
		else return false;*/
		//td->setSlotInfo(
		// based on profiling of Flex apps, it's *much* more common for the slot in this case
		// to have a type (vs "atom"), so check for that first...
		/*if (sst == SST_scriptobject)
		{
			return *((ScriptObject**)p);
		}
		else if (sst == SST_atom)
		{
			Atom const a = *((const Atom*)p);

			// don't call AvmCore::isObject(); it checks for null, which we don't care about here
			if (atomKind(a) == kObjectType)
				return (ScriptObject*)atomPtr(a);
			
			// else fall thru and return null
		}*/
		//pRet->setSlot
		//MultinameHashtable* hash=t->getTraitsBindings()->m_bindings;
		//Namespacep ns=m_pCore->findPublicNamespace();
		//Binding b=hash->get(str,ns);
		//if(b!=BIND_NONE)
		//{

		//}
		//sp->m_bindings

//BIND_NONE
//atomKind
		//Traits t=pRet->traits();
		//t->getTraitsBindings()->findBinding(str, m_pCore->findPublicNamespace())
		//Atom aa=pRet->hasAtomProperty(str->atom());
		//if(atomKind(aa)!=kSpecialType) return false;
		//pRet->setStringProperty(str,((ScriptObject*)pObject)->atom());
		return true;
			//pRet->setStringProperty(
			//ScriptObject* o=AvmCore::atomToScriptObject(aa);
	}

	
	bool XAVM2::CreateAVM1(_XSObject*s,void*pContext,void**instance,void*parent)
	{
		ShellToplevel*top=(ShellToplevel*)m_pCore->GetToplevel();
		AVM1MovieClass*pClass=top->getAVM1MovieClass();
		if(!CreateInstance(s,(ScriptObject*)pClass,instance,pContext,parent)) return false;
		FlashObject*pRet=(FlashObject*)instance;
		s->AS3FillMembers(pRet,this);
		return true;
	}
	
	bool XAVM2::CreateInstance(_XSObject*s,avmplus::ScriptObject*pc,void**instance,void*pContext,void*parent)
	{
		register ClassClosure*pClass=(ClassClosure*)pc;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
			//Atom atom=pClass->atom();
		
		VTable* ivtable = pClass->ivtable();
		
		//if (prototype == NULL) // ES3 spec, 13.2.2 (we've already ensured prototype is either an Object or null)
		ScriptObject*prototype = pClass->prototype;
			//AvmCore::atomToScriptObject(m_pCore->GetToplevel()->objectClass->get_prototype());

		//ScriptObject *obj = createInstance(ivtable, prototype);
		FlashObject*pRet=(FlashObject*)pClass->createInstance(ivtable,prototype);
			//
		//newInstance();
		pRet->SetCodeContext((ShellCodeContext*)pContext);//m_pCodeContext=(ShellCodeContext*)pContext;
		DisplayObjectContainerObject*pCon=(DisplayObjectContainerObject*)parent;
		bool bCon=false;
		if(pCon)
		{
			ShellToplevel*top=(ShellToplevel*)m_pCore->GetToplevel();
			if(ShellCore::IsTypeClass(pCon,top->getDisplayObjectContainerClass()))
				bCon=true;
		}
		if(*instance)
		{
			register FlashObject*pp =(FlashObject*)*instance;
			//pp->m_pObject=NULL;
			pp->SetObject(NULL);
			if(bCon)
				pCon->RemoveChildRef(pp);
			//((ShellToplevel*)m_pCore->GetToplevel())->RemoveObject(pp);
		}
		if(pRet)
		{
			//((ShellToplevel*)m_pCore->GetToplevel())->PushObject(pRet);
	
			pRet->SetObject(s);
			if(bCon)
				pCon->AddChildRef(pRet);
			//pRet->m_pObject=(_XSObject*)s;
			//pRet->m_pLoadInfo=pRet->m_pCodeContext->m_pLoaderInfo;
		}
		*instance=pRet;
		return true;
	}
#ifdef _WINEMU64
	bool XAVM2::ConstructInstance(_XSObject*s,void*pc,void*instance,void*pContext,int argc,_int64* args)//avmplus::ScriptObject *pc,void**instance,void*p,void*pContext)
#else
	bool XAVM2::ConstructInstance(_XSObject*s,void*pc,void*instance,void*pContext,int argc,int* args)//avmplus::ScriptObject *pc,void**instance,void*p,void*pContext)
#endif
	{
//		MMgc::GC* gc=m_pCore->GetGC();
		//MMGC_GCENTER(gc);
		//return false;
		bool bOK=false;
		if(!instance) return bOK;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		GCEnter();
		CODEENTER(pContext);
		TRY(m_pCore,kCatchAction_ReportAsError)
		{
			register ClassClosure*pClass=(ClassClosure*)pc;

			FlashObject*pRet=(FlashObject*)instance;
			{			

				VTable* ivtable = pClass->ivtable();
				AvmAssert(ivtable != NULL);
				//Atom a = pRet->atom();
				args[0]=pRet->atom();
				ivtable->init->coerceEnter(argc,args);//0,&a);//argc, argv);				
				bOK=true;
			}
			
			//return pRet!=NULL;
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"Construct",s->name.strTxt);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		
		return bOK;

		//pRet->call(0,&atom);
		//GCLeave();
		
	}
	void XAVM2::ReleaseInstance(void**instance,void*pContext,bool bDisplay,void*parent)
	{
		/*if(*instance)
		{
			MMgc::GC* gc=m_pCore->GetGC();
			//gc->AddToZ
		}
		*instance=NULL;*/
		DisplayObjectContainerObject*pCon=(DisplayObjectContainerObject*)parent;
		bool bCon=false;
		if(pCon)
		{
			ShellToplevel*top=(ShellToplevel*)m_pCore->GetToplevel();
			if(ShellCore::IsTypeClass(pCon,top->getDisplayObjectContainerClass()))
				bCon=true;
		}
		if(*instance&&!m_bDestroying)
		{
			if(bDisplay)
			{
				register FlashObject*pp =(FlashObject*)*instance;
				//StUTF8String s(pp->vtable->traits->name());
				//if(strcmp(s.c_str(),"Map")==0)
				//	int v=0;
				//if(pp->m_pObject&&pp->m_pObject->character->tag==460)
				//	int v=0;
				//pp->m_pObject=NULL;
				pp->ReleaseObject();
				if(m_pCore)
				{

					ShellToplevel*top=((ShellToplevel*)m_pCore->GetToplevel());
					//m_pCore->RemovePost(pp);
					if(pContext)
//#if (__CORE_VERSION__>=0x02070200)
						m_pCore->RemovePostEvent(pp);
//#else
//#ifdef _CODEPOST_
//						((ShellToplevel*)this->m_pCore->GetToplevel())->m_eventsBinding->remove(pp,true);
//#else
//						
//						((ShellCodeContext*)pContext)->m_eventsBinding->remove(pp,true);
//#endif
//#endif
					//top->RemoveObject(pp);
				}
				if(bCon)
					pCon->RemoveChildRef(pp);
				//pp->DecrementRef();
			}
			else
			{
				ScriptObject*p=(ScriptObject*)*instance;
				//p->ReleaseObject();
				//if(m_pCore)
				//	((ShellToplevel*)m_pCore->GetToplevel())->RemoveObject(p);
				if(bCon)
					pCon->RemoveChildRef(p);
				//p->DecrementRef();
			}
		}
		*instance=NULL;
		//*((MMgc::WriteBarrierRC<ScriptObject*>*)instance)=NULL;
		//m_pCore->GetGC()->Clear
		//*instance=NULL;
		//WB_NULL(instance);
		//*instance=NULL;
		//WB_NULL(instance);
	}
	
	/*void XAVM2::GCEnter()
	{
		if(!m_pGCEnter) return;
		MMgc::GC* gc=m_pCore->GetGC();
		m_pGCEnter->Enter(gc);
	}
	void XAVM2::GCLeave()
	{
		if(!m_pGCEnter) return;
		m_pGCEnter->~GCAutoEnter();
	}*/
	bool XAVM2::CallFrame(void*pInst,int iFrame)
	{
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		GCEnter();
		CODEENTER(pContext);
		TRY(m_pCore,kCatchAction_ReportAsError)
		{
			DisplayObjectObject* pObj=(DisplayObjectObject*)pInst;
			return pObj->CallFrame(iFrame);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Call Frame",iFrame);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return false;
	}
	bool XAVM2::CheckRCObjects(_XSObject*root)
	{
		if(!m_pCore) return false;
		/*ShellToplevel*top=(ShellToplevel*)m_pCore->GetToplevel();
		RCHashTable*hash=top->pRCObjects;
		DisplayObjectClass*pClass=top->getDisplayObjectClass();
		int i,iInv=0,iSet=0,iKeep=0,iCount=hash->numQuads;
		for(i=0;i<iCount;i++)
		{
			DisplayObjectObject*pObj=(DisplayObjectObject*)hash->valueAt(i);
			
			if(!pObj||pObj==hash->m_pDump) continue;
			if(!ShellCore::IsProtoTypeOf(pClass,pObj))
			{
				iInv++;
			}
			else
			{
				if(!pObj->m_pObject)
					iInv++;
				
				else if(pObj->m_pObject->parent==XNULL)
					iSet++;
				else if(!root->IsParentOf(pObj->m_pObject))
					iKeep++;
			}

		}
		return iSet==0;*/
		return true;
	}
	void XAVM2::ResetEvent(void*p,int id,void*pTarget)
	{
		//((EventObject*)pEvent)->nEventID=id;
		GCEnter();
		EventObject*pEvent=(EventObject*)p;
		pEvent->nEventID=id;
		//pEvent->bBubbles=b;
		pEvent->pTarget=(ScriptObject*)pTarget;
		pEvent->strType=((ShellToplevel*)m_pCore->GetToplevel())->IdToEventType(id);
	}
	void* XAVM2::CreatePostEventObject(int id,int type,bool bBubbles,int argc,int*args)
	{		
		if(!m_pCore) return NULL;
		GCEnter();
		switch(type)
		{
		case XAS3EVENT_EVENT:
			{
				EventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getEventClass();
				EventObject*pObj=pClass->CreateEventObject(id);
				pObj->bBubbles=bBubbles;
				pObj->pTarget=(ScriptObject*)args;
				return pObj;
			}break;
		case XAS3EVENT_MOUSE:
			{
				//MouseEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getMouseEventClass();
				//EventObject*pObj=pClass->CreateEventObject(id,args);
				//pObj->bBubbles=bBubbles;
				//return pObj;
				AvmAssert(false);
			}break;
		case XAS3EVENT_ACTIVITY:
			{
				/*ActivityEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getActivityEventClass();
				ActivityEventObject*pEvent=pClass->CreateEventObject(id,args[1]!=0);
				pEvent->pTarget=(ScriptObject*)args[0];
				pEvent->nEventID=id;
				return pEvent;*/
				AvmAssert(false);
			}break;
		case XAS3EVENT_ASYNCERROR:
			{
				/*AsyncErrorEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getAsyncErrorEventClass();
				AsyncErrorEventObject*pEvent=pClass->CreateEventObject(id,(Stringp)args[0],(ErrorObject*)args[1]);
				pEvent->pTarget=(ScriptObject*)args[0];
				pEvent->nEventID=id;
				return pEvent;*/
				AvmAssert(false);
			}break;
		case XAS3EVENT_CONTEXTMENU:
			{
				/*ContextMenuEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getContextMenuEventClass();
				ContextMenuEventObject*pEvent=pClass->CreateEventObject(id,(InteractiveObjectObject*)args[1],(InteractiveObjectObject*)args[2]);
				pEvent->pTarget=(ScriptObject*)args[0];
				pEvent->nEventID=id;
				return pEvent;*/
				AvmAssert(false);
			}break;
		case XAS3EVENT_DATA:
			{
				/*DataEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getDataEventClass();
				Stringp str=m_pCore->newStringUTF8((char*)args[1]);
				DataEventObject*pObj=pClass->CreateEventObject(id,str);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				return pObj;*/
				AvmAssert(false);
			}
		case XAS3EVENT_ERROR:
			{
				/*ErrorEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getErrorEventClass();
				Stringp str=m_pCore->newStringUTF8((char*)args[1]);
				ErrorEventObject*pObj=pClass->CreateEventObject(id,str);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				return pObj;*/
				AvmAssert(false);
			}
			break;
		case XAS3EVENT_FOCUS:
			{
				/*FocusEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getFocusEventClass();
				FocusEventObject*pObj=pClass->CreateEventObject(id,(InteractiveObjectObject*)args[1],args[2]!=0,args[0]);
				pObj->pTarget=(ScriptObject*)args[3];
				pObj->nEventID=id;
				pObj->bBubbles=true;
				return pObj;*/
				AvmAssert(false);
			}break;
		case XAS3EVENT_FULLSCREEN:
			{
				/*FullScreenEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getFullScreenEventClass();
				FullScreenEventObject*pObj=pClass->CreateEventObject(id,args[1]!=0);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				return pObj;*/
				AvmAssert(false);
			}
			break;
		case XAS3EVENT_HTTPSTATUS:
			{
				/*HTTPStatusEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getHTTPStatusEventClass();
				HTTPStatusEventObject*pObj=pClass->CreateEventObject(id,args[1]);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				pObj->bBubbles=false;
				return pObj;*/
				AvmAssert(false);
			}break;
		case XAS3EVENT_IME:
			{
				/*IMEEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getIMEEventClass();
				Stringp str=m_pCore->newStringUTF8((char*)args[1]);
				IMEEventObject*pObj=pClass->CreateEventObject(id,str);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				pObj->bBubbles=false;
				return pObj;*/
				AvmAssert(false);
			}break;
		/*case XAS3EVENT_IOERROR:
			{
				IOErrorEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getIOErrorEventClass();
				Stringp str=m_pCore->newStringUTF8((char*)args[1]);
				IOErrorEventObject*pObj=pClass->CreateEventObject(id,str);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				pObj->bBubbles=false;
				return pObj;
			}break;
		case XAS3EVENT_KEYBOARD:
			{
				KeyboardEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getKeyboardEventClass();
				KeyboardEventObject*pObj=pClass->CreateEventObject(id,args[0],args[1],args[2],args[3]!=0,args[4]!=0,args[5]!=0);
				pObj->pTarget=(ScriptObject*)args[6];
				pObj->nEventID=id;
				return pObj;
			}break;
		case XAS3EVENT_NETSTATUS:
			{
				NetStatusEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getNetStatusEventClass();
				NetStatusEventObject*pObj=pClass->CreateEventObject(id,(ScriptObject*)args[1]);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				pObj->bBubbles=false;
				return pObj;
			}
			break;
		case XAS3EVENT_PROGRESS:
			{
				ProgressEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getProgressEventClass();
				ProgressEventObject*pObj=pClass->CreateEventObject(id,args[1],args[2]);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				pObj->bBubbles=false;
				return pObj;
			}break;
		case XAS3EVENT_SECURITYERROR:
			{
				SecurityErrorEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getSecurityErrorEventClass();
				Stringp str=m_pCore->newStringUTF8((char*)args[1]);
				SecurityErrorEventObject*pObj=pClass->CreateEventObject(id,str);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				pObj->bBubbles=false;
				return pObj;
			}break;
		case XAS3EVENT_STATUS:
			{
				StatusEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getStatusEventClass();
				Stringp code=m_pCore->newStringUTF8((char*)args[1]);
				Stringp level=m_pCore->newStringUTF8((char*)args[2]);
				StatusEventObject*pObj=pClass->CreateEventObject(id,code,level);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				pObj->bBubbles=false;
				return pObj;
			}break;
		case XAS3EVENT_SYNC:
			{
				SyncEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getSyncEventClass();
				SyncEventObject*pObj=pClass->CreateEventObject(id,(ArrayObject*)args[1]);
				pObj->pTarget=(ScriptObject*)args[0];
				pObj->nEventID=id;
				pObj->bBubbles=false;
				return pObj;
			}break;

		case XAS3EVENT_TEXT:
			{
				TextEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getTextEventClass();
				Stringp strTxt=argc==0?
								m_pCore->newStringLatin1((char*)args):
								m_pCore->newStringUTF16((U16*)args,argc);
				TextEventObject*pObj=pClass->CreateEventObject(id,strTxt);
				pObj->bBubbles=bBubbles;
				//pObj->m_pText
				//pObj->pTarget=(ScriptObject*)args;
				return pObj;
			}break;
		case XAS3EVENT_TIMER:
			{
				TimerEventClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getTimerEventClass();
				TimerEventObject*pEvent=pClass->CreateEventObject(id);
				pEvent->nEventID=id;
				pEvent->pTarget=(ScriptObject*)args;
				return pEvent;
			}
			 break;*/
		default:
			AvmAssert(false);
			break;
		}
		return NULL;
	}

	void XAVM2::DispatchEvent(void*pTarget,void*pEvent)
	{
		EventDispatcherObject*obj=(EventDispatcherObject*)pTarget;
		EventObject*ent=(EventObject*)pEvent;

		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			obj->DispatchEvent(ent);
			//((EventDispatcherObject*)pInstance)->OnEvent(nType,b);
		}
		CATCH(Exception *exception)
		{
#ifdef _WINEMU
			StUTF8String s(ent->strType);
			_TraceOut(exception,"Call Event",s.c_str());
#endif
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return;
		}
		END_CATCH
		END_TRY
	}

	bool XAVM2::OnCreateModul(void*pInstance)
	{
		if(!pInstance) return false;
		CODEENTER(pContext);
		bool bRet=false;

		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnEvent(m_pCore->kCreateModalWindowRequest,false);
		}
		CATCH(Exception *exception)
		{
			return false;
		}
		END_CATCH
		END_TRY
		return true;
	}

	bool XAVM2::OnEvent(int nType,bool b,void*pInstance,void*pContext,bool bLater)
	{
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		if(!pInstance) return false;
		CODEENTER(pContext);
		bool bRet=false;

		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnEvent(nType,b,NULL,bLater);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Call Event",nType);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		//XString16 si;
		//si.FromInt(nType);
		//XString16 ss("---Event---");
		//ss+=si;
		//XSWFPlayer::m_pInstance->Trace(ss);
		return bRet;
	}
	bool XAVM2::OnMouseEvent(int nType,MOUSEEVENTDATA&data,void*pInstance,void*pContext)
	{
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		/*if(nType==avmshell::XAS3_EVENT_mouseOver)
		{
			XString16 ss("---- Mouse Over Event:"),s1;
			s1.FromInt((int)pInstance,true);
			ss+=s1;
			XSWFPlayer::m_pInstance->Trace(ss);
		}
		else if(nType==avmshell::XAS3_EVENT_mouseOut)
		{
			XString16 ss("---- Mouse Out Event:"),s1;
			s1.FromInt((int)pInstance,true);
			ss+=s1;
			XSWFPlayer::m_pInstance->Trace(ss);
		}*/
		CODEENTER(pContext);
		bool bRet=false;
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnMouseEvent(nType,data);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Mouse Event",nType);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}
	bool XAVM2::OnFocusEvent(int nType,void*pout,void*pin,int nKey,bool bShift,void*pContext)
	{
		bool bRet=false;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pin)->OnFocusEvent(nType,nKey,bShift,(ScriptObject*)pout);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Focus Event",nType);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}
	bool XAVM2::OnKeyEvent(int nType,KEYEVENTDATA&data,void*pInstance,void*pContext)
	{
		bool bRet=false;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnKeyEvent(nType,data);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Key Event",nType);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}
	bool XAVM2::OnTextEvent(int nType,XU16*s16,XU8*s8,void*pInstance,void*pContext)
	{
		bool bRet=false;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnTextEvent(nType,s16,s8);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Text Event",nType);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}
	bool XAVM2::OnHTTPStatusEvent(int nCode,void*pInstance,void*pContext)
	{
		bool bRet=false;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnHTTPStatusEvent(nCode);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"HTTP Status Event",nCode);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}
	bool XAVM2::OnIOErrorEvent(const char*msg,void*pInstance,void*pContext)
	{
		bool bRet=false;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnIOErrorEvent(msg);
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"IO Error Event",msg);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}
	bool XAVM2::OnErrorEvent(const char*msg,void*pInstance,void*pContext)
	{
		bool bRet=false;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnErrorEvent(msg);
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"Error Event",msg);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}
	bool XAVM2::OnProgressEvent(int nType,int nLoaded,int nTotal,void*pInstance,void*pContext)
	{
		bool bRet=false;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((EventDispatcherObject*)pInstance)->OnProgressEvent(nType,nLoaded,nTotal);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Progress Event",nLoaded);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}

	bool XAVM2::OnTimer(int nTime,void*pInstance,void*pContext)
	{
		bool bRet=false;
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			((TimerObject*)pInstance)->OnTimer(nTime);
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"Time Event",NULL);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}
	int  XAVM2::RemoveAllObject(void*player)
	{
		if(m_bDestroying) return 0;
		int iCount=0;
		//return m_pCore->RemoveAllObject(player);
		/*ShellCodeContext*pCnt=m_pCore->m_pContextList;
		while(pCnt)
		{
			RCHashTable*hash=pCnt->m_eventsBinding;
			int i=0,nSize=hash->numQuads;
			for(i=0;i<nSize;i++)
			{
				EventDispatcherObject*p=(EventDispatcherObject*)hash->valueAt(i);
				if(!p||p==hash->m_pDump) continue;
				if(p->m_pCodeContext==NULL) continue;
				if(p->m_pCodeContext->m_pRoot==player)
				{
					hash->Clear(i);
					iCount++;
				}
				
			}
			pCnt=pCnt->m_pNext;
		}*/
		return iCount;
	}
	bool XAVM2::PostEvent(void*pEvent,void*pContext,bool bMask)
	{
		bool bRet=false;
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			GCEnter();			
			if(pEvent)
			{
//#if (__CORE_VERSION__>=0x02070200)
				m_pCore->PostEvent((EventObject*)pEvent,bMask);
//#else
//#ifdef _CODEPOST_
//				m_pCore->PostEvent((EventObject*)pEvent,bMask);
//#else
//				ShellCodeContext*pCnt=(ShellCodeContext*)pContext;
//				if(!pCnt)
//					pCnt=m_pCore->m_pContextList;
//				while(pCnt)
//				{
//					//ShellCore::ContextEnter enter(m_pCore,pCnt);
//					CODEENTER(pCnt);
//					//((EventObject*)pEvent)->m_pCodeContext=pCnt;
//					pCnt->PostEvent((EventObject*)pEvent,bMask);
//					pCnt=pCnt->m_pNext;
//				}
//				
//#endif
//#endif
			}
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"Post Event",NULL);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return bRet;
	}

	//bool XAVM2::HasEvent(

	/*bool XAVM2::DispatchEvent(void*pEvent,void*pObject)
	{
		GCEnter();
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			register EventDispatcherObject*p=(EventDispatcherObject*)pObject;
			return (p->AS3_dispatchEvent((EventObject*)pEvent));
		}
		CATCH(Exception *exception)
		{

		}
		END_CATCH
		END_TRY
		return false;
		//return false;
	}*/
	avmplus::ScriptObject* XAVM2::GetDefaultClass(int id)
	{
		//if(!m_pCore) return NULL;
		ShellToplevel*t=(ShellToplevel*)m_pCore->GetToplevel();
		switch(id)
		{
		case XAS3_CLASS_video:
			 return (ScriptObject*)t->getVideoClass();break;
		case XAS3_CLASS_bitmap:
			 return (ScriptObject*)t->getBitmapClass();break;
		case XAS3_CLASS_shape:
			 return (ScriptObject*)t->getShapeClass();break;
		case XAS3_CLASS_button:
			 return (ScriptObject*)t->getSimpleButtonClass();
		case XAS3_CLASS_edittext:
			 return (ScriptObject*)t->getTextFieldClass();
		case XAS3_CLASS_movieclip:
			 return (ScriptObject*)t->getMovieClipClass();
		case XAS3_CLASS_statictext:
			 return (ScriptObject*)t->getStaticTextClass();
		case XAS3_CLASS_morphshape:
			return (ScriptObject*)t->getMorphShapeClass();
			 //break;
		}
		return NULL;
	}
	bool XAVM2::AddStringToArray(void*pArray,const char* pStr)
	{
		Stringp str=m_pCore->newStringLatin1(pStr);
		Atom atom=str->atom();
		((ArrayObject*)pArray)->push(&atom,1);
		return true;
	}
	/*void* XAVM2::CreateLoaderInfo(void*p,void*pContext)
	{
		//return NULL;
		GCEnter();
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		LoaderInfoClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getLoaderInfoClass();
		LoaderInfoObject*pObj=pClass->CreateLoaderInfo(p);
		//pObj->IncrementRef();
		//pObj->IncrementRef();
		((ShellToplevel*)m_pCore->GetToplevel())->PushObject(pObj);
		return pObj;
	}
	void* XAVM2::CreateLoader(void*p,void*code)
	{
		//return NULL;
		GCEnter();
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)code);
		CODEENTER(pContext);
		LoaderClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getLoaderClass();
		LoaderObject*pObj=pClass->CreateLoader(p,(ShellCodeContext*)code);
		//pObj->IncrementRef();
		((ShellToplevel*)m_pCore->GetToplevel())->PushObject(pObj);
		return pObj;
	}*/

	LoaderInfoObject* XAVM2::InitContext(void*p,void*pApp,void*pLoader,void*pInfo)
	{		
		GCEnter();
		RCObject*pDump=((ShellToplevel*)m_pCore->GetToplevel())->m_pDumpObject.value();
		ShellCodeContext* codeContext = new (m_pCore->GetGC()) ShellCodeContext(m_pCore,(ScriptObject*)pDump,m_pCore->GetGC());
		codeContext->m_pRoot=(ScriptPlayer*)p;
		//ShellCodeContext* pCode=(ShellCodeContext*)pd;
		ApplicationDomainObject*pApplication=(ApplicationDomainObject*)pApp;
		//ShellCodeContext* pp=(ShellCodeContext*)(pCode?pCode:m_pCore->codeContext());
		//if(pp==NULL)
		//	pp=m_pCore->GetShellContext();
		//if(pDomain==NULL)
		//	pDomain=m_pCore->GetShellContext()->m_domainEnv;

		//DomainEnv*pEnv=pd?((ShellCodeContext*)pd)->m_domainEnv:m_pCore->GetToplevel()->domainEnv();
		//ApplicationDomain*pParent=pd?((ShellCodeContext*)pd)->m_pAppDomain:m_pCore->codeContext()
		//Domain*pBase=NULL;
		//codeContext->m_pRoot=(ScriptPlayer*)p;
		if(pApp)//pCode)
		{
			//pBase=pApplication->m_pDomainEnv->domain();
			codeContext->m_domain=pApplication->m_pDomainEnv->domain();//pDomain->domain();////pCode->m_domain;
			codeContext->m_domainEnv=pApplication->m_pDomainEnv;//pDomain;pCode->m_domainEnv;
			codeContext->m_pAppDomain=pApplication;//pCode->m_pAppDomain;
			codeContext->m_pSecurityDomain=
					(SecurityDomainObject*)
					(((ShellToplevel*)m_pCore->GetToplevel())->getSecurityDomainClass()->newInstance());;//pCode->m_pSecurityDomain;
		}
		else
		{
			ShellCodeContext*code=m_pCore->GetShellContext();
			Domain* pDomain = new (m_pCore->GetGC()) Domain(m_pCore, code->m_domainEnv->domain());
				
				// Return a new DomainEnv for the user code
			codeContext->m_domain=pDomain;
			codeContext->m_domainEnv = new (m_pCore->GetGC()) DomainEnv(m_pCore, pDomain, code->m_domainEnv);
			codeContext->m_pAppDomain= ((ShellToplevel*)m_pCore->GetToplevel())->getApplicationDomainClass()->
				CreateAppDomain(code->m_pAppDomain,codeContext,m_pCore->GetToplevel());
			codeContext->m_pSecurityDomain=
					(SecurityDomainObject*)
					(((ShellToplevel*)m_pCore->GetToplevel())->getSecurityDomainClass()->newInstance());
		}
		LoaderInfoObject*pLoaderInfo=(LoaderInfoObject*)pInfo;
		//if(pInfo)
		//	codeContext->m_pLoaderInfo=(LoaderInfoObject*)pInfo;
		//else
		if(!pLoaderInfo)
		{
			LoaderInfoClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getLoaderInfoClass();
			pLoaderInfo=pClass->CreateLoaderInfo(p,codeContext);
			PushObject(pLoaderInfo);
		}
		pLoaderInfo->SetCodeContext(codeContext);//m_pCodeContext=codeContext;
		if(pLoader)
			pLoaderInfo->m_pLoader=(LoaderObject*)pLoader;
		//if(pLoader!=NULL)
		//	codeContext->m_pLoaderInfo->m_pLoader=(LoaderObject*)pLoader;
		/*else
		{
			LoaderClass*pClass2=((ShellToplevel*)m_pCore->GetToplevel())->getLoaderClass();
			codeContext->m_pLoader=pClass2->CreateLoader(p,codeContext);
			PushObject(codeContext->m_pLoader);
		}*/
		m_pCore->SaveContext(codeContext);
		//codeContext->m_pAppDomain->m_pLoader=(LoaderObject*)pLoader;//codeContext->m_pLoader;
		//PushObject(pLoader);
		return pLoaderInfo;
	}	

	void XAVM2::ReleaseContext(void*pContext,bool bDelete)
	{
		GCEnter();
		ShellCodeContext*pc=(ShellCodeContext*)pContext;
		m_pCore->RemoveContext(pc);
		//RemoveObject(pc->m_pLoader
		//this->RemoveObject(((ShellCodeContext*)pContext)->m_pLoader);
		//m_pCore->RemoveContext((ShellCodeContext*)pContext);
		if(bDelete)
			delete (ShellCodeContext*)pContext;
	}
	bool XAVM2::OnKeyDown(int nKey)
	{
		KeyboardClass*pClass=((ShellToplevel*)m_pCore->GetToplevel())->getKeyboardClass();
		//if(nKey==pClass->getSlotCAPS_LOCK())
#ifdef _WINEMU
		//int s=::GetKeyState(20);
		pClass->SetCapsLock((GetKeyState(20)>>7)!=0);
		pClass->SetNumLock((GetKeyState(144)>>7)!=0);
		//pClass->SetNumLock(GetKeyState(144));
#else
		switch(nKey)
		{
		case 20:
			pClass->SetCapsLock(!pClass->GetCapsLock());break;
		case 144:
			pClass->SetNumLock(!pClass->GetNumLock());break;
		}
#endif
		//else if(nKey==pClass->
		return false;
	}
	bool XAVM2::OnKeyUp(int nKey)
	{
		
		return false;
	}

	XU32 XAVM2::CallProperty(void*pObject,const char*strFun,void*pContext)
	{
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		uint32_t a=kAvmThunkNull;
		GCEnter();
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			//FunctionObject*f=(FunctionObject*)p;
			/*ArrayObject*pObj=(ArrayObject*)pObject;
			Stringp name=m_pCore->internStringLatin1(strFun);

			a=f->AS3_apply(f->atom(),p->atom());*/
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"Call Property",strFun);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return a;
	}

	XU32 XAVM2::Call(void*p,void*pa,void*pContext)
	{
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		CODEENTER(pContext);
		uint32_t a=kAvmThunkNull;
		GCEnter();
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			FunctionObject*f=(FunctionObject*)p;
			ArrayObject*p=(ArrayObject*)pa;
			a=f->AS3_apply(f->atom(),p->atom());
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Call Object",(int)p);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return a;
	}

	bool XAVM2::SendMethod(void*pObj,const char*name,const char*domain,const char*method,void*args,int t,void*pContext)
	{
		//ShellCore::ContextEnter enter(m_pCore,(ShellCodeContext*)pContext);
		GCEnter();
		CODEENTER(pContext);
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			ArrayObject*a=(ArrayObject*)args;
			ArrayObject*as=m_pCore->GetToplevel()->arrayClass->newArray(1);
			as->setIntProperty(0,nullObjectAtom);
			if(a)
				as->push(a->GetAtoms(),a->getDenseLength());//get_length());
			switch(t)
			{
			case XAS3CON_LOCAL:
				{
					LocalConnectionObject*f=(LocalConnectionObject*)pObj;
					return f->SendMethod(name,domain,method,as);
				}break;
			}
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"Send Method",name);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return false;
	}

	/*XDrawDib* XAVM2::GetDrawDibData(void*pDib,void**sbitmap)
	{
		//BitmapDataObject*pBitmap=(BitmapObject*)pDib;
		
		BitmapDataObject*pBmp=(BitmapDataObject*)pDib;
		pBmp->LockBitmap();
		//if(!pBmp) return NULL;
		if(sbitmap)//&&!pBmp->m_sbitmap.HasBits())
		{
			
			if(pBmp->m_pDib&&!pBmp->m_pDib->IsAttach())
			{
				if(!pBmp->m_sbitmap.HasBits())
					pBmp->m_sbitmap.PICreate(*pBmp->m_pDib,pBmp->m_bTransparent,pBmp);
			}
			else
			{
				SCharacter*ch=(SCharacter*)pBmp->m_pChar;
				if(ch)
				{ if(ch->type==bitsChar)
					ch->splayer->BuildBits(ch);				
				  else if(ch->type==loaderImageChar)
				  {
					  if(!ch->bits.HasBits())
					  {
						 SetActive(ch);						
						 ch->bits.PICreate(ch->data,ch->length);
						 ch->bits.CalcTrans();
					   }
					}
				}
			}
			
		}
		if(pBmp->m_pDib&&!pBmp->m_pDib->IsAttach())
			*sbitmap=&pBmp->m_sbitmap;
		else if(pBmp->m_pChar)
			*sbitmap=&((SCharacter*)pBmp->m_pChar)->bits;
		return pBmp->m_pDib;
	}

	XDrawDib* XAVM2::GetDrawDib(void*pDib,void**sbitmap)
	{
		BitmapObject*pBitmap=(BitmapObject*)pDib;
		BitmapDataObject*pBmp=pBitmap->m_pBitmapData;
		if(pBmp)
			pBmp->LockBitmap();
		if(!pBmp||(!pBmp->m_pDib&&!pBmp->m_pChar))
			return NULL;
		if(sbitmap)//&&!pBitmap->m_sbitmap.HasBits())
		{
			
			if(pBmp->m_pDib&&!pBmp->m_pDib->IsAttach())
			{
				if(!pBmp->m_sbitmap.HasBits())
					pBmp->m_sbitmap.PICreate(*pBmp->m_pDib,pBmp->m_bTransparent,pBmp);
				*sbitmap=&pBmp->m_sbitmap;
			}
			else
			{
				SCharacter*ch=(SCharacter*)pBmp->m_pChar;
				if(ch)//&&ch->type==bitsChar)
				{
					if(ch->type==bitsChar)
						ch->splayer->BuildBits(ch);
					else if(ch->type==loaderImageChar)
					{
						if(!ch->bits.HasBits())
						{
							SetActive(ch);
							ch->bits.PICreate(ch->data,ch->length);
							ch->bits.CalcTrans();
						}
					}
				}
				*sbitmap=&ch->bits;
			}
			
		}
		return pBmp->m_pDib;
	}*/
	void XAVM2::ReleaseImage(const char*url)
	{
		if(!m_pCore) return;
		Stringp s=m_pCore->internStringLatin1(url);
		((ShellToplevel*)m_pCore->GetToplevel())->UnLockImage(s);
	}

	void XAVM2::SetActive(void*c)
	{
		_XSCharacter*ch=(_XSCharacter*)c;
		if(ch->data||!ch->length||!ch->classEntry)
			return;
		((ShellToplevel*)m_pCore->GetToplevel())->SetActive((Stringp)ch->classEntry);
	}

	

	/*XDrawDib* XAVM2::GetVideoDib(void*pDib,void**sbitmap)
	{
		VideoObject*pBitmap=(VideoObject*)pDib;		
		if(!pBitmap->m_sbitmap.HasBits())
		{
			pBitmap->m_sbitmap.PICreate(pBitmap->m_dib,false,NULL);
			
		}
		*sbitmap=&pBitmap->m_sbitmap;
		return &pBitmap->m_dib;
	}*/

	_XCOMMAND* XAVM2::GetCommands(void*pDisplay)
	{
		DisplayObjectObject*p=(DisplayObjectObject*)pDisplay;
		GraphicsObject*pObj=p->GetGraphics();
		return pObj?pObj->m_pCmds:NULL;
	}

	void XAVM2::SetStateObject(void*pInstance,void*pObject,int state)
	{
		SimpleButtonObject*pButton=(SimpleButtonObject*)pInstance;
		DisplayObjectObject*pObj=(DisplayObjectObject*)pObject;
		if(state&sbtnUpState)
			 pButton->m_pUpState=pObj;
		if(state&sbtnDownState)
			 pButton->m_pDownState=pObj;
		if(state&sbtnHitTestState)
			 pButton->m_pHitState=pObj;
		if(state&sbtnOverState)
			 pButton->m_pOverState=pObj;
		pObj->m_bNewObject=true;
		//m_pCore->GetPlayer()->AS3Push(pObj->m_pObject);
	}

	/*_XSObject* SetButtonstate(void*pInstance,int state,void*pState)
	{
		SimpleButtonObject*pButton=(SimpleButtonObject*)pInstance;
		DisplayObjectObject*pObj=NULL;
		switch(state)
		{
		case sbtnUpState:
			 pObj=pButton->m_pUpState;
			 pButton->m_pUpState=(DisplayObjectObject*)pState;
			 //if(!pButton->m_pUpState)
			 //	 pButton->m_pUpState=pButton->CreateStateObject();
			 //pObj=pButton->m_pUpState;
			 break;
		case sbtnOverState:
			 //if(!pButton->m_pOverState)
			 //	 pButton->m_pOverState=pButton->CreateStateObject();
			 //pObj=pButton->m_pOverState;
			 pObj=pButton->m_pOverState;
			 pButton->m_pOverState=(DisplayObjectObject*)pState;
			 break;
		case sbtnDownState:
			 //if(!pButton->m_pDownState)
			 //	 pButton->m_pDownState=pButton->CreateStateObject();
			 //pObj=pButton->m_pDownState;
			 pObj=pButton->m_pUpState;
			 pButton->m_pUpState=(DisplayObjectObject*)pState;
			 break;
		case sbtnHitTestState:
			 if(!pButton->m_pHitState)
				 pButton->m_pHitState=pButton->CreateStateObject();
			 pObj=pButton->m_pHitState;
			 break;			 
		}
		return pObj->m_pObject;
	}*/
	void XAVM2::FormatButtonState(void*pInstance)
	{
		SimpleButtonObject*pButton=(SimpleButtonObject*)pInstance;
		pButton->FormatState();
	}

	_XSObject* XAVM2::GetButtonState(void*pInstance,int state)
	{
		SimpleButtonObject*pButton=(SimpleButtonObject*)pInstance;
		DisplayObjectObject*pObj=NULL;
		switch(state)
		{
		case sbtnUpState:
			 if(!pButton->m_pUpState)
				 pButton->m_pUpState=pButton->CreateStateObject();
			 pObj=pButton->m_pUpState;
			 break;
		case sbtnOverState:
			 if(!pButton->m_pOverState)
				 pButton->m_pOverState=pButton->CreateStateObject();
			 pObj=pButton->m_pOverState;
			 break;
		case sbtnDownState:
			 if(!pButton->m_pDownState)
				 pButton->m_pDownState=pButton->CreateStateObject();
			 pObj=pButton->m_pDownState;
			 break;
		case sbtnHitTestState:
			 if(!pButton->m_pHitState)
				 pButton->m_pHitState=pButton->CreateStateObject();
			 pObj=pButton->m_pHitState;
			 break;			 
		}
		return pObj->m_pObject;
	}

	_XSObject* XAVM2::GetButtonObject(void*pInstance,int state,int act)
	{
		SimpleButtonObject*pButton=(SimpleButtonObject*)pInstance;
		DisplayObjectObject*pObj=NULL;
		switch(state)
		{
		//case bsNull:
		case bsIdle:
			 pObj=pButton->m_pUpState;
			 break;
		case bsOverUp:
			 pObj=pButton->m_pOverState;
			 break;
		case bsOverDown:
			 pObj=pButton->m_pDownState;
			 break;
		case bsOutDown:
			 pObj=pButton->m_pUpState;
			 break;
		case bsHitTest:
			 pObj=pButton->m_pHitState;
		}
		if(pObj)
		{
			//ShellCore*c=(ShellCore*)core();
			switch(act)
			{
			case XAS3_BUT_REMOVE:
				if(!pObj->m_bNewObject)
				{
					pObj->m_bNewObject=true;
					//m_pCore->GetPlayer()->AS3Push(pObj->m_pObject);
				}break;
			case XAS3_BUT_ADD:
				if(pObj->m_bNewObject)
				{
					pObj->m_bNewObject=false;
					//m_pCore->GetPlayer()->AS3Remove(pObj->m_pObject);
				}break;
			}
		}
		return pObj?pObj->m_pObject:NULL;
	}

	void XAVM2::GetName(void*p,XString16&name)
	{
		if(!p) return;
		GCEnter();
		ScriptObject*pObj=(ScriptObject*)p;
		Traits*t=pObj->traits();
		Stringp s=t->name();
		//StUTF16String u(s);
		String::Buffer*buf=s->GetBuffer();
		int ilen = s->get_length();
		if(buf&&buf->p8)
		{
			XString8 s((char*)(buf->p8),ilen );
			name=s;
		}
		//name=u.c_str();
	}

	bool XAVM2::HasEvent(void*pObj,int id)
	{
		return ((EventDispatcherObject*)pObj)->hasEvent(id);
	}

	bool XAVM2::SetBinaryData(void*pClass,void*pData,int length)
	{
		ShellToplevel*p=(ShellToplevel*)m_pCore->GetToplevel();
		ClassClosure*pc=(ClassClosure*)pClass;
		//if(m_pCore->IsSuperClassOf(p->getByteArrayClass(),pc))
		{
			//pc->m_pInitData=pData;
			VTable*t=pc->ivtable();
			//if(!t->m_pInitData)
			t->m_pInitData=pData;
		}
			//int v=0;
			//pc->m_pInitData=pData;
			//pc->
			//pc->SetInitData(pData,length);
		return true;
	}

	BitmapObject* XAVM2::CreateSBitmap(void*ch,ShellCodeContext*pCode,void*obj,void*parent)
	{
		GCEnter();
		ShellToplevel*p=(ShellToplevel*)m_pCore->GetToplevel();
		BitmapObject*pBitmap=p->getBitmapClass()->CreateSBitmap(ch,pCode,obj);
		DisplayObjectContainerObject*pCon=(DisplayObjectContainerObject*)parent;
		//bool bCon=false;
		if(pCon)
		{
			ShellToplevel*top=(ShellToplevel*)m_pCore->GetToplevel();
			if(ShellCore::IsTypeClass(pCon,top->getDisplayObjectContainerClass()))
				pCon->AddChildRef(pBitmap);
				//bCon=true;
		}
		//if(pBitmap)
		//	((ShellToplevel*)m_pCore->GetToplevel())->PushObject(pBitmap);
			//pBitmap->IncrementRef();
		return pBitmap;
	}

	BitmapObject* XAVM2::CreateBitmap(XImage*pImage,ShellCodeContext*pCode,void*obj,void*parent)
	{
		GCEnter();
		ShellToplevel*p=(ShellToplevel*)m_pCore->GetToplevel();
		BitmapObject*pBitmap=p->getBitmapClass()->CreateBitmap(pImage,pCode,obj);

		DisplayObjectContainerObject*pCon=(DisplayObjectContainerObject*)parent;
		//bool bCon=false;
		if(pCon)
		{
			ShellToplevel*top=(ShellToplevel*)m_pCore->GetToplevel();
			if(ShellCore::IsTypeClass(pCon,top->getDisplayObjectContainerClass()))
				pCon->AddChildRef(pBitmap);
				//bCon=true;
		}
		//if(pBitmap)
		//	((ShellToplevel*)m_pCore->GetToplevel())->PushObject(pBitmap);
			//pBitmap->IncrementRef();
		return pBitmap;
	}
	ShellToplevel* XAVM2::GetToplevel()
	{
		if(!m_pCore) return NULL;
		return (ShellToplevel*)m_pCore->GetToplevel();
	}

	bool  XAVM2::StreamInNew(EventDispatcherObject*obj,void*streamData,char* streamUrl, char* contentType,XString8Array&headList,int nCode,void*pClient,void*loadInfo)
	{		
		GCEnter();
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			//LOGWHERE();
			return obj->StreamInNew(streamData,streamUrl,contentType,headList,nCode,pClient,loadInfo);
		}
		CATCH(Exception *exception)
		{
			_TraceOut(exception,"Stream In New",streamUrl);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return false;
	}
	int XAVM2::StreamInWrite(EventDispatcherObject*obj,void* buffer,int	  length,void*gcEnter,void*pClient)
	{
		GCEnter();
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			return obj->StreamInWrite(buffer,length,gcEnter,pClient);
		}
		CATCH(Exception *exception)
		{
			_TraceInt(exception,"Stream In Write",length);
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return XSWFPlayer::LOAD_BREAK;
	}
	bool XAVM2::StreamInDestroy(EventDispatcherObject*obj,XBOOL bOK,const char*msg,void*pClient)
	{
		GCEnter();
		TRY(m_pCore, kCatchAction_ReportAsError) 
		{
			return obj->StreamInDestroy(bOK,msg,pClient);
		}
		CATCH(Exception *exception)
		{
#ifdef _WINEMU
			XClient*p=(XClient*)pClient;
			XString8 str=msg;
			str+=" ";
			p->GetConnectInfo()->URL.m_strURL;
			_TraceOut(exception,"Stream In Destroy",str);
#endif
			m_pCore->GetPlayer()->SetErrorCode(0x20000000);
			return false;
		}
		END_CATCH
		END_TRY
		return true;
	}

	void XAVM2::GetClassTree(XString8&strTree,void*pObject)
	{
		ScriptObject*obj=(ScriptObject*)pObject;
		Traits*traits=obj->traits();
		while(traits)
		{
			Stringp uri=traits->ns()->getURI();
			Stringp name=traits->name();
			StUTF8String suri(uri);
			StUTF8String sname(name);
			if(suri.length())
			{
				strTree.AddString(suri.c_str(),suri.length());
				strTree.AddString(":",1);
			}
			strTree.AddString(sname.c_str(),sname.length());
			strTree.AddString("\n",1);
			traits=traits->base;
		}
	}

	void XAVM2::Interrupt(bool bExt)
	{
		//ShellCore::interruptTimerCallback(m_pCore);
#ifdef FEATURE_NANOJIT
		m_pCore->Stop();
#endif
		m_pCore->raiseInterrupt(bExt?AvmCore::ExternalInterrupt:AvmCore::ScriptTimeout);
	}

	void XAVM2::PushObject(void*pObject)
	{
		if(!m_pCore) return;
		((ShellToplevel*)m_pCore->GetToplevel())->PushObject((ScriptObject*)pObject);
	}
	void XAVM2::RemoveObject(void*pObject)
	{
		if(!m_pCore) return;
		((ShellToplevel*)m_pCore->GetToplevel())->RemoveObject((ScriptObject*)pObject);
	}
	void XAVM2::ReleaseIntern(XU8*buffer,int len)
	{
		if(!m_pCore||m_bDestroying) return;
		m_pCore->ReleaseIntern(buffer,len);
	}

}

