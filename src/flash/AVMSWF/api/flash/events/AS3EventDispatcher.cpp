#include "StdAfxflash.h"
#include "splayer.h"
#include "ShellCore.h"
#include "avmplus.h"
#include "sobject.h"
using namespace avmplus;
#include "AS3EventDispatcher.h"
#include "AS3Event.h"
#include "AS3TimerEvent.h"
#include "AS3ActivityEvent.h"
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
#include "AS3TimerEvent.h"
#include "AS3URLRequest.h"
#include "AS3Dictionary.h"

#include "avm2.h"
namespace avmshell{



EventDispatcherClass::EventDispatcherClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	AvmCore*c=core();
	//m_nInEvent=0;
	//m_pStringActivate=c->internStringLatin1(_XAS3_System_name[0]);
	//m_pStringDeactivate=c->internStringLatin1(_XAS3_System_name[1]);
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())EventDispatcherObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};

/*WeakKeyHashtable *EventDispatcherClass::GetWeakTable()
{
	if(m_weakKeys==NULL)
	{
		 m_weakKeys=new (core()->GetGC()) WeakKeyHashtable(core()->GetGC());
	}
	return m_weakKeys;
}*/

ScriptObject* EventDispatcherClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) EventDispatcherObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

void EventDispatcherObject::SetCodeContext(ShellCodeContext*pCode)
{
	m_codeContext=pCode;
	//if(!pCode)
	//	int v=0;
	//if(pCode&&pCode->m_pRoot)
	//	m_loaderInfo=pCode->m_pRoot->m_pLoaderInfo;
}

EventDispatcherObject::EventDispatcherObject(VTable *vtable, ScriptObject* proto, int capacity): FlashObject(vtable, proto, 0)
{
	//m_pNamespaceObject=NULL;
//	m_nCreateTime=FlashObject::m_nCreateObjects;
//	FlashObject::m_nCreateObjects++;
	m_pEventObject=NULL;
	m_pEventInfo=NULL;
	SetCodeContext((ShellCodeContext*)core()->codeContext());
	//m_pCodeContext=(ShellCodeContext*)core()->codeContext();

	//AvmAssert(m_pCodeContext!=NULL);
	/*ShellCore*c=(ShellCore*)core();
	ShellCodeContext*cn=c->GetCodeContext();
		//(ShellCodeContext*)core()->codeContext();
	//if(cn==NULL)
	//	cn=((ShellCore*)core())->GetShellContext();
	if(cn)
		m_pEventTable=cn->m_eventsBinding;//((ShellCodeContext*)core()->codeContext())->m_eventsBinding;*/
	m_bEventObject=false;
//#ifdef _WINEMU
//	((ShellToplevel*)toplevel())->m_allObjects.Add(this);
//#endif
	//traits()->set_needsHashtable(true);
	//Add your construct code here...
}


void EventDispatcherObject::Release()
{
//#ifdef _WINEMU
//	ShellToplevel*top=((ShellToplevel*)toplevel());
	//if(top->pAllObjects)
//	top->m_allObjects.Remove(this);
//#endif
	/*if(IsRunning())
	{
		if(m_bEventObject)
			((ShellCore*)core())->RemovePost(this);
		m_bEventObject=false;
	}*/
	if(m_pEventObject)
	{
		//m_pEventObject->DecrementRef();

		m_pEventObject=NULL;
	}
	if(m_pEventInfo)
	{
		//if(m_pEventTable)
		//	m_pEventTable->clear(this);
		//ShellToplevel*top=(ShellToplevel*)toplevel();
		//top->pSystemEvents->remove(this,true);
		FreeEventInfo();
	}

	//this->m_pEventTable=NULL;
}

void EventDispatcherObject::FreeEventInfo()
{

#ifdef _CHAINEVENT
	m_pEventInfo->chains->clear();
	ShellToplevel*top=(ShellToplevel*)toplevel();
//#if (__CORE_VERSION__>=0x02070200)
	if(m_pEventInfo->iSystemCount>0&&IsRunning())
		((ShellCore*)core())->RemovePostEvent(this);
//#else
//	if(m_pEventInfo->pSystem)
//		m_pEventInfo->pSystem->remove(this,true);
//#endif
	delete m_pEventInfo->chains;
	delete m_pEventInfo->pTable;
#else
	if(IsRunning())
		FreeChain(m_pEventInfo->chains);
	ShellToplevel*top=(ShellToplevel*)toplevel();
//#if (__CORE_VERSION__>=0x02070200)
	if(m_pEventInfo->iSystemCount>0&&IsRunning())
		((ShellCore*)core())->RemovePostEvent(this,NULL);
//#else
//	if(m_pEventInfo->pSystem)
//		m_pEventInfo->pSystem->remove(this,true);
//#endif
	m_pEventInfo->chains=NULL;
	delete m_pEventInfo->pTable;
#endif
	//top->pSystemEvents->remove(this,true);
	//m_pEventTable->clear(this);
	//ShellCore*cc=(ShellCore*)core();
	//ShellCodeContext*cn=(ShellCodeContext*)cc->codeContext();
	//MultinameHashtable*hash=cn->m_eventsBinding;
	//for(int i=0;i<nSize;i++)
	//{
		//if(pChains[i]->pName)
		//	hash->put(pChains[i]->pName,GetNamespace(),0);
	//	FreeChain(&pChains[i],m_pEventInfo->pRCObjects);//top->GetRCHashTable());
	//}
	m_pEventInfo->pName=NULL;
	m_pEventInfo->pTable=NULL;
	if(m_pEventInfo->pSysChains)
		mmfx_free(m_pEventInfo->pSysChains);
//#if (__CORE_VERSION__>=0x02070200)
	m_pEventInfo->iSystemCount=0;
//#else
//	m_pEventInfo->pSystem=NULL;
//#endif
	delete m_pEventInfo;
	m_pEventInfo=NULL;
}

void EventDispatcherObject::RemoveSystem()
{
	if(!m_pEventInfo) return;
	//if(m_bEventObject)
	//	((ShellCore*)core())->RemovePost(this);
	//m_bEventObject=false;
//#if (__CORE_VERSION__>=0x02070200)
	if(IsRunning())
	{
		if(m_pEventInfo->iSystemCount>0)
		{
			((ShellCore*)core())->RemovePostEvent(this);
			m_pEventInfo->iSystemCount=0;
		}
	}
//#else
//	m_pEventInfo->pSystem->remove(this,true);
//#endif
}
//////////////////////////////////////////////////////////
//Native Method start...
//AS3 contructor function..
void EventDispatcherObject::AS3_constructor(Atom obj)//AvmMethodEnv, uint32_t argc, AvmBox* argv)
{
	//Add your act code here...
	//if(p)
	if(obj)
		m_pEventObject=AvmCore::atomToScriptObject(obj);
}


