#include "StdAfxflash.h"
#include "ShellCore.h"
#include "avmplus.h"
#include "sobject.h"
#include "splay.h"
#include "splayer.h"
#include "sobject.h"
using namespace avmplus;
#include "AS3MovieClip.h"
#include "AS3Scene.h"
#include "AS3FrameLabel.h"
#include "avm2.h"
//#ifdef _WINEMU
//void* _pDebugP=NULL;
//#endif


namespace avmshell{
MovieClipClass::MovieClipClass(VTable* cvtable):ClassClosure(cvtable)
{
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())MovieClipObject(cvtable, toplevel()->objectClass->prototype, 0);
	//MovieClipObject*pObj=(MovieClipObject*)prototype;
	//Add your construct code here...
	createVanillaPrototype();
	//this->prototype->setStringProperty(core()->kconstructor,this->atom());
};
ScriptObject* MovieClipClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	//MovieClipObject*pObj=(MovieClipObject*)prototype;
	return new (core()->GetGC(), ivtable->getExtraSize()) MovieClipObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
MovieClipObject::MovieClipObject(VTable *vtable, ScriptObject* proto, int capacity): SpriteObject(vtable, proto, 0)
{
	//pFrameMethods=NULL;
	//Add your construct code here...
	m_pFrameInfo=NULL;
	m_bEnter=false;
	//StUTF8String s(vtable->traits->name());
	//StUTF8String s1(vtable->traits->ns()->getURI());
	//if(strcmp(s.c_str(),"Timeline_4")==0)
	//	int v=0;


	ArrayClass*pClass=toplevel()->arrayClass;

	//m_pFrames=pClass->newArray(0);
	m_pScenes=pClass->newArray(0);

}
void MovieClipObject::Release()
{
	//DRC

	if(m_pFrameInfo)
	{
		/*int i,nSize=m_pFrameInfo->chains.size();
		ObjectChain**pInfo=(ObjectChain**)m_pFrameInfo->chains.getData();
		for(i=0;i<nSize;i++)
			FreeChain(&pInfo[i]);
		//delete m_pFrameInfo->pChains;
		//m_pFrameInfo->pChains=NULL;
		//mmfx_free(m_pFrameInfo);
		delete m_pFrameInfo;
		m_pFrameInfo=NULL;*/
		//mmfx_free(m_pFrameInfo);
		//delete m_pFrameInfo;
		//m_pFrameInfo=NULL;
		/*int i,nSize=m_pFrameInfo->size();
		ObjectChain**pInfo=(ObjectChain**)m_pFrameInfo->getData();
		ShellToplevel*top=(ShellToplevel*)toplevel();
		for(i=0;i<nSize;i++)
			FreeChain(&pInfo[i]);//,top->GetRCHashTable());
		delete m_pFrameInfo;
		m_pFrameInfo=NULL;*/
		m_pFrameInfo->clear();
		delete m_pFrameInfo;
		m_pFrameInfo=NULL;
	}
	SpriteObject::Release();
	//if(m_pChain)
	//{
	//	FreeChain(&m_pChain);
		/*int nSize=m_pFrameInfo->size();
		FrameInfo*pInfo=(FrameInfo*)m_pFrameInfo->getData();
		for(int i=1;i<nSize;i++)
		{
			FreeChain(&pInfo[i].pChain);
			//if(pInfo[i].)
			//	pInfo[i].methods=NULL;
			//->DecrementRef();
		}*/
		//FreeChain(m_pFrameInfo
		//delete m_pFrameInfo;
		//m_pFrameInfo=NULL;
			//(core()->GetGC())m_pFrameInfo;
		//m_pFrameInfo=NULL;
		//mmfx_free(m_pFrameInfo);
	//}
}
//////////////////////////////////////////////////////////
//Native Method start...
int MovieClipObject::AS3_currentFrame_get()
{
	//Add your act code here...
	if(m_pObject&&m_pObject->thread)
	{
		//if(m_pObject->character->tag==65534)
		//	int v=0;
		return m_pObject->thread->curFrame+1;
	}
	else
		return 0;//Modify this please!
}

