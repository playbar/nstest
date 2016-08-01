#ifndef _XMicrophone_
#define _XMicrophone_

#include "GGType.h"
#include "XString.h"
#include "XMutex.h"
#include "XNativeSoundMix.h"
#include "XStream.h"

enum
{
	MICROPHONE_DATA=1000,
	MICROPHONE_ACTIVE=1001,
	MICROPHONE_DEACTIVE=1002
};
typedef void  (*_MICROPHONECALLBACK)(void*,int id,void*,int l);

struct _WAVEBLOCK
{
	XU8* pData;
	int  nSize;
	int  nSamples;
	_WAVEBLOCK* pNext;
};
#define MAXWAVEBLOCKSIZE 32678
class XRingBlock
{
public:
	XRingBlock()
	{
		m_pGet=m_pSet=XNULL;
		m_nBlockSize=4096;
	}
	void Init(int nSize)
	{
		Free();
		m_nBlockSize=nSize;
		m_pGet=NewBlock(m_pGet);
		m_pSet=m_pGet;
		m_pGet=NewBlock(m_pGet);
		m_pGet=NewBlock(m_pGet);
		m_pSet->pNext=m_pGet;
		m_pSet=m_pGet;
	}
	
	_WAVEBLOCK* NewBlock(_WAVEBLOCK*next)
	{
		_WAVEBLOCK*p=new _WAVEBLOCK();
		p->pNext=next;
		p->pData=new XU8[m_nBlockSize];
		return p;
	}


	_WAVEBLOCK* GetBlock()
	{
		if(m_pGet==m_pSet) return XNULL;
		_WAVEBLOCK*pBlock=m_pGet;
		m_pGet=m_pGet->pNext;
		return pBlock;
	}
	bool SetBlock(void*pData,int nSize,int nSamples)
	{
		memcpy(m_pSet->pData,pData,nSize);
		m_pSet->nSize=nSize;
		m_pSet->nSamples=nSamples;
		if(m_pSet->pNext==m_pGet||m_pSet->pNext->pNext==m_pGet)
		{
			m_pSet->pNext=NewBlock(m_pSet->pNext);
			//_WAVEBLOCK*p=new _WAVEBLOCK();
			//p->pData=new XU8[m_nBlockSize];
			//p->pNext=m_pSet->pNext;
			//m_pSet->pNext=p;
		}
		m_pSet=m_pSet->pNext;
		return true;
	}
	~XRingBlock()
	{
		Free();
	}
	void Free()
	{
		_WAVEBLOCK*p=m_pGet;
		while(p)
		{
			_WAVEBLOCK*next=p->pNext;
			delete p->pData;
			delete p;
			p=next;
			if(p==m_pGet) break;
		}
		m_pGet=m_pSet=XNULL;
	}

	
protected:
	int m_nBlockSize;
	_WAVEBLOCK* m_pGet;
	_WAVEBLOCK* m_pSet;
};

typedef struct _MICROPHONEATTACH
{
	_MICROPHONECALLBACK call;
	void*		    pArg;
}MICROPHONEATTACH;

class XWaveIn;
class XSoundChannel;
class XRingStream;
class XMicrophone
{
public:
	XMicrophone();
	~XMicrophone();
public:
	int	 DoSound(XSoundChannel*channel);
	static int GetNames(XString8Array&list);
	bool SetID(int id)
	{
		m_nID=id;
		return true;
	}
	int  GetIndex(){return m_nID;}
	//void GetName(XString8&name);
	bool DataIn(void*pData);
#ifdef __APPLE__
    bool DataIn(void *bufRef, void *desc);
#endif
	bool Attach(_MICROPHONECALLBACK call,void*pArg);
	bool SetLoopback(bool bLoop,XSoundMix*pMix,bool bClose);
	bool UnAttach(void*pArg);
	bool Stop();
	bool Start();	
	int  GetChannels();
	int  GetBitsPerSample();
	int GetActiveLevel()
	{
		return m_activeLevel;
	}
	int GetGain(){return m_gain;}
	void SetGain(int g){m_gain=g;}
	void SetTransform(XSoundTransform*trans);
	void SetSilenceLevel(int v,int t)
	{
		m_silenceLevel=v;
		m_silienceTimeout=t;
	}
	void setDCT()
	{
		m_bDCT = true;
	}
	int GetRate();
	void SetRate(int v);
	bool GetName(XString8&name);
	int GetSilenceLevel(){return m_silenceLevel;}
	int GetSilenceTimeout(){return m_silienceTimeout;}
	bool IsStarted(){return m_bStart;}
	bool IsLoopback(){return m_bLoopback;}
	XS32* GetDCTData(int&l);
protected:
	//XStream m_stream;
	XRingBlock m_blocks;
	XSoundChannel* m_pChannel;
	XSoundMix* m_pMix;
	XSound	m_sound;
	XSoundTransform* m_pTrans;
	bool   m_bStart;
	bool   m_bDCT;
	bool   m_bLoopback;
	void   Call(int nCmd,void*pData=XNULL,int l=0);
	int	   m_activeLevel;
	int	   m_gain;
	int    m_silenceLevel;
	int	   m_silienceTimeout;
	bool   m_bActive;
	XU32   m_nLastTime;
	XMutex m_lock;
	XWaveIn* m_pDevice;
	int m_nID;
	
	XArray<_MICROPHONEATTACH> m_attachs;
};

#endif