bool EventDispatcherObject::CheckEventMask(int nCmd)
{
	switch(nCmd)
	{
	case XAS3_EVENT_activate:
	case XAS3_EVENT_deactivate:
	case XAS3_EVENT_enterFrame:
//#if (__CORE_VERSION__>=0x02076000)
	case XAS3_EVENT_exitFrame:
//#endif
		 return true;
	}
	return false;
}

void EventDispatcherObject::AS3_addEventListener(Stringp strName,ScriptObject*method,bool useCapture,int priotity,bool useWeekRef)//AvmMethodEnv, uint32_t argc, AvmBox* argv)
{
	//useWeekRef=false;
	//useWeekRef=false;
	//Add your act code here...
	/*enum {
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject
        , argoff2 = argoff1 + AvmThunkArgSize_AvmString
    };*/
	//if(!this->m_pEventTable) return;
	//if(useWeekRef) return;
	//XString16 si;
	//si.FromInt(priotity);
	//XWindow::Trace(si);
	if(!strName)
		return;
//#ifdef _WINEMU
//	StUTF8String s(strName);
//	if(strcmp(s.c_str(),"switch_location")==0)
//		int v=0;
//#endif
	//StUTF8String s(strName);
	//if(strcmp(s.c_str(),"click")==0)
	//	int v=0;

	ShellCore*cc=(ShellCore*)core();
	strName=cc->intern(strName->atom());

	XU32 nCreateTime=cc->GetPlayer()->m_nRunTime;
	//Stringp strName=AvmThunkUnbox_AvmString(argv[argoff1]);
	//ScriptObject* method=AvmThunkUnbox_AvmObject(argv[argoff2]);
	if(!method||!cc->isFunction(method->atom()))
		toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);

	bool bNew=false;
	if(!m_pEventInfo)
	{
		m_pEventInfo=new(cc->GetGC())EventInfo();
		
		m_pEventInfo->pTable=new(cc->GetGC())EventHashTable(4);
#ifdef _CHAINEVENT
		m_pEventInfo->chains=new(cc->GetGC())ChainArray(2);
#else
		m_pEventInfo->chains=toplevel()->arrayClass->newArray(2);
		for(int i=0;i<2;i++)
			m_pEventInfo->chains->setUintProperty(0,nullObjectAtom);
#endif
		//m_pEventInfo->pRCObjects=new(cc->GetGC())RCHashTable(4,true);
//#if (__CORE_VERSION__>=0x02070200)

//#else
//#ifdef _CODEPOST_
//		m_pEventInfo->pSystem=((ShellToplevel*)toplevel())->m_eventsBinding;//cc->GetCodeContext()->m_eventsBinding;
//		
//#else
//		m_pEventInfo->pSystem=m_codeContext->m_eventsBinding;//cc->GetCodeContext()->m_eventsBinding;
//#endif
//#endif
		m_pEventInfo->pChain=NULL;
		m_pEventInfo->pName=NULL;
		//m_pEventInfo->
		m_pEventInfo->nChains=0;
		m_pEventInfo->pSysChains=NULL;
		bNew=true;
	}

	method=SetMethod((MethodClosure*)method,useWeekRef);
	//PushMethod(method,useWeekRef);

	//cc->throwErrorV(
	//Namespacep ns=GetNamespace();
	//Multiname name;
	//name.setName(strName);
	//name.setNamespace(ns);

	ShellToplevel*top=(ShellToplevel*)toplevel();
	//MultinameHashtable*hash=((ShellCodeContext*)cc->codeContext())->m_eventsBinding;
	EventHashTable*hash=m_pEventInfo->pTable;//this->m_pEventTable;
	//ShellCodeContext*pCnt=cc->GetCodeContext();
	Binding bind=hash->get(strName,this);//(ScriptObject*)pCnt);//getMulti(name);

	//if(((ShellToplevel*)toplevel())->MapSysEvent(strName)>=0)
	//	int v=0;
	//PushMethod((MethodClosure*)method);

	if(bind==BIND_NONE)
	{
		//PushMethod(method);
#ifdef _CHAINEVENT
		int nIndex=m_pEventInfo->chains->newObject(useCapture,method,priotity,nCreateTime)+1;
#else
		int nIndex=m_pEventInfo->NewObject(method,top)+1;
#endif
		int nCmd,nMax=XAS3_EVENT_MAX;
		nCmd=((ShellToplevel*)toplevel())->MapSysEvent(strName);
			//MapSystemEvent(strName,nMax);
		if(nCmd>=0)
		{
			if(CheckEventMask(nCmd))
			{
////#if (__CORE_VERSION__>=0x02070200)
				if(m_pEventInfo->iSystemCount==0)
					cc->AddPostEvent(this);
				m_pEventInfo->iSystemCount++;
//#else
//				if(m_pEventInfo->pSystem->put(this))
//				{
//					//ShellCore*m_pCore=cc;
//					//_TraceInt(NULL,"System event table grow",m_pEventInfo->pSystem->numQuads);
//				}
//#endif
				//((ShellCore*)core())->AddPost(this,m_nCreateTime);
				//m_bEventObject=true;
				//top->pSystemEvents->put(this);
			}
				//nIndex|=0x8000;//for ENTER_FRAME
			if(m_pEventInfo->pSysChains==NULL)
			{
				m_pEventInfo->pSysChains=(XU16*)mmfx_alloc(sizeof(XU16)*nMax);
				VMPI_memset(m_pEventInfo->pSysChains,0,sizeof(XU16)*nMax);
			}
			m_pEventInfo->pSysChains[nCmd]=(XU16)nIndex;
			//((ShellCodeContext*)cc->codeContext())->AddEvent(nCmd,this);
		}

		if(hash->put(strName,this,(Binding)((nIndex)|(nCmd<<16))))
		{
			//ShellCore*m_pCore=cc;
			//_TraceInt(NULL,"Object evant table grow",hash->numQuads);
		}
		if(bNew)
		{
#ifdef _CHAINEVENT
			m_pEventInfo->pChain=m_pEventInfo->chains->getAt(nIndex-1);
#else
			m_pEventInfo->pChain=(ArrayObject*)AvmCore::atomToScriptObject(m_pEventInfo->chains->getUintProperty(nIndex-1));
#endif
			m_pEventInfo->pName=strName;
		}
	}
	else
	{
		XU32 index=(XU32)bind;
		index=(index&0x0fff)-1;
#ifdef _CHAINEVENT
		if(index<m_pEventInfo->chains->numQuads)//.size())
		{
			/*StUTF8String s(vtable->traits->name());
			if(strcmp(s.c_str(),"BinaryItem")==0)
			{
				int v=0;
				ObjectChain*pChain=m_pEventInfo->chains->m_pChain[index];
				int nCount=0;
				while(pChain)
				{
					nCount++;
					pChain=pChain->pNext;
				}
				if(nCount>=2)
					int v=0;
			}*/
			m_pEventInfo->chains->addObject(index,useCapture,method,priotity,nCreateTime);
		}
#else
		if(index<m_pEventInfo->chains->getDenseLength())
		{
			ArrayObject*pChain=(ArrayObject*)AvmCore::atomToScriptObject(m_pEventInfo->chains->getUintProperty(index));
			if(!InChain(pChain,method))
				pChain=AddChainTail(pChain,method,NULL,top);
			//ObjectChain*pChain=AddChainTail(m_pChain[i],pObj,gc);
			m_pEventInfo->chains->setUintProperty(index,pChain->atom());
			//setAt(i,pChain);
		}
#endif
		else
			AvmAssert(false);
	}
