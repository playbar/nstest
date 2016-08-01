#include "StdAfxflash.h"
#include "avmplus.h"
#include "ShellCore.h"
#include "avm2.h"
#include "splayer.h"
#include "XMicrophone.h"
using namespace avmplus;
#include "AS3Microphone.h"
#include "AS3SoundTransform.h"
#include "AS3ActivityEvent.h"
#include "AS3StatusEvent.h"
#include "AS3WaveStream.h"
#include "AS3NetStream.h"
namespace avmshell{
MicrophoneClass::MicrophoneClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())MicrophoneObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* MicrophoneClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) MicrophoneObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

static void _MicroCall(void*pArg,int id,void*pData,int l)
{
	MicrophoneObject*pObj=(MicrophoneObject*)pArg;
	if(pObj==XNULL) return;
	XSWFPlayer*player=((ShellCore*)pObj->core())->GetPlayer();
	//m_nFramesPacket = 128*4;
	//ShellCore* c=(ShellCore*)pObj->core();
	switch(id)
	{
	case MICROPHONE_DATA://=1000,
		 pObj->DataIn(pData,l);
		 break;
	case MICROPHONE_ACTIVE://=1001,
		 //pObj->OnActivityEvent(true);
		 player->m_pAVM2->PushObject(pArg);
		 player->AddEvent(XSWFPlayer::XACTIVITY,(int)pArg,(int)(ShellCodeContext*)pObj->m_codeContext,1);
		 break;
	case MICROPHONE_DEACTIVE://=1002
		 //pObj->OnActivityEvent(false);
		 player->m_pAVM2->PushObject(pArg);
		 player->AddEvent(XSWFPlayer::XACTIVITY,(int)pArg,(int)(ShellCodeContext*)pObj->m_codeContext,0);
		 break;
	}
}

void MicrophoneObject::DataIn(void*pData,int l)
{
	int i,iCount=m_netAttachs.GetSize();
	for(i=0;i<iCount;i++)
	{
		if(m_netAttachs[i])// continue;
		m_netAttachs[i]->WaveIn(pData,l);
	}
	iCount=m_waveAttachs.GetSize();
	for(i=0;i<iCount;i++)
	{
		if(m_waveAttachs[i])
			m_waveAttachs[i]->WaveIn(pData,l);
	}
}

void MicrophoneObject::AttachNetStream(NetStreamObject*obj)
{
	int i,iCount=m_netAttachs.GetSize();	
	for(i=0;i<iCount;i++)
	{
		if(m_netAttachs[i]==obj) return;
	}
	for(i=0;i<iCount;i++)
	{
		if(m_netAttachs[i]==XNULL)
			m_netAttachs[i]=obj;
	}
	m_netAttachs.Add(obj);
	m_pMicrophone->Start();
}

void MicrophoneObject::UnAttachNetStream(NetStreamObject*obj)
{
	int i,iCount=m_netAttachs.GetSize();		
	for(i=0;i<iCount;i++)
	{
		if(m_netAttachs[i]==obj)
		{
			m_netAttachs[i]=XNULL;
			//return;
			break;
		}
	}	
}



void MicrophoneObject::AttachWaveStream(BXWaveStreamObject*obj)
{
	int i,iCount=m_netAttachs.GetSize();	
	for(i=0;i<iCount;i++)
	{
		if(m_waveAttachs[i]==obj) return;
	}
	for(i=0;i<iCount;i++)
	{
		if(m_waveAttachs[i]==XNULL)
			m_waveAttachs[i]=obj;
	}
	m_waveAttachs.Add(obj);
	//m_pMicrophone->Start();
}

void MicrophoneObject::UnAttachWaveStream(BXWaveStreamObject*obj)
{
	int i,iCount=m_netAttachs.GetSize();		
	for(i=0;i<iCount;i++)
	{
		if(m_waveAttachs[i]==obj)
		{
			m_waveAttachs[i]=XNULL;
			//return;
			break;
		}
	}	
}




MicrophoneObject::MicrophoneObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	//XSoundMix*pMix=&((ShellCore*)core())->GetPlayer()->theSoundMix;
	m_pMicrophone = new XMicrophone();
	m_pMicrophone->Attach(_MicroCall,this);
	m_bEchoSupperession = false;
	m_bFirst = true;
	m_nFramesPacket = 128*4;
	m_nNoiseSuppression = 0;
	//m_pMicrophone = new XMicrophone();
	//int v=sizeof(MicrophoneObject);
}

void MicrophoneObject::Release()
{
	if(m_pMicrophone)
	{
		m_pMicrophone->Stop();
		delete m_pMicrophone;
		m_pMicrophone = XNULL;
	}
	EventDispatcherObject::Release();
	//m_phone.Stop();
}
//////////////////////////////////////////////////////////
//Native Method start...
ArrayObject* MicrophoneClass::AS3_names_get()
{
	//Add your act code here...
	ArrayObject*pArray=toplevel()->arrayClass->newArray(0);
	XString8Array list;
	int i,iCount=XMicrophone::GetNames(list);
	AvmCore*c=core();
	for(i=0;i<iCount;i++)
	{
		Stringp name=c->newStringUTF8(list[i]);
		Atom atoms[]={name->atom()};
		pArray->push(atoms,1);
	}
	return pArray;
}