Stringp MovieClipObject::AS3_currentFrameLabel_get()
{
	//if(m_pObject&&m_pObject->character&&m_pObject->character->tag==90)
	//	int v=0;
	if(m_pObject&&m_pObject->thread)
	{
		m_pObject->thread->doLabelCommand();
		if(m_pObject->thread->currentLabel)
		{
			if(m_pObject->thread->curFrame==m_pObject->thread->labelFrame+1)
				return core()->internStringUTF8(m_pObject->thread->currentLabel);
			else
				return NULL;
		}
		else
			return NULL;
	}
	return NULL;//core()->;
	//return AS3_currentLabel_get();
}

Stringp MovieClipObject::AS3_currentLabel_get()
{
	//Add your act code here...
	if(m_pObject&&m_pObject->thread)
	{
		/*m_pObject->thread->AS3BuildSecnesAndFrames(this);
		int iBest=0;
		SceneObject*p=this->GetCurrentScene(iBest);
		if(p&&iBest)
		{
			int i,iCount=p->m_pLabels->get_length(),iCur=m_pObject->thread->curFrame;
			Stringp strLabel=NULL;
			for(i=iCount-1;i>=0;i++)
			{
				FrameLabelObject*obj=(FrameLabelObject*)AvmCore::atomToScriptObject(p->m_pLabels->getIntProperty(i));
				if(obj->m_nFrame<=iCur)
				{
					strLabel=obj->m_strName;
					break;
				}
				//if(obj->m_nFrame<=iCur&&obj->m_nFrame)
				//	strLabel=obj->m_strName;
			}
			if(strLabel) return strLabel;
		}*/

//#if (__CORE_VERSION__>=0x02075000)		
		m_pObject->thread->doLabelCommand();
//#endif
		if(m_pObject->thread->currentLabel)
			return core()->internStringUTF8(m_pObject->thread->currentLabel);
		else
			return NULL;//core()->nullStringAtom;
	}
	else
		return NULL;//core()->nullStringAtom;//kAvmThunkUndefined;//Modify this please!
}


SceneObject* MovieClipObject::GetCurrentScene(int&iBest)
{
	int nSize=m_pScenes->getDenseLength();//get_length();
	AvmCore* c=core();
	if(m_pObject)
	{
		AvmAssert(m_pObject->thread!=NULL);
		int f=m_pObject->thread->curFrame;
		int t=m_pObject->thread->numFrames;
		SceneObject*pBest=NULL;
		int nBest=f;
		for(int i=0;i<nSize;i++)
		{
			SceneObject*pObj=(SceneObject*)AvmCore::atomToScriptObject(m_pScenes->getIntProperty(i));
			if(pBest==NULL) {pBest=pObj;iBest=i;}
			else if(pObj->m_pLabels->getDenseLength()>0||pBest->m_pLabels->getDenseLength()==0)
				//if(pObj->m_pLabels->get_length()>0||pBest->m_pLabels->get_length()==0)
			{
				int fl=f-pObj->m_nOffset;
				if(fl>=0&&fl<=nBest&&fl<pObj->m_nFrames)
				{
					//if(f<=nBest)
					nBest=fl;
					iBest=i;
					pBest=pObj;
				}
			}
		}
		return pBest;
	}
	//	return (SceneObject*)(nSize?core()->atomToScriptObject(m_pScenes->getIntProperty(0)):NULL);
	return (SceneObject*)core()->atomToScriptObject(m_pScenes->getIntProperty(0));
}

SceneObject* MovieClipObject::AS3_currentScene_get()
{
	//Add your act code here...
	int iBest;
	if(m_pObject)
		 m_pObject->thread->AS3BuildSecnesAndFrames(this);
	return GetCurrentScene(iBest);
	//return kAvmThunkUndefined;//Modify this please!
}

bool MovieClipObject::AS3_enabled_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsEnable();
	return false;
}

void MovieClipObject::AS3_enabled_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
	{
		m_pObject->SetEnable(b);
	}
}