//return kAvmThunkUndefined;//Modify this please!
}

#ifdef _CHAINEVENT
inline ObjectChain* EventDispatcherObject::GetChain(EventObject*pObject)
{
	if(!m_pEventInfo) return NULL;
	if(pObject->nEventID>0) return GetSystemEvent(pObject->nEventID);
	else
	{
		EventHashTable*hash=m_pEventInfo->pTable;//this->m_pEventTable;
		Stringp name=pObject->strType;
		Binding bind=hash->get(core()->internString(name),this);//(ScriptObject*)pCnt);//Multi(name);
		if(bind==BIND_NONE)
			return NULL;
		else
		{
            
			XU32 index=(XU32)bind;
			index = ( index & 0x0fff)-1;
			AvmAssert(index<m_pEventInfo->chains->numQuads);
			//if(index>=m_pEventInfo->chains.size()) return NULL;
			return m_pEventInfo->chains->getAt(index);//.getData()[index];
		}
			//(ObjectChain*)bind;
	}
}
#else
inline ArrayObject* EventDispatcherObject::GetChain(EventObject*pObject)
{
	if(!m_pEventInfo) return NULL;
	if(pObject->nEventID>0) return GetSystemEvent(pObject->nEventID);
	else
	{
		EventHashTable*hash=m_pEventInfo->pTable;//this->m_pEventTable;
		Binding bind=hash->get(pObject->strType,this);//(ScriptObject*)pCnt);//Multi(name);
		if(bind==BIND_NONE)
			return NULL;
		else
		{
			XU32 index=(XU32)bind;
			index=(index&0x0fff)-1;
			AvmAssert(index<m_pEventInfo->chains->getDenseLength());
			//if(index>=m_pEventInfo->chains.size()) return NULL;
			return (ArrayObject*)AvmCore::atomToScriptObject(m_pEventInfo->chains->getUintProperty(index));//getAt(index);//.getData()[index];
		}
			//(ObjectChain*)bind;
	}
}
#endif


void EventDispatcherObject::UpdateCreateTime()
{
	/*if(m_pEventInfo==NULL||
	   m_pEventInfo->chains==NULL) return;
	//EventHashTable*hash=m_pEventInfo->pTable;//this->m_pEventTable;
	//XAS3_EVENT_MAX
	int i;
	XU32 nTime=XSWFPlayer::m_pInstance->m_nRunTime;
	for(i=0;i<m_pEventInfo->chains->numQuads;i++)
	{
		ObjectChain* pChain=m_pEventInfo->chains->m_pChain[i];
		if(pChain==NULL) continue;
		pChain->nCreateTime = nTime+100000;
	}*/
}

bool EventDispatcherObject::CaptureEvent(EventObject*pObject)
{
	//StUTF8String s(pObject->strType);
	//if(strcmp(s.c_str(),"result")==0)
	//	int v=0;
	if(!this->vtable)
	{
		return false;
	}
	if(pObject->pStopObj==this)
		pObject->bStoped=true;
	if(pObject->bStoped) 
		return false;
	
	pObject->bStoped=pObject->bStopedLater;

	ShellToplevel*top=(ShellToplevel*)toplevel();

	bool bRet=false;
	//if(pObject->pTarget==NULL)
	EventDispatcherObject*parent=GetParentObject(XTRUE);
#ifdef _CHAINEVENT
	ObjectChain*pChain=GetChain(pObject);
	if(pChain)
	{
		ObjectArray list(gc(),4);
		ObjectChain*p=pChain;
		while(p)
		{
			//[Note X]
			if(p->nCreateTime<=pObject->nCreateTime&&p->bUseCapture)
				list.put(p->pObject);
			p=p->pNext;
		}
		pObject->pCurrentTarget = m_pEventObject;
		if ( m_pEventObject == NULL )
		{
			pObject->pCurrentTarget=this;
		}
		
		
		int i,iCount=list.size;
		for(i=0;i<iCount;i++)
		{
			ScriptObject*pCall=GetMethod(pObject->strType,(MethodClosure*)list.getAt(i));
			if(!pCall) continue;
			Atom atoms[]={pCall->atom(),pObject->atom()};
			{
				bRet|=AvmThunkUnbox_AvmBool32(pCall->call(1,atoms));
			}

			//[Note X]
			if(pObject->bStoped) break;
			//pChain=pChain->pNext;
		}
		for(i=0;i<iCount;i++)
			list.setAt(i,NULL);
//#if (__CORE_VERSION__>=02070200)
//#else
//		pObject->pCurrentTarget=NULL;
//#endif
	}
#else
	ArrayObject*pChain=GetChain(pObject);
	if(pChain)
	{
		int i,iCount=pChain->getDenseLength();
		for(i=0;i<iCount;i++)
		{
			Atom atom=pChain->getUintProperty(i);
			if(AvmCore::isNullOrUndefined(atom)) continue;
			ScriptObject*pCall=AvmCore::atomToScriptObject(atom);
			Atom atoms[]={pCall->atom(),pObject->atom()};
			bRet|=AvmThunkUnbox_AvmBool32(pCall->call(1,atoms));
			if(pObject->bStoped) break;
		}
//#if (__CORE_VERSION__>=02070200)
//#else
//		pObject->pCurrentTarget=NULL;
//#endif
	}
#endif
	if(parent&&!pObject->bStoped)
	{
		
		if(parent!=this)
		{
			if(pObject->bWithTarget)
				pObject->pTarget=parent;
			if(parent->CaptureEvent(pObject)) bRet=true;
		}
		//void*p=m_pObject->parent->pASObject;
	}
	//pObject->nCallStack--;

	return bRet;
}

