#include "StdAfxflash.h"
#include "ShellCore.h"
#include "splayer.h"
#include "avmplus.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3SoundMixer.h"
#include "AS3SoundTransform.h"
#include "AS3ByteArray.h"

namespace avmshell{
SoundMixerClass::SoundMixerClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())SoundMixerObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
	m_strAudioPlaybackMode = NULL;//((ShellToplevel*)toplevel())->getAudioPlaybackModeClass()->getSlotMEDIA();
	m_bUseSpeakerphoneForVoice = false;
};
ScriptObject* SoundMixerClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) SoundMixerObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}
SoundMixerObject::SoundMixerObject(VTable *vtable, ScriptObject* proto, int capacity): ScriptObject(vtable, proto, 0)
{
	//Add your construct code here...
}
//////////////////////////////////////////////////////////
//Native Method start...

void SoundMixerClass::AS3_computeSpectrum(ByteArrayObject*b,bool,int)
{
	//Add your act code here...
	b->SetLength(512*sizeof(4));
	b->Seek(0);
}

void SoundMixerClass::AS3_stopAll()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
#ifdef SOUND
	player->theSoundMix.StopAll();//..SetVolume(0);
#endif
}

int SoundMixerClass::AS3_bufferTime_get()
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	return player->nSoundBufferTime;
}

SoundTransformObject* SoundMixerClass::AS3_soundTransform_get()
{
	//Add your act code here...
	//char*address=(char*)player->theSoundMix.m_pTransformn;
	if(m_pTransform==NULL)//!address) //return NULL;
	{
		XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
		SoundTransformClass*pClass=((ShellToplevel*)toplevel())->getSoundTransformClass();
		m_pTransform=pClass->CreateTransform();
#ifdef SOUND
		player->theSoundMix.m_pTransformn=&m_pTransform->m_trans;
#endif
		//if(m_pChannel)
		//	m_pChannel->m_pTransform=&m_pTransform->m_trans;
	}
	return m_pTransform;
	//return ((ShellToplevel*)toplevel())->getSoundTransformClass()->GetObjectOf(address);
}

void SoundMixerClass::AS3_bufferTime_set(int v)
{
	//Add your act code here...
	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	player->nSoundBufferTime=v;
}

void SoundMixerClass::AS3_soundTransform_set(SoundTransformObject*obj)
{
	//Add your act code here...
	//int nSize=((char*)&obj->m_trans)-(char*)obj;

	XSWFPlayer*player=((ShellCore*)core())->GetPlayer();
	m_pTransform=obj;
	//ShellToplevel*top=((ShellToplevel*)toplevel());
	//char*address=(char*)player->theSoundMix.m_pTransformn;
	//if(address)
#ifdef SOUND
	if(obj)
	{
		player->theSoundMix.m_pTransformn=&obj->m_trans;
	}
	else
	{
		player->theSoundMix.m_pTransformn=XNULL;
	}
#endif

}
//new adds 11
 
Stringp SoundMixerClass::AS3_audioPlaybackMode_get()
{
	//LOGWHERE();
	if(!m_strAudioPlaybackMode)
		return core()->newConstantStringLatin1("media");
	else return m_strAudioPlaybackMode;//Stringp("media");
}
void SoundMixerClass::AS3_audioPlaybackMode_set(Stringp backMode)
{
	//LOGWHERE();
	m_strAudioPlaybackMode = backMode;
}
 
bool SoundMixerClass::AS3_useSpeakerphoneForVoice_get()
{
	//LOGWHERE();
	return m_bUseSpeakerphoneForVoice;
}
void SoundMixerClass::AS3_useSpeakerphoneForVoice_set(bool b)
{

	//LOGWHERE();
	m_bUseSpeakerphoneForVoice = b;
}
//new adds 11 end 

}