int MovieClipObject::AS3_framesLoaded_get()
{
	//Add your act code here...
	if(m_pObject&&m_pObject->thread)
		//return m_pObject->thread->
	{
		ScriptThread*pThread=m_pObject->thread;
		if (pThread == pThread->player) {
                    // this is the main movie
					if (pThread->player->numFramesComplete < pThread->numFrames) {
						return pThread->player->numFramesComplete;
					} else {
						return pThread->numFrames;
					}
				} else {
                    // movie sprites are alwayes fully loaded
                    // before they start playing
                    return pThread->numFrames;
				}
	}
	return 0;
}


int MovieClipObject::AS3_totalFrames_get()
{
	//Add your act code here...
	if(m_pObject&&m_pObject->thread)
		return m_pObject->thread->numFrames;
	return 0;
}

bool MovieClipObject::AS3_trackAsMenu_get()
{
	//Add your act code here...
	if(m_pObject)
		return m_pObject->IsTrackAsMenu();
	return false;
}

void MovieClipObject::AS3_trackAsMenu_set(bool b)
{
	//Add your act code here...
	if(m_pObject)
		m_pObject->SetTrackAsMenu(b);
}

//AS3 contructor function..
void MovieClipObject::AS3_constructor()
{
	//Add your act code here...
	//ScriptObject::construct(
	//SpriteObject::AS3_constructor();
	//StUTF8String s(vtable->traits->name());
	//StUTF8String s1(vtable->traits->ns()->getURI());
	//if(strcmp(s.c_str(),"Timeline_4")==0)
	//	int v=0;

	if(m_pObject==NULL)
	{
		DisplayObjectObject::CreateNewObject(spriteEmptyChar);//CreateNewObject(
		if(m_pObject->thread->numFrames<0)
			m_pObject->thread->playing=false;
		//AS3_stop();
	}

	//
	//ScenseObject*pObj=top->getSceneClass()->CreateScene(core()->newConstantStringLatin1("Scense 1");
	//ShellToplevel*top=(ShellToplevel*)
	//ScenseObject*p=
	if(m_pObject)
	{
		ShellToplevel*top=(ShellToplevel*)toplevel();
		SceneObject*pObj=top->getSceneClass()->CreateScene(core()->newConstantStringLatin1("Scene 1"),m_pObject->thread->numFrames,0);
		this->m_pScenes->setIntProperty(0,pObj->atom());
		m_nTag=m_pObject->character->tag;
		this->FillMembers();

		//m_pObject->AS3FillMembers(pRet,this);
		//AvmAssert(m_pObject->thread!=NULL);
		//m_pObject->thread->AS3BuildSecnesAndFrames(this);
	}

}

inline int MovieClipObject::FindFrame(Stringp frame,SceneObject*pScene)
{
	ArrayObject*pArray=pScene->m_pLabels;//:m_pFrames;
	int i,nSize=pArray->getDenseLength();//get_length();
	AvmCore*c=core();
	for(i=0;i<nSize;i++)
	{
		FrameLabelObject* f=(FrameLabelObject*)c->atomToScriptObject(pArray->getIntProperty(i));
		if(f->AS3_name_get()->equals(frame))
			return f->AS3_frame_get()-1;

	}
	return -1;
}


inline int MovieClipObject::FindFrame(int iFrame,SceneObject*pScene)
{
	ArrayObject*pArray=pScene->m_pLabels;//:m_pFrames;
	int i,nSize=pArray->getDenseLength();//get_length();
	AvmCore*c=core();
	for(i=0;i<nSize;i++)
	{
		FrameLabelObject* f=(FrameLabelObject*)c->atomToScriptObject(pArray->getIntProperty(i));
//#if (__CORE_VERSION__>=0x02071000)
		if(f->m_iLabel>=0&&f->m_iLabel+1==iFrame)
			return f->AS3_frame_get()-1;
//#else
//		if(f->m_iLabel>=0&&f->m_iLabel==iFrame)
//			return f->AS3_frame_get()-1;
//#endif
	}
	return -1;
}

ArrayObject* MovieClipObject::AS3_scenes_get()
{
	if(m_pObject)
		m_pObject->thread->AS3BuildSecnesAndFrames(this);
	return m_pScenes;
}