bool EventDispatcherObject::DispatchEvent(EventObject*pObject)
{
//#ifdef _WINEMU
//	StUTF8String s(pObject->strType);
//	if(strcmp(s.c_str(),"switch_location")==0)
//		int v=0;
//#endif
	if(this->vtable==NULL)
	{
		return false;
	}
	if(pObject->pStopObj==this)
		pObject->bStoped=true;
	if(pObject->bStoped) 
		return false;

	if(pObject->uEventPhase==0&&!pObject->bBubbles)
	{
		pObject->uEventPhase=1;
		CaptureEvent(pObject);
	}

	pObject->bStoped=pObject->bStopedLater;



	ShellToplevel*top=(ShellToplevel*)toplevel();

	bool bRet=false;
	ShellCore* m_pCore=(ShellCore*)core();
	//if(pObject->pTarget==NULL)
	EventDispatcherObject*parent=GetParentObject(XTRUE);
#ifdef _CHAINEVENT
	ObjectChain*pChain=GetChain(pObject);
	if(pChain)
	{
		ObjectArray list(gc(),4);
		ObjectChain*p=pChain;
		pObject->uEventPhase=2;
		while(p)
		{
			//[Note X]
			if(p->nCreateTime<=pObject->nCreateTime&&(!p->bUseCapture||pObject->bBubbles))
			{
				/*switch(pObject->uEventPhase)
				{
				case 1:
					if(p->bUseCapture)
						list.put(p->pObject);
					break;
				default:
				case 2:
				case 3:
					if(!p->bUseCapture)*/
				list.put(p->pObject);
				//	break;
				//}
			}
			p=p->pNext;
		}
		pObject->pCurrentTarget = m_pEventObject;
		if ( m_pEventObject == NULL )
		{
			pObject->pCurrentTarget=this;
		}
		
		int i,iCount=list.size;
		for(i=0;i<iCount;i++)
		{
			ScriptObject*pCall=GetMethod(pObject->strType,(MethodClosure*)list.getAt(i));
			if(!pCall) continue;
			Atom atoms[]={pCall->atom(),pObject->atom()};

			//TRY(core(),kCatchAction_ReportAsError)
			{
				bRet|=AvmThunkUnbox_AvmBool32(pCall->call(1,atoms));
			}
/*			CATCH(Exception *exception)
			{
#ifdef _WINEMU
				StUTF8String ss(pObject->strType);
				_TraceOut(exception,"Dispatch Event",ss.c_str());
#endif
				//pCore->GetPlayer()->SetErrorCode(0x20000000);
			}
			END_CATCH
			END_TRY*/
		

			//[Note X]
			if(pObject->bStoped) break;
			//pChain=pChain->pNext;
		}
		for(i=0;i<iCount;i++)
			list.setAt(i,NULL);
//#if (__CORE_VERSION__>=0x02070200)
//#else
//		pObject->pCurrentTarget=NULL;
//#endif
	}
#else
	ArrayObject*pChain=GetChain(pObject);
	if(pChain)
	{
		int i,iCount=pChain->getDenseLength();
		for(i=0;i<iCount;i++)
		{
			Atom atom=pChain->getUintProperty(i);
			if(AvmCore::isNullOrUndefined(atom)) continue;
			ScriptObject*pCall=AvmCore::atomToScriptObject(atom);
			Atom atoms[]={pCall->atom(),pObject->atom()};
			bRet|=AvmThunkUnbox_AvmBool32(pCall->call(1,atoms));
			if(pObject->bStoped) break;
		}
		pObject->pCurrentTarget=NULL;
	}
#endif

//#if (__CORE_VERSION__>=0x02075000)
	if(!pObject->bStoped)
		pObject->bStoped=pObject->bStopedLater;
//#endif

	if(parent&&pObject->bBubbles&&!pObject->bStoped)
	{
		
		if(parent!=this)
		{
			if(pObject->bWithTarget)
				pObject->pTarget=parent;
			if(parent->DispatchEvent(pObject)) bRet=true;
		}
		//void*p=m_pObject->parent->pASObject;
	}
	//pObject->nCallStack--;

	return bRet;
}

bool EventDispatcherObject::AS3_dispatchEvent(EventObject*pObject)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	/*if(m_pEventInfo==NULL) return false;
	enum {
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject
        , argoff2 = argoff1 + AvmThunkArgSize_AvmObject
    };*/
	//EventObject* pObject=(EventObject*)AvmThunkUnbox_AvmObject(argv[argoff1]);
	if(!pObject->strType) return false;

//#if (__CORE_VERSION__>=0x02078000)
	pObject->bStoped=false;
//#endif
/*#ifdef _WINEMU
	StUTF8String s(pObject->strType);
	//XSWFPlayer::m_pInstance->Trace(s.c_str());
	if(strcmp(s.c_str(),"changeChannel")==0)
	{
		bool b = true;
	}
	//	int v=0;	
#endif*/
	ScriptObject*obj=pObject->pTarget;
	ShellToplevel*top=(ShellToplevel*)toplevel();
	ShellCore*c=(ShellCore*)core();
	//[Note X] 
	bool bLater=c->GetPlayer()->m_bEnterFrame;
//#if (__CORE_VERSION__>=0x02077000)
	if(c->prevCheckStack(1))
	{
		bLater=true;
	}
//#endif
				 //(c->GetPlayer()->m_pDomFilter&&
				 //c->GetPlayer()->m_pDomFilter->bDispatchLater);



	//StUTF8String s(pObject->strType);
	//if(!bLater&&strstr(s.c_str(),"childAdd"))//creationComplete"))
	//	int v=0;
		//bLater=true;
	/*switch(s.c_str()[0])
	{
	case 'c':	
		switch(s.c_str()[1])
		{
		case 'h':
			{
				StUTF16String ss(pObject->strType);
				XSWFPlayer::m_pInstance->Trace(ss.c_str());
			}
			bLater=true;
			break;
		}
		break;
	}*/
		//int v=0;
	if(!bLater)//!bLater)//||!top->getEventDispatcherClass()->m_nInEvent)
	{
		ScriptObject*pTarget=pObject->pTarget;
		if(m_pEventObject!=NULL)
			pObject->pTarget=m_pEventObject;
		else
			pObject->pTarget=this;


		//return ((ShellCore*)core())->GetPlayer()->OnAS3Event(this,pObject)!=0;//AS3DoCommand(
		TRY(core(),kCatchAction_ReportAsError)
		{
				bool ret= DispatchEvent(pObject);
				pObject->pTarget=pTarget;
				return true;
		}
		CATCH(Exception *exception)
		{
#ifdef _WINEMU
				ShellCore*m_pCore=(ShellCore*)core();
				StUTF8String strType(pObject->strType);
				_TraceOut(exception,"Dispatch Event",strType.c_str());
#endif
				return false;
				//AvmAssert(false);
		}
		END_CATCH
		END_TRY
		
		return true;
	}
	else
	{
	//StUTF8String ss(pObject->strType);
	//if(strcmp(ss.c_str(),"__itemComplete")==0)
	//	int v=0;

		ScriptObject*pTarget=m_pEventObject;
		if(pTarget==NULL)
			pTarget=this;
		
		((ShellCore*)core())->GetPlayer()->OnAS3Event(this,pObject,pTarget);//AS3DoCommand(*/

		return true;
	}
	/*EventObject*pNew=pObject->Clone(NULL);
	if(m_pEventObject!=NULL)
		pNew->pTarget=m_pEventObject;
	else
		pNew->pTarget=this;
	if(pNew->vtable->traits->name()!=pObject->vtable->traits->name())
		int v=0;
	return ((ShellCore*)core())->GetPlayer()->OnAS3Event(this,pNew)!=0;//AS3DoCommand(*/
	//return bRet;//Modify this please!
}

