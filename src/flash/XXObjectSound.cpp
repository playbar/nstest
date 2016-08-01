// XXObjectSound.cpp: implementation of the XXObjectSound class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxflash.h"
#include "XXObjectSound.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



XXObjectSound::XXObjectSound(ScriptPlayer*pRoot):XXObject(pRoot)
{
	m_pClip=XNULL;
	m_pSound=XNULL;
	m_pChannel=XNULL;
	m_bStream=XFALSE;
}

XXObjectSound::~XXObjectSound()
{
	FREEOBJECT(m_pClip);
	StopSound();
}


XBOOL XXObjectSound::SysGetMember(int id, XSWFCONTEXT *pCnt, XXVar &var)
{
	XSound*pSound=m_pSound;
	XSoundChannel*pChannel=m_pChannel;
	ScriptThread*thread=XNULL;
	if(m_pClip&&m_pClip->m_pObject)
		thread=m_pClip->m_pObject->thread;
	if(m_bStream)
	{
#ifdef SOUND
		pSound=&thread->snd;
		pChannel=thread->sndChannel;
#endif
	}
	switch(id)
	{
	case _SYSID(duration):
		 if(pSound)
		 {
			 int nRate=pSound->Rate();
			 if(nRate)
				 var=(int)(pSound->nSamples/nRate);
			// pVar=thread->GetDuration();
		 }
	case _SYSID(id3)://XXSOUND_id3Get:
		 return XTRUE;
	case _SYSID(position)://XXSOUND_positionGet:
		 if(pChannel&&pSound)
		 {
			int nRate=pSound->Rate();
			if(nRate)
				var=(int)(pChannel->pos/nRate);
		 }
		 return XTRUE;
	case _SYSID(attachSound)://XXSOUND_attachSound:
	case _SYSID(getBytesLoaded):
	case _SYSID(getBytesTotal):
	case _SYSID(getPan):
	case _SYSID(getTransform):
	case _SYSID(getVolume):
	case _SYSID(loadSound):
	case _SYSID(setPan):
	case _SYSID(setTransform):
	case _SYSID(setVolume):
	case _SYSID(start):
	case _SYSID(stop):
		 var=this;
		 var.ToFunction(id);
		 return XTRUE;

	}
    return XXObject::SysGetMember(id,pCnt,var);;
}
XBOOL XXObjectSound::SysCallMethod(int id,XSWFCONTEXT*pCnt,XXVARLIST&list)
{
	XSound*pSound=m_pSound;
	XSoundChannel*pChannel=m_pChannel;
	ScriptThread*thread=XNULL;
	if(m_pClip&&m_pClip->m_pObject)
		thread=m_pClip->m_pObject->thread;
	if(m_bStream)
	{
#ifdef SOUND
		pSound=&thread->snd;
		pChannel=thread->sndChannel;
#endif
	}
	int nSize=list.GetSize();
	switch(id)
	{
	case _SYSID(attachSound):
		 if(nSize&&thread)
		 {
			 StopSound();
			 list[0].ToString(XFALSE);
			 SCharacter*ch=m_pRoot->ImportChar(list[0].strTxt);
#ifdef SOUND
			 if(ch)
			 {
				m_pSound=&ch->sound;
			 }
#endif
			 m_bStream=XFALSE;
		 }
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	case _SYSID(getBytesLoaded):
		 if(thread)
			pCnt->pStack->Push((int)thread->player->len);
		 else
			pCnt->pStack->PushConst(XOCT_UNDEFINE);
		//if(thread)
		//	pVar=(int)thread->player->len;
		 return XTRUE;
	case _SYSID(getBytesTotal):
		 if(thread)
			 pCnt->pStack->Push((int)thread->player->scriptLen);
		 else
			 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 break;
	case _SYSID(getPan):
	case _SYSID(getTransform):
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	case _SYSID(getVolume):
		 if(thread)
		 {
			 if(m_pChannel==XNULL)
			 {
#ifdef SOUND
				pCnt->pStack->Push(thread->splayer->theSoundMix.GetVolume()*100/0xffff);
#endif
			 }
			 else
			 {
				 int v=(m_pChannel->nVolume)&0xff;
				 if(v<0) v=100;
				 //else v=v*100/sndMaxLevel;
				 pCnt->pStack->Push(v);
			 }
		 }
		 else
			 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	case _SYSID(loadSound):
	case _SYSID(setPan):
	case _SYSID(setTransform):
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	case _SYSID(setVolume):
		 if(m_pChannel)
		 {								//sndMaxLevel
			 int v=list[0].ToInt();//*0x10000/100;
			 if(v<0) v=0;
			 else if(v>=100) v=-1;
			 //else v=v*sndMaxLevel/100;
			 if(v<0) 
			 {
				 m_pChannel->nVolume=v;
				 //m_pChannel->mixer->SetVolume(list[0].ToInt()*0xffff/100);
			 }
			 else
			 {
				 m_pChannel->nVolume=(v<<8)|v;
				 //m_pChannel->mixer->SetVolume((v<<8)|v);
			 }
		 }
		 //else
		 {
#ifdef SOUND
			if(thread&&nSize)
			  thread->splayer->theSoundMix.SetVolume(list[0].ToInt()*0xffff/100);
#endif
		 }
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	case _SYSID(start):
		 if(!m_bStream)
		 {
			if(!m_pSound) break;
			StopSound();
			XSoundChannel*channel = new XSoundChannel();
			if(channel)
			{
				m_pChannel=channel;
				channel->AddRef();
				channel->sound = m_pSound;
				channel->tag = (U32)thread;
				if(nSize)
					channel->inPoint44=m_pSound->Rate()*list[0].ToInt();
				if(nSize>=2)
					channel->loops=list[1].ToInt();
#ifdef SOUND
				thread->splayer->theSoundMix.AddSound(channel);
#endif
			}
		 }
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	case _SYSID(stop):
		 StopSound();
#ifdef SOUND
		 if(m_bStream&&thread)
			thread->StopStream();
#endif
		 pCnt->pStack->PushConst(XOCT_UNDEFINE);
		 return XTRUE;
	}
	return XXObject::SysCallMethod(id,pCnt,list);
}

