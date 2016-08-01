#include "StdAfxflash.h"
#include "sobject.h"
#include "ShellCore.h"
#include "splayer.h"

using namespace avmplus;
#include "AS3SoundChannel.h"
#include "AS3SoundTransform.h"
#include "AS3Sound.h"
#include "AS3Event.h"
#include "avm2.h"

namespace avmshell{
SoundChannelClass::SoundChannelClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SoundChannelObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* SoundChannelClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SoundChannelObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

static void _CallBack(void*pData,bool bRemove)
{
	SoundChannelObject*pSound=(SoundChannelObject*)pData;
	if(pSound==NULL) return;
//pSound->m_bEvent=true;
	XSWFPlayer*player=((ShellCore*)pSound->core())->GetPlayer();
	player->m_pAVM2->PushObject(pData);
	player->AddEvent(XSWFPlayer::XSOUNDCOMPLETE,(int)pSound,(int)(ShellCodeContext*)pSound->m_codeContext,(int)bRemove);
}
void SoundChannelObject::PlayEnd(bool bRemove)
{
//m_bEvent=false;
	if(m_pSoundObj)
	{
		m_pSoundObj->m_pLoaderInfo->bPlaying=false;
		m_pSoundObj->m_pLoaderInfo->pObject=NULL;
	}
	m_pSoundObj=NULL;
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->m_pAVM2->RemoveObject(this);
	if(bRemove)
	{
		m_pChannel=XNULL;
	}
	if(hasEvent(XAS3_EVENT_soundComplete))
	{
		this->OnEvent(XAS3_EVENT_soundComplete,false);//,ents->x,(void*)pEvents->y,pEvents->);
	}

}

static BOOL ScriptStreamProc(XSoundChannel* channel)
// WARNING: this is called at interupt time on the Mac, don't do any memory allocations...
{
	SoundChannelObject* snd = (SoundChannelObject*)(channel->refPtr);
	if(snd==NULL)
        return XFALSE;

	if(snd->m_pChannel == NULL)
	{
		channel->loops=0;
		channel->streamProc=NULL;
		return XFALSE;
		//return XFALSE;
	}
	AvmAssert(snd->m_pChannel == channel);
	return snd->DoSoundFrame();
}

bool SoundChannelObject::DoSoundFrame()
{
	if(m_pSoundObj==NULL) return false;

	{
		m_pSoundObj->StreamProc(this);
		//stop
		//m_pChannel->streamProc=NULL;
		//XSWFPlayer*player=((ShellCore*)pSound->core())->GetPlayer();
		//player->m_pAVM2->RemoveObject(this);
	}
	//return m_pSoundObj->DoSoundFrame();
	//SoundObject::LoaderData*ld=m_pSoundObj->m_pLoaderInfo;
	//if(!ld->bInitOK) return false;

	return false;

}

SoundChannelObject* SoundChannelClass::CreateSoundChannel(SoundObject*snd,SoundTransformObject*trans,int nStart,int nLoops,bool bInitOK)
{
	SoundChannelObject*pObj=(SoundChannelObject*)createInstance(ivtable(),prototype);
#ifdef SOUND
	XSoundChannel*channel = new XSoundChannel();
	if(channel&&snd->m_pLoaderInfo)
	{
		channel->Init();
		pObj->m_pChannel=channel;
		//channel->completeStop=true;
		channel->AddRef();
		channel->samplesPlayed=nStart;
		channel->sound = &snd->m_pLoaderInfo->snd;
		channel->tag = 0;
		channel->loops = nLoops;
		channel->completeProc=_CallBack;
		channel->pArgs=pObj;
		channel->refPtr = pObj;
		channel->streamProc = ScriptStreamProc;
		//if(nStart)
		//	channel->inPoint44=ch->sound.Rate()*nStart;
		if(bInitOK)
		{
			((ShellCore*)core())->GetPlayer()->theSoundMix.AddSound(channel);
			pObj->m_bAdded=true;
		}
		channel->sound->delay=0;
		if(trans)
			channel->m_pTransform=&trans->m_trans;
		((ShellCore*)core())->GetPlayer()->m_pAVM2->PushObject(pObj);
		//XThread::Sleep(1000);
		//pObj->m_pChannel=channel;
	}
#endif
	pObj->m_pTransform=trans;
	pObj->m_pSoundObj=snd;
	return pObj;
}





SoundChannelObject* SoundChannelClass::CreateSoundChannel(_XSCharacter*ch,SoundTransformObject*trans,int nStart,int nLoops)
{
	SoundChannelObject*pObj=(SoundChannelObject*)createInstance(ivtable(),prototype);
#ifdef SOUND
	XSoundChannel*channel = new XSoundChannel();
	if(channel)
	{
		channel->Init();
		pObj->m_pChannel=channel;
		//channel->completeStop=true;
		channel->AddRef();
		channel->sound = &ch->sound;
		channel->tag = 0;
		channel->loops = nLoops;
		channel->fadeIn=false;
		channel->blockCanBeZero=true;
		channel->completeProc=_CallBack;
		channel->pArgs=pObj;
		if(nStart)
			channel->inPoint44=ch->sound.Rate()*nStart;
		((ShellCore*)core())->GetPlayer()->theSoundMix.AddSound(channel);
		pObj->m_bAdded=true;
		channel->sound->delay=0;
		if(trans)
			channel->m_pTransform=&trans->m_trans;
		((ShellCore*)core())->GetPlayer()->m_pAVM2->PushObject(pObj);
		//XThread::Sleep(1000);
		//pObj->m_pChannel=channel;
	}
#endif
	pObj->m_pTransform=trans;
	return pObj;
}



/*void SoundChannelObject::OnComplete()
{
	if(this->hasEvent(XAS3_EVENT_soundComplete))
	{
		EventObject*pObj=((ShellToplevel*)toplevel())->getEventClass()->CreateEventObject(XAS3_EVENT_soundComplete);
		pObj->pTarget=this;
		this->AS3_dispatchEvent(pObj);
	}
}*/
bool SoundChannelObject::InitOK()
{
	if(m_bAdded||!this->m_pChannel)
		return true;
#ifdef SOUND
	((ShellCore*)core())->GetPlayer()->theSoundMix.AddSound(m_pChannel);
#endif
	m_bAdded=true;
	return true;
}

SoundChannelObject::SoundChannelObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	XSoundChannel* m_pChannel=NULL;
	m_bAdded=false;
//m_bEvent=false;
}