bool EventDispatcherObject::AS3_hasEventListener(Stringp strType)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(m_pEventInfo==NULL||strType==NULL) return false;
	//Namespacep ns=GetNamespace();
	//Multiname name;
	//name.setName(strType);
	//name.setNamespace(ns);
	strType=core()->intern(strType->atom());

	//MultinameHashtable*hash=((ShellCodeContext*)core()->codeContext())->m_eventsBinding;
	EventHashTable*hash=m_pEventInfo->pTable;//this->m_pEventTable;
	//ShellCodeContext*pCnt=((ShellCore*)core())->GetCodeContext();
	Binding bind=hash->get(strType,this);//(ScriptObject*)pCnt);//Multi(name);
	if(bind==BIND_NONE)
		return false;
	return true;
	//return kAvmThunkUndefined;//Modify this please!
}

bool EventDispatcherObject::hasMouseEvent()
{
	if(m_pEventInfo==NULL||m_pEventInfo->pSysChains==NULL) return false;
	const XU8 nIndex[]=
	{		XAS3_EVENT_click,//  当单击 RadioButton 实例时调度。
			XAS3_EVENT_link,
			XAS3_EVENT_mouseLeave,
			XAS3_EVENT_focusIn,
			XAS3_EVENT_focusOut,
			XAS3_EVENT_mouseFocusChange,
			XAS3_EVENT_doubleClick,
			XAS3_EVENT_mouseDown,
			XAS3_EVENT_mouseMove,
			XAS3_EVENT_mouseOut,
			XAS3_EVENT_mouseOver,
			XAS3_EVENT_mouseUp,
			XAS3_EVENT_mouseWheel,
			XAS3_EVENT_rollOut,
			XAS3_EVENT_rollOver};
	for(int i=0;i<sizeof(nIndex);i++)
		if(m_pEventInfo->pSysChains[nIndex[i]]) return true;
	return false;
}
bool EventDispatcherObject::hasKeyEvent()
{
	if(m_pEventInfo==NULL||m_pEventInfo->pSysChains==NULL) return false;
	const XU8 nIndex[]=
	{		XAS3_EVENT_keyFocusChange,
			XAS3_EVENT_keyDown,
			XAS3_EVENT_keyUp};
	for(int i=0;i<sizeof(nIndex);i++)
		if(m_pEventInfo->pSysChains[nIndex[i]]) return true;
	return false;
}
bool EventDispatcherObject::hasAnyEvent()
{
	if(m_pEventInfo==NULL||m_pEventInfo->pSysChains==NULL) return false;
	return true;
}

bool EventDispatcherObject::hasEvent(int type,bool bParent)
{
	if(m_pEventInfo==XNULL||m_pEventInfo->pSysChains== XNULL||
		m_pEventInfo->pSysChains[type]==XNULL)
	{
		if(bParent&&m_codeContext)
		{
			EventDispatcherObject*parent=this->GetParentObject(true);
			if(parent) 
				return parent->hasEvent(type,bParent);
		}
		return false;
	}
	AvmAssert(type<XAS3_EVENT_MAX);
	return true;
	//return m_pEventInfo->pSysChains[type]!=NULL;
}

void EventDispatcherObject::AS3_removeEventListener(Stringp strType,ScriptObject*pFunction,bool)//AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	if(!m_pEventInfo||!strType) return;
/*#ifdef _WINEMU
	StUTF8String s(strType);
	if(strcmp(s.c_str(),"taskChange")==0)
		int v=0;
#endif*/

	strType=core()->intern(strType->atom());
	//Namespacep ns=GetNamespace();
	//name.setNamespace(ns);

	//MultinameHashtable*hash=((ShellCodeContext*)core()->codeContext())->m_eventsBinding;
	EventHashTable* hash=m_pEventInfo->pTable;//this->m_pEventTable;
	//Binding tt=hash->get(strType,this);
	//ShellCodeContext*pCnt=((ShellCore*)core())->GetCodeContext();
	Binding bind=hash->get(strType,this);//(ScriptObject*)pCnt);//Multi(name);
	ShellToplevel*top=(ShellToplevel*)toplevel();
	
	if(bind!=BIND_NONE)
	{

		XU32 index=(XU32)bind;
		index=(index&0x0fff)-1;
#ifdef _CHAINEVENT
		AvmAssert(index<m_pEventInfo->chains->numQuads);
#else
		AvmAssert(index<m_pEventInfo->chains->getDenseLength());
#endif
		//if(index<m_pEventInfo->chains.size())
		{
#ifdef _CHAINEVENT
			BOOL bSet=m_pEventInfo->chains->getAt(index)==m_pEventInfo->pChain;
			ObjectChain*pChain=m_pEventInfo->chains->removeObject(index,pFunction);
			if(pChain==NULL)
			{
				pFunction=SetMethod((MethodClosure*)pFunction,true,false);
				if(pFunction)
					pChain=m_pEventInfo->chains->removeObject(index,pFunction);
			}
#else
			BOOL bSet=m_pEventInfo->GetChain(index)==m_pEventInfo->pChain;
			ArrayObject*pChain=m_pEventInfo->RemoveObject(index,pFunction);
#endif
			//ObjectChain**pChains=(ObjectChain**)m_pEventInfo->chains.getData();
			//ObjectChain*pChain=pChains[

			//RemoveChain(&pChains[index],pFunction,m_pEventInfo->pRCObjects);//top->GetRCHashTable());
			//if(pChain==NULL)//pChains[index]==NULL)
			if(m_pEventInfo->chains->m_pChain[index]==NULL)
			{
				if(m_pEventInfo->pSysChains)
				{
					index=((int)bind)>>16;
					m_pEventInfo->pSysChains[index]=0;
				}

				if(bSet)
				{
					m_pEventInfo->pChain=NULL;
					m_pEventInfo->pName=NULL;
					//hash->remove(strType,this);
					//if(m_pEventInfo->chains.size()==2)
					//	FreeEventInfo();
				}
				XU16 nCmd=((int)bind)>>16;
				if(CheckEventMask(nCmd))
				//{
				//	if(m_bEventObject)
				//		((ShellCore*)core())->RemovePost(this);
				//	m_bEventObject=false;
				//}
//#if (__CORE_VERSION__>=0x02070200)
				if(m_pEventInfo->iSystemCount>0)//&&pFunction)
				{
					//if(((ShellCore*)core())->RemovePostEvent(this,pFunction))
					m_pEventInfo->iSystemCount--;
					if(m_pEventInfo->iSystemCount<=0)
						((ShellCore*)core())->RemovePostEvent(this);
				}
//#else
//				m_pEventInfo->pSystem->remove(this,true);
//#endif
					//top->pSystemEvents->remove(this);
				hash->remove(strType,this);
			}
			/*else if(((int)bind>>16)==XAS3_EVENT_enterFrame)
			{
				int v=0;
			}*/
		}
		//RemoveMethod((MethodClosure*)pFunction);
		return;
	}
	return;//Modify this please!
}

