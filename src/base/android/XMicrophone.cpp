#include "StdAfxGGBase.h"
#include "XMicrophone.h"
//#include "XWaveIn.h"
#include "XRingStream.h"
//#include "XTime.h"
XMicrophone::XMicrophone()
{
	this->m_gain=50;
	this->m_activeLevel=-1;
	this->m_silenceLevel=10;
	this->m_silienceTimeout=2000;
	m_bActive = false;
	
	m_nLastTime = 0;
	m_bStart = false;
	m_bLoopback = false;
	m_pTrans = XNULL;
	memset(&m_sound,0,sizeof(m_sound));
	//int v=sizeof(XMicrophone);
	//v=sizeof(XWaveIn);
	//m_pStream = NULL;
}

XMicrophone::~XMicrophone()
{
	SetLoopback(false,NULL,true);
	
	
}

bool XMicrophone::Start()
{
	if(m_bStart) return false;
	
	m_bStart = true;
	return true;
}

bool XMicrophone::Stop()
{
	SetLoopback(false,NULL,true);

	m_bStart = false;
	return true;
}

int XMicrophone::GetNames(XString8Array&list)
{
	return 0;
}

static void _EndProc(void*pData,bool bRemove)
{
	//SoundChannelObject*pSound=(SoundChannelObject*)pData;
	//if(pSound==NULL) return;

//pSound->m_bEvent=true;
	//XSWFPlayer*player=((ShellCore*)pSound->core())->GetPlayer();
	//player->m_pAVM2->PushObject(pData);
	//player->AddEvent(XSWFPlayer::XSOUNDCOMPLETE,(int)pSound,(int)(ShellCodeContext*)pSound->m_codeContext,(int)bRemove);
}

static XBOOL _StreamProc(XSoundChannel* channel)
// WARNING: this is called at interupt time on the Mac, don't do any memory allocations...
{
	XMicrophone* m = (XMicrophone*)(channel->refPtr);
	if(m==NULL) return XFALSE;

	/*if(m->m_pChannel == NULL)
	{
		channel->loops=0;
		channel->streamProc=NULL;
		return XFALSE;
		//return XFALSE;
	}*/
	//AvmAssert(snd->m_pChannel == channel);
	return m->DoSound(channel);
}

void XMicrophone::SetTransform(XSoundTransform*trans)
{
	this->m_pTrans=trans;
	XLock lock(&m_lock);
	if(lock.Lock(XSHORTTIME))
	{
		if(m_pChannel)
			m_pChannel->m_pTransform=trans;
		lock.UnLock();
	}
}

bool XMicrophone::GetName(XString8&name)
{
	return false;
}

int XMicrophone::DoSound(XSoundChannel*channel)
{

	XLock lock(&m_lock);
	if(lock.Lock(XSHORTTIME))
	{
		if(m_pChannel == NULL)
		{
			channel->loops=0;
			channel->streamProc = XNULL;
			channel->completeProc = XNULL;
			lock.UnLock();
			return 0;
		}
		_WAVEBLOCK*pb=m_blocks.GetBlock();		
		channel->blockCanBeZero = true;
		if(pb)
		{
			XSound*snd=channel->sound;
			channel->blockSamples= pb->nSamples;
			//WAVEFORMATEX*pFmt=m_pDevice->GetFormat();
			//int nBlock=pFmt->nChannels*pFmt->wBitsPerSample/8;
			//snd->samples=(void*)pb->nSamples;
			snd->dataLen=pb->nSize;
			snd->samples=pb->pData;
			if(channel->decomp)
			{
				channel->decomp->Setup(snd, false);
			}
		}
		lock.UnLock();
	}
	return 1;
}


int  XMicrophone::GetChannels()
{
	return 0;
}
int XMicrophone::GetBitsPerSample()
{
	return 0;
}

int XMicrophone::GetRate()
{
	return 11;
}

void XMicrophone::SetRate(int v)
{
	if(m_bStart)
		return;
}

bool XMicrophone::SetLoopback(bool bLoop,XSoundMix*pMix,bool bClose)
{
	return true;
}

bool XMicrophone::DataIn(void *pData)
{
	return true;
}

void XMicrophone::Call(int nCmd,void*pData,int l)
{
	int i,iCount=m_attachs.GetSize();
	for(i=0;i<iCount;i++)
	{
		_MICROPHONEATTACH&c=m_attachs[i];
		if(c.call&&c.pArg)
			c.call(c.pArg,nCmd,pData,l);
	}
}


bool  XMicrophone::Attach(_MICROPHONECALLBACK call,void*pArg)
{
	int iCount=0,i;
	bool bNew=true;
	for(i=0;i<m_attachs.GetSize();i++)
	{
		_MICROPHONEATTACH&c=m_attachs[i];
		if(!c.call||!c.pArg)
		{
			if(pArg)
			{
				c.call=call;
				c.pArg=pArg;
				iCount++;
				bNew=false;
			}
		}
		else
			iCount++;
	}
	if(bNew)
	{
		_MICROPHONEATTACH c={call,pArg};
		m_attachs.Add(c);
	}
	//if(iCount)
	//	m_pDevice->Start();
	return true;
}
bool  XMicrophone::UnAttach(void*pArg)
{
	return true;
}
XS32* XMicrophone::GetDCTData(int&l)
{
	//return m_pDevice->GetDCTData(l);
    return NULL;
}