void XXObjectSound::InitObject()
{
	_SYSCALL(duration);
	_SYSCALL(id3);
	_SYSCALL(position);

	_SYSCALL(attachSound);
	_SYSCALL(getBytesLoaded);
	_SYSCALL(getBytesTotal);
	_SYSCALL(getPan);
	_SYSCALL(getTransform);
	_SYSCALL(getVolume);
	_SYSCALL(loadSound);
	_SYSCALL(setPan);
	_SYSCALL(setTransform);
	_SYSCALL(setVolume);
	_SYSCALL(start);
	_SYSCALL(stop);
	/*(AddProperty(XXVar(_strDuration),XXFUNOBJ(durationGet),XNULL);
	AddProperty(XXVar(_strID3),XXFUNOBJ(id3Get),XNULL);
	AddProperty(XXVar(_strPosition),XXFUNOBJ(positionGet),XNULL);

	AddMember(XXVar(_strAttachSound),XXFUNVAR(attachSound));
	AddMember(XXVar(_strGetBytesLoaded),XXFUNVAR(getBytesLoaded));
	AddMember(XXVar(_strGetBytesTotal),XXFUNVAR(getBytesTotal));
	AddMember(XXVar(_strGetPan),XXFUNVAR(getPan));
	AddMember(XXVar(_strGetTransform),XXFUNVAR(getTransform));
	AddMember(XXVar(_strGetVolume),XXFUNVAR(getVolume));
	AddMember(XXVar(_strLoadSound),XXFUNVAR(loadSound));
	AddMember(XXVar(_strSetPan),XXFUNVAR(setPan));
	AddMember(XXVar(_strSetTransform),XXFUNVAR(setTransform));
	AddMember(XXVar(_strSetVolume),XXFUNVAR(setVolume));
	AddMember(XXVar(_strStart),XXFUNVAR(start));
	AddMember(XXVar(_strStop),XXFUNVAR(stop));*/
}