bool EventDispatcherObject::AS3_willTrigger(Stringp str)//AvmMethodEnv env, uint32_t argc, AvmBox* argv)
{
	//Add your act code here...
	if(!str) return false;
	return AS3_hasEventListener(str);//env,argc,argv);
	//return kAvmThunkUndefined;//Modify this please!
}

bool EventDispatcherObject::OnTimerEvent(int nType,ScriptObject*pTarget)
{
	if(!hasEvent(nType)) return false;
	//TimerEventObject*pEvent=(TimerEventObject*)player->m_pAVM2->
	//	CreateEventObject(XAS3_EVENT_timer,XAS3EVENT_TIMER,false,0,(int*)this);
	TimerEventObject*pEvent=((ShellToplevel*)toplevel())->getTimerEventClass()->CreateEventObject(nType);
	pEvent->pTarget=pTarget?pTarget:this;
	return DispatchEvent(pEvent);
}


bool EventDispatcherObject::OnClose(const char*msg)
{
	if(msg)
		return OnErrorEvent(msg);
	else
		return OnEvent(XAS3_EVENT_close,false);
}

bool EventDispatcherObject::OnEvent(Stringp strType,bool bLater)
{
	//if(!hasEvent(nType,bBubbles)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getEventClass()->CreateEventObject(strType);
	
	pEvent->bBubbles=false;

//#if (__CORE_VERSION__>=0x02077000)
	ShellCore*c=(ShellCore*)core();
	if(c->prevCheckStack(1))
	{
		bLater=true;
	}
//#endif

	if(bLater)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		//if(!ShellCore::IsTypeClass(pTarget?pTarget:this,top->getDisplayObjectClass()))
		//	int v=0;
		return ((ShellCore*)core())->GetPlayer()->OnAS3Event(this,pEvent,this);
	}
	else
	{
		pEvent->pTarget=this;
		return DispatchEvent(pEvent);
	}
}

bool EventDispatcherObject::OnEvent(int nType,bool bBubbles,ScriptObject*pTarget,bool bLater)
{
	if(!hasEvent(nType,bBubbles)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getEventClass()->CreateEventObject(nType);
	
	pEvent->bBubbles=bBubbles;
	if(bLater)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		//if(!ShellCore::IsTypeClass(pTarget?pTarget:this,top->getDisplayObjectClass()))
		//	int v=0;
		return ((ShellCore*)core())->GetPlayer()->OnAS3Event(this,pEvent,pTarget?pTarget:this);
	}
	else
	{
		pEvent->pTarget=pTarget?pTarget:this;
		return DispatchEvent(pEvent);
	}
}

