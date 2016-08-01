#include "StdAfxGGBase.h"
#include "XMicrophone.h"
#include "XWaveIn.h"
#include "XRingStream.h"
//#include "XTime.h"
XMicrophone::XMicrophone()
{
	this->m_gain=50;
	this->m_activeLevel=-1;
	this->m_silenceLevel=10;
	this->m_silienceTimeout=2000;
	m_bActive = false;
	m_pDevice = new XWaveIn(this);
	m_nLastTime = 0;
	m_bStart = false;
	m_bLoopback = false;
	m_pTrans = XNULL;
	m_pChannel = XNULL;
	memset(&m_sound,0,sizeof(m_sound));
	//int v=sizeof(XMicrophone);
	//v=sizeof(XWaveIn);
	//m_pStream = NULL;
}

XMicrophone::~XMicrophone()
{
	SetLoopback(false,NULL,true);
	
	if(m_pDevice)
	{
		m_pDevice->Stop();
		delete m_pDevice;
		m_pDevice = NULL;
	}
	if(m_pChannel)
		m_pChannel->Release();
	m_pChannel=XNULL;
}

bool XMicrophone::Start()
{
	if(m_bStart) return false;
	m_pDevice->Start();
	m_bStart = true;
	return true;
}

bool XMicrophone::Stop()
{
	SetLoopback(false,NULL,true);
	m_pDevice->Stop();
	m_bStart = false;
	return true;
}

int XMicrophone::GetNames(XString8Array&list)
{
	return XWaveIn::GetNames(list);
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

static BOOL _StreamProc(XSoundChannel* channel)
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
	return XWaveIn::GetName(m_nID,name);
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
	return m_pDevice->GetFormat()->nChannels;
}
int XMicrophone::GetBitsPerSample()
{
	return m_pDevice->GetFormat()->wBitsPerSample;
}

int XMicrophone::GetRate()
{
	WAVEFORMATEX*pFmt=m_pDevice->GetFormat();
	switch(pFmt->nSamplesPerSec)
	{
		default:
		case XPCM_8K:
			 return 8;
		case XPCM_11K:return 11;
		case XPCM_12K:return 12;
		case XPCM_16K:return 16;
		case XPCM_22K:return 22;
		case XPCM_24K:return 24;
		case XPCM_32K:return 32;
		case XPCM_44K:return 44;
		case XPCM_48K:return 48;
	}
	return 11;
}

void XMicrophone::SetRate(int v)
{
	if(m_bStart)
		return;
	switch(v)
	{
	default:
		return;
	case 11:
		m_pDevice->SetSamples(XPCM_11K);break;
	case 12:
		m_pDevice->SetSamples(XPCM_12K);break;
	case 16:
		m_pDevice->SetSamples(XPCM_16K);break;
	case 22:
		m_pDevice->SetSamples(XPCM_22K);break;
	case 24:
		m_pDevice->SetSamples(XPCM_24K);break;
	case 32:
		m_pDevice->SetSamples(XPCM_32K);break;
	case 44:
		m_pDevice->SetSamples(XPCM_44K);break;
	case 48:
		m_pDevice->SetSamples(XPCM_48K);break;
	}
}