inline SceneObject* MovieClipObject::FindScene(Stringp scene)
{

	if(!scene) return NULL;
	int nSize=m_pScenes->getDenseLength();//get_length();
	AvmCore*c=core();
	for(int i=0;i<nSize;i++)
	{
		SceneObject*pObj=(SceneObject*)c->atomToScriptObject(m_pScenes->getIntProperty(i));
		if(pObj->m_strName->equals(scene))
			return pObj;
	}
	return NULL;
}

int MovieClipObject::GetFrame(Atom frame,Stringp scense)
{
	int iFrame=-1,iBest;
	AvmCore* c=core();
	int kind=atomKind(frame);
	if(kind!=kIntptrType&&kind!=kDoubleType)
	{
		m_pObject->thread->AS3BuildSecnesAndFrames(this);
		SceneObject*p=scense?FindScene(scense):this->GetCurrentScene(iBest);
		Stringp name=c->string(frame);
		iFrame=FindFrame(name,p);
		if(iFrame<0)
		{
			iFrame=c->integer(frame)-1;
		}
	}
	else
	{
		iFrame=c->integer(frame);
		{
			m_pObject->thread->AS3BuildSecnesAndFrames(this);
			SceneObject*p=scense?FindScene(scense):this->GetCurrentScene(iBest);
//#if (__CORE_VERSION__>=0x02071000)
			int i=FindFrame(iFrame,p);
//#else
//			int i=FindFrame(iFrame+1,p);
//#endif
			if(i>=0) return i;
		}
		iFrame--;
		if(iFrame<0) iFrame=0;
		if(scense)
		{
			m_pObject->thread->AS3BuildSecnesAndFrames(this);
			SceneObject*p=FindScene(scense);//:this->GetCurrentScene(iBest);
			if(p)
				iFrame+=p->m_nOffset;
		}
	}
	return iFrame;
}

void MovieClipObject::AS3_gotoAndPlay(Atom frame,Stringp scense)
{
	//return;
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
	{

		AvmAssert(m_pObject->thread!=NULL);
		int iFrame=GetFrame(frame,scense);
		if(iFrame<0||iFrame>=m_pObject->thread->numFrames)
			iFrame=m_pObject->thread->curFrame;
		if(iFrame>=0)
		{
			m_pObject->thread->CMDGotoAndPlay(iFrame);//Seek(iFrame,true,ScriptPlayer::SK_PLAY);
			//m_pObject->thread->Play();
		}
		else
			m_pObject->thread->CMDPlay();//Play();
		//if(!m_pObject->AS3IsCalling())
		//[Note X]
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(!player->m_bDoFrame)
			m_pObject->thread->doCommand(true);
//#if (__CORE_VERSION__>=0x02076000)
//		else
//			m_pObject->thread->playing=true;
//#endif		
		player->ModifyEvent(XSWFPlayer::XMOVIECLIP0,(int)m_pObject,0,1);
	}
}

ArrayObject* MovieClipObject::AS3_currentLabels_get()
{
	if(m_pObject)
	{
		ScriptThread*thread=m_pObject->thread;
		thread->AS3BuildSecnesAndFrames(this);

	}
	int iBest;
	SceneObject*pObj=GetCurrentScene(iBest);
	if(pObj) return pObj->m_pLabels;
	return NULL;
}


void MovieClipObject::AS3_gotoAndStop(Atom frame,Stringp scense)
{
	//Add your act code here...
	//if(m_pObject&&m_pObject->character&&m_pObject->character->tag==90)
	//	int v=0;
#ifdef _WINEMU
/*	if(m_pObject->character&&m_pObject->character->tag==19)
	{
		int v=0;
		if(_pDebugP==NULL&&frame==14)
			_pDebugP=m_pObject;
	}*/
#endif
	if(m_pObject)
	{
		AvmAssert(m_pObject->thread!=NULL);

		//if(m_pObject->character->tag==1082)
		//	int v=0;

		int iFrame=GetFrame(frame,scense);
		if(iFrame>=m_pObject->thread->numFrames)
			iFrame=m_pObject->thread->numFrames-1;
		if(iFrame<0)//||iFrame>=m_pObject->thread->numFrames)
			iFrame=m_pObject->thread->curFrame;
		
		if(iFrame>=0)
		{
			//if(iFrame<m_pObject->thread->numFrames)
			m_pObject->thread->CMDGotoAndStop(iFrame);//Seek(iFrame,true,ScriptPlayer::SK_STOP);
			//if(iFrame==4&&m_pObject->thread->numFrames==5)
			//	int v=0;
			//m_pObject->thread->StopPlay();
		}
		else 
			m_pObject->thread->CMDStop();//StopPlay();
		//if(!m_pObject->AS3IsCalling())
		//[Note X]
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(!player->m_bDoFrame)
			m_pObject->thread->doCommand(true);
		
		player->ModifyEvent(XSWFPlayer::XMOVIECLIP0,(int)m_pObject,0,2);
		//m_pObject->thread->doCommand();
	}
	//this->AS3_play();
	//this->AS3_stop();
}