MicrophoneObject* MicrophoneClass::AS3_getMicrophone(int id)
{
	//Add your act code here...
	//return NULL;
	MicrophoneObject*pObj=(MicrophoneObject*)createInstance(ivtable(),prototype);
	pObj->SetID(id);
	return pObj;
	//return NULL;
}

//new adds
MicrophoneObject* MicrophoneClass::AS3_getEnhancedMicrophone(int index)
{
	MicrophoneObject*pObj=(MicrophoneObject*)createInstance(ivtable(),prototype);
	pObj->SetID(index);
	return pObj;
}
//new adds end

bool MicrophoneObject::SetID(int id)
{
	return m_pMicrophone->SetID(id);
}

double MicrophoneObject::AS3_activityLevel_get()
{
	//Add your act code here...
	return m_pMicrophone->GetActiveLevel();
}

double MicrophoneObject::AS3_gain_get()
{
	//Add your act code here...
	return m_pMicrophone->GetGain();
}

void MicrophoneObject::AS3_gain_set(double gain)
{
	//Add your act code here...
	m_pMicrophone->SetGain(gain);
}

int MicrophoneObject::AS3_index_get()
{
	//Add your act code here...
	return m_pMicrophone->GetIndex();
}

bool MicrophoneObject::AS3_muted_get()
{
	//Add your act code here...
	return false;
}

Stringp MicrophoneObject::AS3_name_get()
{
	//Add your act code here...
	XString8 name;
	if(m_pMicrophone->GetName(name))
	{
		AvmCore*c=core();
		return c->newStringUTF8(name);
	}
	return core()->kEmptyString;
}

int MicrophoneObject::AS3_rate_get()
{
	//Add your act code here...
	return m_pMicrophone->GetRate();
}

void MicrophoneObject::AS3_rate_set(int v)
{
	//Add your act code here...
	m_pMicrophone->SetRate(v);
	
}

double MicrophoneObject::AS3_silenceLevel_get()
{
	//Add your act code here...
	return m_pMicrophone->GetSilenceLevel();
						 // GetSilenceLevel
}

int MicrophoneObject::AS3_silenceTimeout_get()
{
	//Add your act code here...
	return m_pMicrophone->GetSilenceTimeout();
}

bool MicrophoneObject::AS3_useEchoSuppression_get()
{
	//Add your act code here...
	return m_bEchoSupperession;
}

void MicrophoneObject::AS3_setLoopBack(bool b)
{
	//Add your act code here...
	//bool b=m_pMicrophone->IsStarted();
	XSoundMix*pMix=&((ShellCore*)core())->GetPlayer()->theSoundMix;
	m_pMicrophone->SetLoopback(b,pMix,(m_netAttachs.GetSize()<=0&&m_waveAttachs.GetSize()<=0));
	if(b)
	{
		if(!m_pMicrophone->IsStarted())
		{
			Stringp str=core()->newConstantStringLatin1("Microphone.error");
			this->OnStatusEvent(str,StatusEventObject::LV_ERROR);
		}
		else if(m_bFirst)
		{
			m_bFirst = false;
			Stringp str=core()->newConstantStringLatin1("Microphone.Unmuted");
			this->OnStatusEvent(str,StatusEventObject::LV_STATUS);
		}
	}
}

void MicrophoneObject::AS3_setSilenceLevel(double v,int t)
{
	//Add your act code here...
	m_pMicrophone->SetSilenceLevel(v,t);
}

void MicrophoneObject::AS3_soundTransform_set(SoundTransformObject*p)
{
	this->m_pTransform=p;
	if(p)
		m_pMicrophone->SetTransform(&p->m_trans);
	else
		m_pMicrophone->SetTransform(XNULL);
}

void MicrophoneObject::AS3_setUseEchoSuppression(bool b)
{
	//Add your act code here...
	m_bEchoSupperession = b;
}

//new adds
Stringp MicrophoneObject::AS3_codec_get()
{
	return m_pCodec;
}
void MicrophoneObject::AS3_codec_set(Stringp pCodec)
{
	m_pCodec=pCodec;
}
bool MicrophoneObject::AS3_enableVAD_get()
{
	return m_bVAD;
}
void MicrophoneObject::AS3_enableVAD_set(bool enableVAD)
{
	m_bVAD=enableVAD;
}
int MicrophoneObject::AS3_encodeQuality_get()
{
	return m_nQuality;
}
void MicrophoneObject::AS3_encodeQuality_set(int encodeQuality)
{
	m_nQuality = encodeQuality;
}
MicrophoneEnhancedOptionsObject* MicrophoneObject::AS3_enhancedOptions_get()
{
	return m_pEnhancedOption;
}
void MicrophoneObject::AS3_enhancedOptions_set(MicrophoneEnhancedOptionsObject *pMicrophoneEnhancedOptions)
{
	m_pEnhancedOption = pMicrophoneEnhancedOptions;
}
int MicrophoneObject::AS3_framesPerPacket_get()
{
	return m_nFramesPacket;
}
void MicrophoneObject::AS3_framesPerPacket_set(int framesPerPacket)
{
	m_nFramesPacket = framesPerPacket;
}
//bool MicrophoneObject::AS3_isSupported_get()
//{
//	return false;
//}
int MicrophoneObject::AS3_noiseSuppressionLevel_get()
{
	return m_nNoiseSuppression;
}
void MicrophoneObject::AS3_noiseSuppressionLevel_set(int noiseSuppressionLevel)
{
	m_nNoiseSuppression = noiseSuppressionLevel;
}
//new adds end

}