void SoundChannelObject::Release()
{
	//m_pTransform=NULL;
	if(m_pSoundObj&&m_pSoundObj->m_pLoaderInfo)
	{
		m_pSoundObj->m_pLoaderInfo->bPlaying=false;
		m_pSoundObj->m_pLoaderInfo->pObject=NULL;
	}
	if(IsRunning())
		((ShellCore*)core())->GetPlayer()->RemoveEvent(XSWFPlayer::XSOUNDCOMPLETE,(int)this,0,0);
	if(m_pChannel)
	{
		//if(IsRunning())
		{
			m_pChannel->Stop();
			m_pChannel->refPtr=NULL;
			//m_pChannel->streamProc=NULL;
			m_pChannel->completeProc=NULL;
			m_pChannel->Release();
		}
		m_pChannel=NULL;
	}
	EventDispatcherObject::Release();
}
//////////////////////////////////////////////////////////
//Native Method start...
double SoundChannelObject::AS3_leftPeak_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pChannel)
	{
		double v=1;
		if(m_pChannel->nVolume>=0)
			v=(m_pChannel->nVolume&0xff)/100.0;
		if(m_pTransform)
		{
			v=v*m_pTransform->m_trans.m_dVolume/256;
		}
		return v;
	}
	return 0;
}

double SoundChannelObject::AS3_position_get()
{
	//Add your act code here...
	if(m_pChannel&&m_pChannel->sound)
	{
		int nRate=m_pChannel->sound->Rate();
		if(nRate)
			return (double)m_pChannel->pos/nRate;
	}
	return 0;
}

double SoundChannelObject::AS3_rightPeak_get()
{
	//Add your act code here...
	if(m_pChannel)
	{
		double v=1;
		if(m_pChannel->nVolume>=0)
			v=((m_pChannel->nVolume>>8)&0xff)/100.0;
		if(m_pTransform)
		{
			v=v*m_pTransform->m_trans.m_dVolume/256;
		}
		return v;
	}
	return 0;//Modify this please!
}

SoundTransformObject* SoundChannelObject::AS3_soundTransform_get()
{
	//Add your act code here...
	if(m_pTransform==NULL)
	{
		SoundTransformClass*pClass=((ShellToplevel*)toplevel())->getSoundTransformClass();
		m_pTransform=pClass->CreateTransform();
		if(m_pChannel)
			m_pChannel->m_pTransform=&m_pTransform->m_trans;
	}
	return m_pTransform;
}

void SoundChannelObject::AS3_soundTransform_set(SoundTransformObject*obj)
{
	//Add your act code here...
	m_pTransform=obj;
	if(m_pChannel)
		m_pChannel->m_pTransform=&obj->m_trans;
	//return kAvmThunkUndefined;//Modify this please!
}

void SoundChannelObject::Stop()
{
	if(m_pChannel)
	{
		if(m_pSoundObj&&m_pSoundObj->m_pLoaderInfo)
		{
			m_pSoundObj->m_pLoaderInfo->bPlaying=false;
			m_pSoundObj->m_pLoaderInfo->pObject=NULL;
		}
		m_pSoundObj=NULL;
		//m_pChannel->Stop();
		//m_pChannel->remove=true;
		m_pChannel->streamProc=NULL;
		m_pChannel->completeStop=NULL;
		m_pChannel->refPtr=NULL;
		if(hasEvent(XAS3_EVENT_soundComplete))
		{
			//m_bEvent=true;
			((ShellCore*)core())->GetPlayer()->AddEvent(XSWFPlayer::XSOUNDCOMPLETE,(int)this,(int)(ShellCodeContext*)m_codeContext,0);
		}
	}
	if(IsRunning())
	{
		((ShellCore*)core())->GetPlayer()->m_pAVM2->RemoveObject(this);
	}
}

void SoundChannelObject::AS3_stop()
{
	//Add your act code here...
	if(m_pChannel)
	{
		if(m_pSoundObj&&m_pSoundObj->m_pLoaderInfo)
		{
			m_pSoundObj->m_pLoaderInfo->bPlaying=false;
			m_pSoundObj->m_pLoaderInfo->pObject=NULL;
		}
		m_pSoundObj=NULL;
#ifdef SOUND
		//XSoundMix* mix=&((ShellCore*)core())->GetPlayer()->theSoundMix;
		//XLock lock(&mix->m_playLock);
		//m_pChannel->completeProc=NULL;
		//m_pChannel->streamProc=NULL;
		//lock.UnLock();
#endif
		m_pChannel->completeStop=NULL;
		m_pChannel->refPtr=NULL;
		//m_pChannel->remove=true;
		m_pChannel->Stop();
		//m_pChannel->Release();
		//m_pChannel=NULL;

	}
	m_pTransform=NULL;
	if(IsRunning())
	{
		((ShellCore*)core())->GetPlayer()->m_pAVM2->RemoveObject(this);
	}
}

}