void MovieClipObject::AS3_nextFrame()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pObject)
	{
		AvmAssert(m_pObject->thread!=NULL);
		m_pObject->thread->CMDGotoAndKeep(m_pObject->thread->curFrame+1);//Seek(m_pObject->thread->curFrame+1,true,ScriptPlayer::SK_KEEP);
		//if(!m_pObject->AS3IsCalling())
		//[Note X]
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(!player->m_bDoFrame)
			m_pObject->thread->doCommand(true);
//#if (__CORE_VERSION__>=0x02081000)
//		player->Stop();
//#endif
	}
}

void MovieClipObject::AS3_nextScene()
{
	//Add your act code here...
	int iBest=0;
	if(!m_pObject) return;
	m_pObject->thread->AS3BuildSecnesAndFrames(this);
	if(GetCurrentScene(iBest))
	{
		iBest++;
		if(iBest>=(int)m_pScenes->getDenseLength())//;get_length())
		{
			m_pObject->thread->CMDGotoAndPlay(m_pObject->thread->curFrame+1);
			XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
			player->ModifyEvent(XSWFPlayer::XMOVIECLIP0,(int)m_pObject,0,1);
			return;
			//iBest=m_pScenes->get_length()-1;
		}
		SceneObject*pObj=(SceneObject*)core()->atomToScriptObject(m_pScenes->getIntProperty(iBest));
		m_pObject->thread->CMDGotoAndPlay(pObj->m_nOffset);//Seek(pObj->m_nOffset,true,ScriptPlayer::SK_KEEP);
		//if(!m_pObject->AS3IsCalling())
		//[Note X]
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(!player->m_bDoFrame)
			m_pObject->thread->doCommand(true);
		//XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		player->ModifyEvent(XSWFPlayer::XMOVIECLIP0,(int)m_pObject,0,1);
	}
}

void MovieClipObject::AS3_play()
{
	//Add your act code here...
	if(m_pObject)
	{
		m_pObject->thread->CMDPlay();//Play();
		m_pObject->thread->doCommand(true);
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		player->ModifyEvent(XSWFPlayer::XMOVIECLIP0,(int)m_pObject,0,1);
	}

}

void MovieClipObject::AS3_prevFrame()
{
	//Add your act code here...
	if(m_pObject)
	{
		AvmAssert(m_pObject->thread!=NULL);
		m_pObject->thread->CMDGotoAndKeep(m_pObject->thread->curFrame-1);//Seek(m_pObject->thread->curFrame-1,true,ScriptPlayer::SK_KEEP);
		//if(!m_pObject->AS3IsCalling())
		//[Note X]
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(!player->m_bDoFrame)
			m_pObject->thread->doCommand(true);
//#if (__CORE_VERSION__>=0x02081000)
//		player->Stop();
//#endif
	}
}

void MovieClipObject::AS3_prevScene()
{
	//Add your act code here...
	int iBest=0;
	if(GetCurrentScene(iBest))
	{
		iBest--;
		if(iBest<0)
		{
			m_pObject->thread->CMDGotoAndPlay(m_pObject->thread->curFrame-1);//Seek(m_pObject->thread->curFrame-1,true,ScriptPlayer::SK_KEEP);
			XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
			player->ModifyEvent(XSWFPlayer::XMOVIECLIP0,(int)m_pObject,0,1);
			return;
			//iBest=0;
		}
		SceneObject*pObj=(SceneObject*)core()->atomToScriptObject(m_pScenes->getIntProperty(iBest));
		m_pObject->thread->CMDGotoAndKeep(pObj->m_nOffset);//Seek(pObj->m_nOffset,true,ScriptPlayer::SK_KEEP);
		//if(!m_pObject->AS3IsCalling())
		//[Note X]
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(!player->m_bDoFrame)
			m_pObject->thread->doCommand(true);
	}
}