bool EventDispatcherObject::OnMouseEvent(int nType,MOUSEEVENTDATA&data)
{
	if(!hasEvent(nType,true))
        return false;
	//data.bWithTarget=false;
	MouseEventObject*pEvent=((ShellToplevel*)toplevel())->getMouseEventClass()->CreateEventObject(nType,&data);//()->CreateEventObject(nType);
	pEvent->pTarget=(ScriptObject*)data.pTarget;
	if(data.bWithTarget)
		pEvent->pTarget=this;
	pEvent->pStopObj=(ScriptObject*)data.pStop;
//#if (__CORE_VERSION__>=0x02070200)
	if(data.pStop==(void*)2)
	{
		pEvent->bBubbles=false;
		data.pStop=NULL;
		pEvent->uEventPhase=1;
	}
	else
//#endif
	pEvent->bBubbles=true;
	bool bOK=DispatchEvent(pEvent);
	pEvent->pStopObj=NULL;
	return bOK;
}
bool EventDispatcherObject::OnFocusEvent(int nType,int nKey,bool bShift,ScriptObject*pout)
{
	if(!hasEvent(nType,true)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getFocusEventClass()->CreateEventObject(nType,(InteractiveObjectObject*)pout,bShift,nKey);
	pEvent->pTarget=this;
	pEvent->bBubbles=true;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnKeyEvent(int nType,KEYEVENTDATA&data)
{
	if(!hasEvent(nType)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getKeyboardEventClass()->CreateEventObject(nType,data);
	pEvent->pTarget=(ScriptObject*)data.pTarget;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnTextEvent(int nType,XU16*s16,XU8*s8)
{
	if(!hasEvent(nType,true)) return false;
	AvmCore*c=core();
	Stringp s=s16?c->newStringUTF16(s16):c->newStringUTF8((char*)s8);
	EventObject*pEvent=((ShellToplevel*)toplevel())->getTextEventClass()->CreateEventObject(nType,s);
	pEvent->bBubbles=true;
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnSyncEvent(ArrayObject*objs)
{
	if(!hasEvent(XAS3_EVENT_sync)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getSyncEventClass()->CreateEventObject(XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnStatusEvent(Stringp strCode,int level)
{
	if(!hasEvent(XAS3_EVENT_status)) return false;
	AvmCore*c=core();
	//Stringp strCode=c->newStringUTF8(code);

	Stringp strLevel=NULL;
	switch(level)
	{
	case StatusEventObject::LV_ERROR:
		strLevel=c->newStringLatin1("error");
		break;
	case StatusEventObject::LV_WARING:
		strLevel=c->newStringLatin1("warning");
		break;
	default:
		strLevel=c->newStringLatin1("staus");
		break;
	}
	EventObject*pEvent=((ShellToplevel*)toplevel())->getStatusEventClass()->CreateEventObject(XAS3_EVENT_status,strCode,strLevel);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnSecurityErrorEvent(const char*txt)
{
	if(!hasEvent(XAS3_EVENT_sync)) return false;
	Stringp s=core()->newStringUTF8(txt);
	EventObject*pEvent=((ShellToplevel*)toplevel())->getSecurityErrorEventClass()->CreateEventObject(XAS3_EVENT_securityError,s);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnProgressEvent(int nType,int loaded,int total)
{
	if(!hasEvent(nType)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getProgressEventClass()->CreateEventObject(nType,loaded,total);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnNetStatusEvent(ScriptObject*obj)
{
	if(!hasEvent(XAS3_EVENT_netStatus)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getNetStatusEventClass()->CreateEventObject(XAS3_EVENT_netStatus,obj);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnIOErrorEvent(const char*txt)
{
	if(!hasEvent(XAS3_EVENT_ioError)) return false;
	Stringp s=core()->newStringLatin1(txt);
	EventObject*pEvent=((ShellToplevel*)toplevel())->getIOErrorEventClass()->CreateEventObject(XAS3_EVENT_ioError,s);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnIMEEvent(const char*txt)
{
	if(!hasEvent(XAS3_EVENT_imeComposition)) return false;
	Stringp s=core()->newStringLatin1(txt);
	EventObject*pEvent=((ShellToplevel*)toplevel())->getIMEEventClass()->CreateEventObject(XAS3_EVENT_imeComposition,s);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnHTTPStatusEvent(int nCode)
{
	if(!hasEvent(XAS3_EVENT_httpStatus)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getHTTPStatusEventClass()->CreateEventObject(XAS3_EVENT_httpStatus,nCode);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnFullScreenEvent(bool b, bool bb)
{
	if(!hasEvent(XAS3_EVENT_fullscreen)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getFullScreenEventClass()->CreateEventObject(XAS3_EVENT_fullscreen,b,bb);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnErrorEvent(const char*txt)
{
	if(!hasEvent(XAS3_EVENT_error)) return false;
//#if (__CORE_VERSION__>=0x02070100)
	Stringp s=core()->newStringUTF8(txt);
//#else
//	Stringp s=core()->newStringLatin1(txt);
//#endif
	EventObject*pEvent=((ShellToplevel*)toplevel())->getErrorEventClass()->CreateEventObject(XAS3_EVENT_error,s);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnDataEvent(int nType,const char*data)
{
	if(!hasEvent(nType)) return false;
//#if (__CORE_VERSION__>=0x02070100)
	Stringp s=core()->newStringUTF8(data);
//#else
//	Stringp s=core()->newStringLatin1(data);
//#endif
	EventObject*pEvent=((ShellToplevel*)toplevel())->getDataEventClass()->CreateEventObject(nType,s);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnContextMenuEvent(int nType,InteractiveObjectObject*owner,InteractiveObjectObject*mouseTarget)
{
	if(!hasEvent(nType)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getContextMenuEventClass()->CreateEventObject(nType,mouseTarget,owner);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnAsyncErrorEvent(const char*txt,ErrorObject*obj)
{
	if(!hasEvent(XAS3_EVENT_asyncError)) return false;
	Stringp s=core()->newStringLatin1(txt);
	EventObject*pEvent=((ShellToplevel*)toplevel())->getAsyncErrorEventClass()->CreateEventObject(XAS3_EVENT_asyncError,s,obj);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}
bool EventDispatcherObject::OnActivityEvent(bool b)
{
	if(!hasEvent(XAS3_EVENT_activity)) return false;
	EventObject*pEvent=((ShellToplevel*)toplevel())->getActivityEventClass()->CreateEventObject(XAS3_EVENT_activity,b);//XAS3_EVENT_sync,objs);
	pEvent->pTarget=this;
	//pEvent->bBubbles=bBubbles;
	return DispatchEvent(pEvent);
}



bool EventDispatcherObject::OnConnected()
{
	return OnEvent(XAS3_EVENT_connect,false,this);
}
bool EventDispatcherObject::OnRecieve(int nSize)
{
	return OnProgressEvent(XAS3_EVENT_socketData,nSize,0);
}

MethodClosure* EventDispatcherObject::SetMethod(MethodClosure*method,bool bWeakRef,bool bCreate)
{
	//return method;

	if(!bWeakRef) return method;
	if(!ShellCore::IsTypeClass(method,toplevel()->methodClosureClass))
		return method;
	//EventDispatcherClass*pClass=((ShellToplevel*)toplevel())->getEventDispatcherClass();
	MethodEnv*env=method->peek_call();
	WeakKeyHashtable* mcTable =env->getMethodClosureTableRef();
	Atom obj=method->peek_savedThis();
	Atom mcWeakAtom = mcTable->get(obj);
	GCWeakRef* ref = (GCWeakRef*)AvmCore::atomToGenericObject(mcWeakAtom);
	union {
			GCObject* mc_o;
			MethodClosure* mc;
	};	
	if (!ref || !ref->get())
	{
		if(!bCreate) return NULL;
		VTable* ivtable = toplevel()->methodClosureClass->ivtable();//method->vtable;
		//mc = (new (core()->GetGC(), ivtable->getExtraSize()) MethodClosure(ivtable, m, obj));
		mc=new (core()->GetGC(),ivtable->getExtraSize())WeakMethodClosure(method,ivtable);
		// since MC inherits from CC, we must explicitly set the prototype and delegate since the
		// ctor will leave those null (and without delegate set, apply() and friends won't be found
		// in pure ES3 code)
		mc->prototype = method->prototype;
		mc->setDelegate(method->prototype);
		mcWeakAtom = AvmCore::genericObjectToAtom(mc->GetWeakRef());
		mcTable->add(obj, mcWeakAtom);
	}
	else
	{
		mc_o = ref->get();
	}
	return mc;

}


MethodClosure* EventDispatcherObject::GetMethod(Stringp strType,MethodClosure*obj)
{
	//FunctionObject
	if(!ShellCore::IsTypeClass(obj,toplevel()->methodClosureClass))
		return obj;
	ScriptObject*thisObj=obj->GetThisObject();
	if(thisObj==NULL)
	{
		this->AS3_removeEventListener(strType,obj,false);
		return NULL;
	}
	return obj;
	//return obj;
	/*ScriptObject*method=NULL;	
	if(m_methods)
	{
		Atom atom=obj->atom();
		Atom atomValue=m_methods->get(atom);//>getAtomProperty(atom);		
		if(atomKind(atomValue)==kSpecialType)
		{
			MMgc::GCWeakRef*ref=(MMgc::GCWeakRef*)(atomValue&~7);
			if(ref==NULL)
				method=NULL;
			else
				method=(ScriptObject*)ref->get();
		}
		else
			method=AvmCore::atomToScriptObject(atomValue);
	}
	if(method==NULL)
		this->AS3_removeEventListener(strType,obj,false);
	//if(method!=obj)
	//	int v=0;
	return method;*/
}




//#if (__CORE_VERSION__>=0x02075000)
XU32 EventDispatcherObject::LoadURL(URLRequestObject*url,LoaderContextObject*pContext)
//#else
//bool EventDispatcherObject::LoadURL(URLRequestObject*url,LoaderContextObject*pContext)
//#endif
{
	ShellCore*c=(ShellCore*)core();
	XSWFPlayer*player=c->GetPlayer();
	StUTF8String strURL(url->m_strURL);
	//const char*str=strstr(strURL.c_str(),"core.swf");
	//if(str!=NULL)
	//	return;
	XU8Array datas;
	Stringp strType=url->GetPostData(datas,false);
	if(strType==NULL) strType=url->m_strContext;
	int nType=kHttpLoadTarget;
	if(url->m_nMethod==URLRequestMethodClass::POST)
		nType|=kHttpSendUsePost;
	else
		nType|=kHttpSendUseGet;
	//const char*context=NULL;
	//LoaderInfoObject*pEvent=m_pCodeContext->m_pLoaderInfo;
		//(LoaderInfoObject*)m_pObject->character->player->m_pLoaderInfo;
	//LOGWHERE();	

	_XLoadInfo*pInfo=player->AddLoader(strURL.c_str(),NULL,nType,NULL,this,strType?StUTF8String(strType).c_str():NULL);

	pInfo->strHeaders.Add(XString8("x-flash-version"));
	pInfo->strHeaders.Add(XString8("11,1,102,62"));

	if(m_codeContext->m_pRoot)
	{
		pInfo->strRefer=m_codeContext->m_pRoot->m_url;
		pInfo->strRefer.ToString(XTRUE);
	}
	
	if(datas.GetSize())
	{
		pInfo->postData.Append(datas);
		/*XString8 str((char*)pInfo->postData.GetData(),pInfo->postData.GetSize());
		if(str.Find("Corps.getJoinedCorps")>=0)
			int v=0;*/
	}
//#if (__CORE_VERSION__>=0x02075000)
	return pInfo->nLoadID;
//#else
//	return true;
//#endif
}

/*const int _AS3_Systemlength[]=
{
	sizeof(_AS3_SystemEvents[0]),
	sizeof(_AS3_SystemEvents[1])
};*/
/*int EventDispatcherObject::MapSystemEvent(Stringp strName,int&nMax)
{
	nMax=XAS3_EVENT_MAX;
	EventClass*pClass=(EventClass*)(toplevel()->getBuiltinExtensionClass(abcclass_flash_events_Event));
	if(strName==pClass->getSlotACTIVATE())
		return XAS3_EVENT_activate;
	else if(strName==pClass->getSlotDEACTIVATE())
		return XAS3_EVENT_deactivate;
	return -1;
	//return idd;
}*/

/*EventObject* EventDispatcherObject::CreateEventObject(int id,uint32 argc,int* args)
{
	if(m_pEventInfo==NULL||m_pEventInfo->pSysChains==NULL) return NULL;
	ShellToplevel*t=(ShellToplevel*)toplevel();
	EventClass*pClass=t->getEventClass();
	Stringp strType=NULL;
	switch(id)
	{
	case XAS3_EVENT_activate:
		 strType=pClass->getSlotACTIVATE();
		 id=XAS3_OBJEVENT_activate;
		 break;
	case XAS3_EVENT_deactivate:
		 strType=pClass->getSlotDEACTIVATE();
		 id=XAS3_OBJEVENT_deactivate;
		 break;
	default:
		 return NULL;
	}
	if(!m_pEventInfo->pSysChains[id]) return false;
	Atom atoms[]={(AvmBox)pClass,strType->atom(),false,false};
	Atom atom=pClass->construct(3,atoms);
	EventObject*pObj=(EventObject*)AvmCore::atomToScriptObject(atom);
	pObj->nEventID=id;
	return pObj;
}*/
#ifdef _ANALY
	AS3PROPITEM* EventDispatcherObject::CreateItem(Stringp name,Atom atom,int type)
	{
		AS3PROPITEM* item=new AS3PROPITEM;
		StUTF16String sn(name);
		item->name=sn.c_str();
		item->nData=atom;
		Stringp s=core()->string(atom);
		StUTF16String sv(s);
		item->value=sv.c_str();
		item->next=NULL;
		//const Atom nullObjectAtom = kObjectType|0;
        //const Atom nullStringAtom = kStringType|0;
        //const Atom nullNsAtom     = kNamespaceType|0;
        //const Atom undefinedAtom  = kSpecialType|0; // 0x03
        //const Atom trueAtom       = kBooleanType|0x08; // 0x0D
        //const Atom falseAtom      = kBooleanType|0x00; // 0x05
        //const Atom zeroIntAtom    = Atom(kIntptrType|0);
		if(type>=0)
			item->nType=type;
		else
		{
			switch(atomKind(atom))
			{
			case kObjectType:
				 item->nType=AS3PROPITEM::OBJECT;break;
			case kStringType:
				 item->nType=AS3PROPITEM::STRING;break;
			case kNamespaceType:
				 item->nType=AS3PROPITEM::NAMESPACE;break;
			default:
			case kSpecialType:
				 item->nType=AS3PROPITEM::OTHER;break;
			case kBooleanType:
				 item->nType=AS3PROPITEM::LOGIC;break;
			case kIntptrType:
			case kDoubleType:
				 item->nType=AS3PROPITEM::NUMBER;break;
			}
		}
		return item;
	}



	AS3PROPITEM* EventDispatcherObject::GetChildren()
	{
		AS3PROPITEM*root=NULL;
		AS3PROPITEM*item=root;
		TraitsBindingsp b=vtable->traits->getTraitsBindings();
		int index=0,iSet=0;
		for(;;)
		{
			index=b->next(index);
			if(index<=0) break;
			Stringp str=b->keyAt(index);
			if(str)
			{
				Binding bd=b->valueAt(index);
				if(bd==BIND_NONE) continue;
				Atom atom=0;
				switch (AvmCore::bindingKind(bd))
				{
					case BKIND_METHOD:
						 break;
					case BKIND_VAR:
					case BKIND_CONST:
						 {
						   atom=getSlotAtom(AvmCore::bindingToSlotId(bd));
						   AS3PROPITEM* newItem=CreateItem(str,atom,-1);
						   ADDITEM(newItem);
						 }
						 break;
					/*case BKIND_GET:
					case BKIND_GETSET:
						{
							int m = AvmCore::bindingToGetterId(bd);
							MethodEnv *f = vtable->methods[m];//vtable->method[m];
							Atom atomv=this->atom();
							atom = f->coerceEnter(atomv);
							AS3PROPITEM*newItem=CreateItem(str,atom,-1);
							ADDITEM(newItem);
						}break;*/

				}
			}
		}
		if(vtable->traits->getHashtableOffset()!=0)
		{
			InlineHashtable*b=getTable();
			int index=0;
			for(;;)
			{
				index=b->next(index);
				if(index<=0) break;
				Stringp str=AvmCore::atomToString(b->keyAt(index));
				if(!str) continue;

				Atom atom=b->valueAt(index);
				AS3PROPITEM*newItem=CreateItem(str,atom,-1);
				ADDITEM(newItem);
			}
		}
		if(m_pEventInfo)
		{
			EventHashTable*hash=m_pEventInfo->pTable;
			int i,iCount=hash->numQuads;

			//ObjectChain**pChains=(ObjectChain**)m_pEventInfo->chains.getData();
			for(i=0;i<iCount;i++)
			{
				//if(hash->ownerAt(i)!=this) continue;
				Stringp name=hash->nameAt(i);
				Binding b=hash->valueAt(i);
				int index=((int)b&0x0fff)-1;
				if(index>=0)//&&index<=(int)m_pEventInfo->chains.size())
				{
					//if(!pChains[index]) continue;
#ifdef _CHAINEVENT
					ObjectChain*pChain=m_pEventInfo->chains->getAt(index);
					if(!pChain) continue;
					ScriptObject*obj=pChain->pObject;
#else
					ArrayObject*pChain=m_pEventInfo->GetChain(index);
					if(!pChain) continue;
					ScriptObject*obj=NULL;
					for(int i=0;i<pChain->getDenseLength();i++)
					{
						obj=AvmCore::atomToScriptObject(pChain->getUintProperty(i));
						if(obj) break;
					}
#endif
					AS3PROPITEM*newItem=CreateItem(name,obj->atom(),AS3PROPITEM::EVENT);
					ADDITEM(newItem);
				}
			}
		}
		return root;
	}
#endif

}