bool XMicrophone::SetLoopback(bool bLoop,XSoundMix*pMix,bool bClose)
{
	m_pMix=pMix;
	if(m_bStart==bLoop)
		return true;
	m_bLoopback = bLoop;
	XLock lock(&m_lock);
	if(lock.Lock(XSHORTTIME))
	{
		if(bLoop)
		{		
			if(!m_bStart)
			{
				//m_bStart = true;
				if(!m_pDevice->Start()) return false;
				m_bStart = true;
			}
			m_blocks.Init(WAVEBUFSIZE);//m_pDevice->GetBufferSize());
			WAVEFORMATEX*pFmt=m_pDevice->GetFormat();
			m_sound.format = sndCompressNone;
			if(pFmt->nChannels==2)
				m_sound.format |= sndStereo;
			if(pFmt->wBitsPerSample==16)
				m_sound.format|=snd16Bit;
			switch(pFmt->nSamplesPerSec)
			{
				default:
				case XPCM_8K:
				case XPCM_11K:
				case XPCM_12K:
				case XPCM_16K:
					 m_sound.format |= 1<<2;
					 break;			
				case XPCM_22K:
				case XPCM_24K:
				case XPCM_32K:
					 m_sound.format |= 2<<2;
					 break;
				case XPCM_44K:
				case XPCM_48K:
					 m_sound.format |= 3<<2;
					 break;
			}

			XSoundChannel*channel=m_pChannel;// = new XSoundChannel();
			if(channel == XNULL)
			{
				channel = new XSoundChannel();
				channel->AddRef();
			}
			if(channel)
			{
				channel->Init();
				m_pChannel=channel;
				//channel->completeStop=true;
				channel->AddRef();
				channel->samplesPlayed=0;
				channel->sound = &m_sound;
				channel->tag = 0;
				channel->loops = 1;
				channel->completeProc=_EndProc;
				channel->pArgs= this;
				channel->refPtr = this;
				channel->streamProc = _StreamProc;
				//if(nStart)
				//	channel->inPoint44=ch->sound.Rate()*nStart;
				m_pMix->AddSound(channel);
				channel->sound->delay=0;
				if(channel->decomp)
				{
					channel->decomp->Setup(&m_sound, false);
				}
				if(m_pTrans)
					channel->m_pTransform=m_pTrans;
			}
		}
		else
		{
			
			if(m_pChannel)
			{
				//if(IsRunning())
				{
					m_pChannel->Stop();
					m_pChannel->refPtr=NULL;
					m_pChannel->streamEnd = true;
					//m_pChannel->streamProc=NULL;
					m_pChannel->completeProc=NULL;
					m_pChannel->streamProc = NULL;
					//m_pChannel->Release();
				}
				//m_pChannel=NULL;
			}	
			if(bClose&&m_bStart)
			{
				m_pDevice->Stop();
				m_bStart=false;
			}
		}
		lock.UnLock();
	}
	return true;
}

bool XMicrophone::DataIn(void *pData)
{
	WAVEHDR*pHDR=(WAVEHDR*)pData;
	if(pHDR->dwBytesRecorded<=0) return false;
	WAVEFORMATEX*pFmt=m_pDevice->GetFormat();
	if(m_bLoopback)
	{
		XLock lock(&m_lock);
		if(lock.Lock(XSHORTTIME))
		{
			int iSamples=pHDR->dwBytesRecorded;
			if(pFmt->nChannels==2) iSamples>>=1;
			if(pFmt->wBitsPerSample==16) iSamples>>=1;
			m_blocks.SetBlock(pHDR->lpData,pHDR->dwBytesRecorded,iSamples);
			//m_stream.WriteData(pHDR->lpData,pHDR->dwBytesRecorded);
			lock.UnLock();
		}
	}
	//int iCount=m_attachs.GetSize(),i;
	
	
	int gain=0;
	if(pFmt->wBitsPerSample==8)
	{
		XS8* pData=(XS8*)pHDR->lpData;
		int i,iCount=pHDR->dwBytesRecorded;
		for(i=0;i<iCount;i++)
		{
			XS16 c=pData[i]+128;
			//if(c<0) c=-c;
			c=c<<8;
			if(gain<c)
				gain=c;
		}
	}
	else
	{
		XS16* pData=(XS16*)pHDR->lpData;
		int i,iCount=pHDR->dwBytesRecorded/2;
		for(i=0;i<iCount;i++)
		{
			XS16 c=pData[i]+32768;
			//if(c<0) c=-c;
			//c=c<<8;
			if(gain<c)
				gain=c;
		}
	}
	gain=(gain*m_gain*5)>>15;
	if(gain>100)
		gain=100;
	if(gain>=this->m_silenceLevel)
	{
		this->m_activeLevel=gain;
		if(!m_bActive)
		{
			m_bActive=XTRUE;
			Call(MICROPHONE_ACTIVE);
		}
	}
	else if(m_bActive)
	{
		XU32 t=::GetTickCount();
		if(m_activeLevel>=m_silenceLevel)
			m_nLastTime=t;
		else if(t-m_nLastTime>=this->m_silienceTimeout)
		{
			m_bActive=false;
			this->m_activeLevel=gain;
			Call(MICROPHONE_DEACTIVE);
		}
		this->m_activeLevel=gain;
	}
	
	Call(MICROPHONE_DATA,pHDR->lpData,pHDR->dwBytesRecorded);
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
	int iCount=0,i;
	for(i=0;i<m_attachs.GetSize();i++)
	{
		_MICROPHONEATTACH&c=m_attachs[i];
		if(c.pArg==pArg)
		{
			c.pArg=XNULL;
			c.call=XNULL;
		}
		else if(c.pArg&&c.pArg)
			iCount++;
	}	
	if(!iCount)
		m_pDevice->Stop();
	return true;
}

XS32* XMicrophone::GetDCTData(int&l)
{
	return m_pDevice->GetDCTData(l);
}