void MovieClipObject::AS3_stop()
{
	//Add your act code here...
	if(m_pObject&&m_pObject->thread)
	{
		m_pObject->thread->CMDStop();//StopPlay();
		//XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		//[Note X]
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		if(!player->m_bDoFrame)
			m_pObject->thread->doCommand(true);	
//#if (__CORE_VERSION__>=0x02076000)
//		else
//			m_pObject->thread->playing=false;
//#endif
		player->ModifyEvent(XSWFPlayer::XMOVIECLIP0,(int)m_pObject,0,2);
	}
}

void MovieClipObject::AS3_addFrameScript(ArrayObject*args)//uint32 iFrame,FunctionObject*atom)//AvmMethodEnv, uint32_t iCount, AvmBox* argv)
{
	//Add your act code here...

	/*{
		AvmCore*cc=core();
		int t=(int)this;
		//.const char* str=(const char*)this;
		Stringp strNamespace=cc->internStringLatin1((const char*)&t,4);//pDot?strName:"",pDot?pDot-strName:0);
		Namespacep ns = cc->internNamespace(cc->newNamespace(strNamespace,avmplus::Namespace::NS_Public, 0));
		int v=0;
		//Stringp strMultiname=internStringLatin1(pDot?pDot+1:strName);

		//Multiname name;
		//name.setName(strMultiname);


	}*/

	/*enum {
        argoff0 = 0
        , argoff1 = argoff0 + AvmThunkArgSize_AvmObject
        , argoff2 = argoff1 + AvmThunkArgSize_uint32_t
    };
	AvmAssert(iCount>=2);
	int iFrame=AvmThunkUnbox_uint32_t(argv[argoff1]);
	//Atom atom=
	ScriptObject* atom=AvmCore::atomToScriptObject(argv[argoff2]);*/
	//Atom atom=AvmThunkUnbox_AvmBox(argv[argoff2]);
	//Atom atom=pObject->atom();
	//pObject->call(0,(Atom*)&atom);
//	Atom nn=toString();
//	Stringp name=core()->atomToString(nn);
	//if(atom)//&&!InChain(m_pChain,atom))
	AvmCore*cc=core();
	ShellToplevel*top=(ShellToplevel*)toplevel();
	int nMaxCount=1,nCurFrame=-1;
	if(m_pObject)
	{
		nMaxCount=m_pObject->thread->numFrames;
		nCurFrame=m_pObject->thread->curFrame;
	}
	ScriptObject*pCall=NULL;
	XU32 nCreateTime=((ShellCore*)cc)->GetPlayer()->m_nRunTime;
	//m_pObject?m_pObject->thread->numFrames:1;

	for(uint32_t j=0;j<args->getDenseLength();j+=2)//get_length();j+=2)
	{
		//ShellCore*cc=(ShellCore*)core();
		int iFrame=cc->integer_i(args->getIntProperty(j));

		//if(iFrame>=nMaxCount) return;
		if(!m_pFrameInfo)
		{
			int nSize=args->getDenseLength()*2/3;//get_length()*2/3;
#ifdef _CHAINEVENT
			m_pFrameInfo=new (cc->GetGC())ChainHashTable(nSize);
#else
			m_pFrameInfo=new (cc->GetGC())ArrayHashTable(nSize);
#endif
			/*int i;
			m_pFrameInfo=new (cc->GetGC())FRAMEINFOS();
			for(i=0;i<nMaxCount;i++)
				m_pFrameInfo->add(NULL);*/
			//m_pFrameInfo=new (cc->GetGC())FrameInfo();
				//(FrameInfo*)mmfx_alloc(sizeof(FrameInfo));
			//m_pFrameInfo->pChains=new(cc->GetGC())FRAMEINFOS();
			//m_pFrameInfo->nFrame=0;
			//m_pFrameInfo->pChain=NULL;
		}

		//if(iFrame>=(int)m_pFrameInfo->size()) return;
		ScriptObject* atom=cc->atomToScriptObject(args->getIntProperty(j+1));
		if(!atom)
		{
#ifdef _CHAINEVENT
			//RemoveMethod(atom);
			m_pFrameInfo->remove(iFrame+1);//,atom,true);
#else
			m_pFrameInfo->remove(iFrame+1);//,atom,true);
			//m_pFrameInfo->addObject(iFrame+1,atom,true,toplevel());
#endif
				//ObjectChain**p=(ObjectChain**)m_pFrameInfo->getData();

				//if(*p) AddChainTail(&p[iFrame],atom,top->GetRCHashTable());
				//else   AddChainHead(&p[iFrame],atom,top->GetRCHashTable());
			if(iFrame==nCurFrame)
					pCall=atom;
			continue;
		}
		else if(!cc->isFunction(atom->atom()))
			toplevel()->throwTypeError(kInvalidArgumentError,core()->knull);
#ifdef _CHAINEVENT
		//PushMethod(atom,false);
		m_pFrameInfo->addObject(iFrame+1,XFALSE,atom,0,nCreateTime,true);
#else
		m_pFrameInfo->addObject(iFrame+1,atom,true,toplevel());
#endif
		//ObjectChain**p=(ObjectChain**)m_pFrameInfo->getData();

		//if(*p) AddChainTail(&p[iFrame],atom,top->GetRCHashTable());
		//else   AddChainHead(&p[iFrame],atom,top->GetRCHashTable());
		if(iFrame==nCurFrame)
			pCall=atom;
		//	m_pFrameInfo=new(core()->GetGC())FRAMEINFOS();
		/*Namespacep ns=GetNamespace();

		MultinameHashtable*hash=cc->GetEventsBinding();
		int t=iFrame+1;
		Stringp strName=cc->internStringLatin1((const char*)&t,4);
		Multiname name;
		name.setName(strName);
		name.setNamespace(ns);
		Binding bind=hash->getMulti(name);
		if(bind==BIND_NONE)
		{
			ObjectChain*pChain=NULL;
			AddChainHead(&pChain,atom);
			m_pFrameInfo->chains.add(pChain);
			hash->add(strName,ns,(Binding)pChain);
			m_pFrameInfo->nFrame=iFrame;
			m_pFrameInfo->pChain=pChain;
		}
		else
		{
			ObjectChain*pChain=(ObjectChain*)bind;
			if(!InChain(pChain,atom))
				AddChainTail(&pChain,atom);
		}
		//AddChainHead(m_pChain);
		//if(!m_pFrameInfo)
		//	m_pFrameInfo=new(core()->GetGC())FRAMEINFOS();
		//m_pFrameInfo->Inc
			////new(core->GetGC());
		//AddFrameInfo(iFrame,atom);
		/*int nCount=m_pObject?(m_pObject->thread?m_pObject->thread->numFrames:0):iFrame+1;
		if(iFrame<nCount)
		{
			if(m_pFrameInfo==NULL||m_pFrameInfo->nCount<nCount)
			{

				FrameInfo*pNew=(FrameInfo*)mmfx_alloc(_FMETHODSIZE(nCount));
				if(pNew)
				{
					VMPI_memset(pNew,0,_FMETHODSIZE(nCount));
					if(m_pFrameInfo)
					{
						VMPI_memcpy(pNew,m_pFrameInfo,_FMETHODSIZE(m_pFrameInfo->nCount));
						mmfx_free(m_pFrameInfo);
					}
					pNew->nCount=nCount;
					m_pFrameInfo=pNew;
				}
			}
			if(m_pFrameInfo&&iFrame<m_pFrameInfo->nCount)
			{
				//if(m_pFrameInfo->methods[iFrame])
				//	m_pFrameInfo->methods[iFrame]->DecrementRef();
				m_pFrameInfo->methods[iFrame]=atom;
				//atom->IncrementRef();
			}
		}*/
			//(m_pObject&&m_pObject->thread)?m_pObject->thread->
		//XSortCall calls(&m_frameMethods);
		//calls.Add(iFrame,atom);
		/*int nCount=pFrameMethods?pFrameMethods->nMethodCount:0;
		nCount++;
		FrameMethod*pNew=(FrameMethod*)mmfx_alloc(_FMETHODSIZE(nCount));
		pNew->methods[nCount-1]=0;
		pNew->cacheMethod=0;
		pNew->nCacheFrame=-1;

		if(pFrameMethods)
		{
			VMPI_memcpy(pNew,pFrameMethods,_FMETHODSIZE(nCount));
			mmfx_free(pFrameMethods);
		}
		pFrameMethods=pNew;
		pNew->nMethodCount=nCount;

		int n = 7;

		int bitmask = (nCount - 1);

		unsigned i = ((0x7FFFFFF8 & (uintptr)atom) >> 3) & bitmask;

		//Stringp k;
		while (pFrameMethods->methods[i])
		{
			i = (i + (n++)) & bitmask;			// quadratic probe
		}
		atom->IncrementRef();
		pFrameMethods->methods[i]=atom;
		return i;*/

	}
	if(pCall&&m_pObject)//&&m_pObject->thread->m_bAS3Frame0)
	{
		//Atom a=atom();
		//pCall->call(0,&a);
		m_pObject->thread->m_bAS3Frame0=true;//false;
		XSWFPlayer*player=XSWFPlayer::m_pInstance;
		if(m_pObject->thread)
		{
			player->m_pAVM2->PushObject(this);
			player->AddEvent(XSWFPlayer::XMOVIECLIP0,(int)m_pObject,0,0);
		}
	}
	//return kAvmThunkUndefined;//Modify this please!
}