XXObject* XXObjectSound::CreateObject(XSWFCONTEXT *pCnt, XXVARLIST &list,XBOOL)
{
	//return 0;
   XXObjectSound*pObj;
   //if(list.GetSize()<=0)
   pObj=new XXObjectSound(m_pRoot);
   //else
   if(pObj)
   {

	   pObj->m_pSuperObj=GetSuperObject();
	   pObj->SetSuper(this,XFALSE);
	   pObj->Construct(pCnt,list,this);
   }

//   pObj->SetType(XOOT_OBJECT);
   else
	   pCnt->pStack->Push((pObj));
   return pObj;
}

//DEL void XXObjectSound::DoCallback(XSWFCONTEXT &cnt, XXVARLIST &list, XU16 funID)
//DEL {
//DEL //	return;
//DEL 	int nSize=list.GetSize();
//DEL 	//XXStack*pStack=cnt.pStack;
//DEL 	XXVar pVar;//=XXVar::CreateConst(XOCT_UNDEFINE);
//DEL 	XBOOL bPush=XTRUE;
//DEL 	pVar.ToConst(XOCT_UNDEFINE);
//DEL //	if(!pVar) return;
//DEL 	//XSound XSoundChannel
//DEL 	ScriptThread*thread=XNULL;
//DEL 	if(m_pClip&&m_pClip->m_pObject)
//DEL 		thread=m_pClip->m_pObject->thread;
//DEL 	XSound*pSound=m_pSound;
//DEL 	XSoundChannel*pChannel=m_pChannel;
//DEL 	if(m_bStream)
//DEL 	{
//DEL 		m_pSound=&thread->snd;
//DEL 		m_pChannel=thread->sndChannel;
//DEL 	}
//DEL 	switch(funID)
//DEL 	{
//DEL 	case XXSOUND_durationGet:
//DEL 		 if(pSound)
//DEL 		 {
//DEL 			 int nRate=pSound->Rate();
//DEL 			 if(nRate)
//DEL 				 pVar=(int)(pSound->nSamples/nRate);
//DEL 			// pVar=thread->GetDuration();
//DEL 		 }
//DEL 		 break;
//DEL 	case XXSOUND_id3Get:
//DEL 		 break;
//DEL 	case XXSOUND_positionGet:
//DEL 		 if(pChannel&&pSound)
//DEL 		 {
//DEL 			int nRate=pSound->Rate();
//DEL 			if(nRate)
//DEL 				pVar=(int)(pChannel->pos/nRate);
//DEL 		 }
//DEL 		 break;
//DEL 	case XXSOUND_attachSound:
//DEL 		 if(nSize&&thread)
//DEL 		 {
//DEL 			 StopSound();
//DEL 			 list[0].ToString(XFALSE);
//DEL 			 SCharacter*ch=m_pRoot->ImportChar(list[0].strTxt);
//DEL 			 if(ch)
//DEL 			 {
//DEL 				m_pSound=&ch->sound;
//DEL //				thread->AttachSound(this,ch);
//DEL 			 }
//DEL 			 m_bStream=XFALSE;
//DEL 		 }
//DEL 		 break;
//DEL 	case XXSOUND_getBytesLoaded:
//DEL 		 if(thread)
//DEL 			pVar=(int)thread->player->len;
//DEL 		 break;
//DEL 	case XXSOUND_getBytesTotal:
//DEL 		 if(thread)
//DEL 			 pVar=(int)thread->player->scriptLen;
//DEL 		 break;
//DEL 	case XXSOUND_getPan:
//DEL 		 break;
//DEL 	case XXSOUND_getTransform:
//DEL 		 break;
//DEL 	case XXSOUND_getVolume:
//DEL 		 if(thread)
//DEL 		 {
//DEL 			 if(m_pChannel==XNULL)
//DEL 				pVar=thread->splayer->theSoundMix.GetVolume()*100/0xffff;
//DEL 			 else
//DEL 			 {
//DEL 				 int v=m_pChannel->nVolume;
//DEL 				 if(v<0) v=100;
//DEL 				 else v=v*100/sndMaxLevel;
//DEL 				 pVar=v;
//DEL 			 }
//DEL 		 }
//DEL 		 break;
//DEL 	case XXSOUND_loadSound:
//DEL 		 if(nSize&&thread)
//DEL 		 {
//DEL 			 /*list[0].ToString();
//DEL 			 m_bStream=nSize>=2?list[1].ToLogic():XFALSE;
//DEL 			 char*path=cnt.pPlayer->GetTargetPath(thread->rootObject);
//DEL 			 if(path)
//DEL 				cnt.pPlayer->GetURL2(cnt,thread,list[0].strData,path,kHttpLoadSoundStream);
//DEL 			 FreeStr(path);*/
//DEL 		 }
//DEL 		 break;
//DEL 	case XXSOUND_setPan:
//DEL 		 bPush=XFALSE;
//DEL 		 break;
//DEL 	case XXSOUND_setTransform:
//DEL 		 bPush=XFALSE;
//DEL 		 break;
//DEL 	case XXSOUND_setVolume:
//DEL 		 bPush=XFALSE;
//DEL 		 if(m_pChannel)
//DEL 		 {								//sndMaxLevel
//DEL 			 int v=list[0].ToInt();//*0x10000/100;
//DEL 			 if(v<0) v=0;
//DEL 			 else if(v>=100) v=-1;
//DEL 			 else v=v*sndMaxLevel/100;
//DEL 			 m_pChannel->nVolume=v;
//DEL 		 }
//DEL 		 else
//DEL 		 {
//DEL 			if(thread&&nSize)
//DEL 				 thread->splayer->theSoundMix.SetVolume(list[0].ToInt()*0xffff/100);
//DEL 		 }
//DEL 		 break;
//DEL 	case XXSOUND_start:
//DEL 		 bPush=XFALSE;
//DEL 		 if(!m_bStream)
//DEL 		 {
//DEL 			if(!m_pSound) break;
//DEL 			StopSound();
//DEL 			XSoundChannel*channel = new XSoundChannel();
//DEL 			if(channel)
//DEL 			{
//DEL 				m_pChannel=channel;
//DEL 				channel->AddRef();
//DEL 				channel->sound = m_pSound;
//DEL 				channel->tag = (U32)thread;
//DEL 				if(nSize)
//DEL 					channel->inPoint44=m_pSound->Rate()*list[0].ToInt();
//DEL 				if(nSize>=2)
//DEL 					channel->loops=list[1].ToInt();
//DEL
//DEL 				thread->splayer->theSoundMix.AddSound(channel);
//DEL 			}
//DEL 		 }
//DEL 		 break;
//DEL 	case XXSOUND_stop:
//DEL 		 bPush=XFALSE;
//DEL 		 StopSound();
//DEL 		 if(m_bStream&&thread)
//DEL 			thread->StopStream();
//DEL 		 break;
//DEL 	}
//DEL //	if(pVar)
//DEL 	if(bPush)
//DEL 		cnt.pStack->Push(pVar);
//DEL }

void XXObjectSound::StopSound()
{
	if(m_pChannel)
	{
		m_pChannel->Stop();
		m_pChannel->Release();
		m_pChannel=XNULL;
	}
}

XU32 XXObjectSound::Construct(XSWFCONTEXT *pCnt, XXVARLIST &list, XXObject *)
{
	FREEOBJECT(m_pClip);
	if(list.GetSize()&&list[0].IsObject(XXOBJ_CLIP))
	 {
		   //SObject*pObj=list[0].pObject->m_pObject;
		   if(list[0].pObject->m_pObject)
				m_pClip=list[0].pObject->RefObject();
		   //list[0].ToString();
		   //pObj=new XXObjectString(m_pRoot);//,list[0].strData);//XXObjectCreator::CreateString(list[0].strData);
	 }
	 else m_pClip=
	   m_pRoot->rootObject->pObject->RefObject();
	 pCnt->pStack->Push(this);
	 return (XU32)this;
}