bool MovieClipObject::CallFrame(int iFrame)
{

	if(!m_pFrameInfo) return false;//||iFrame>=(int)m_pFrameInfo->size()) return false;
	Atom a=atom();
#ifdef _CHAINEVENT
	ObjectChain*pChain=m_pFrameInfo->get(iFrame+1);
	if(!pChain) return false;


	while(pChain)
	{
		//Atom atom=pChain->pObject->atom();
		if(!pChain->pObject) break;
		pChain->pObject->call(0,&a);//&atom);
		pChain=pChain->pNext;
	}

#else
	ArrayObject*pChain=m_pFrameInfo->get(iFrame+1);
	if(!pChain) return false;
	int i,iCount=pChain->get_length();
	for(i=0;i<iCount;i++)
	{
		Atom atom=pChain->getUintProperty(i);
		if(AvmCore::isNullOrUndefined(atom)) continue;
		AvmCore::atomToScriptObject(atom)->call(0,&a);
	}
#endif
	return true;
}
#ifdef _ANALY
	AS3PROPITEM* MovieClipObject::GetChildren()
	{
		AS3PROPITEM* root=SpriteObject::GetChildren();
		AS3PROPITEM* item=root;
		while(item&&item->next)
			item=item->next;
		if(m_pFrameInfo)
		{
			//ObjectChain**pChain=(ObjectChain**)m_pFrameInfo->getData();
			int i,iCount=m_pFrameInfo->numQuads;//size();
			for(i=0;i<iCount;i++)
			{
				//if(!pChain[i]) continue;
#ifdef _CHAINEVENT
				ObjectChain*pChain=m_pFrameInfo->chainAt(i);
#else
				ArrayObject*pChain=m_pFrameInfo->chainAt(i);
#endif
				if(!pChain) continue;
				AS3PROPITEM*newItem=new AS3PROPITEM;
				newItem->nType=AS3PROPITEM::EVENT;
				newItem->name="Frame:";
				newItem->nData=0;
				newItem->value.FromInt(m_pFrameInfo->keyAt(i));
				newItem->next=XNULL;
				ADDITEM(newItem);
			}
		}
		return root;
	}
#endif


//new adds 11
bool MovieClipObject::AS3_isPlaying_get()
{
	//LOGWHERE();
	if(m_pObject)
		return m_pObject->thread->playing;
	return false;
}
		 
//new adds 11 end
}
