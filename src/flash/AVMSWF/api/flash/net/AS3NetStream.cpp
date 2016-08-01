#include "StdAfxflash.h"
#include "ShellCore.h"
using namespace avmplus;
#include "AS3NetStream.h"
#include "AS3SoundTransform.h"

namespace avmshell{
NetStreamClass::NetStreamClass(VTable* cvtable):ClassClosure(cvtable)
{
	createVanillaPrototype();
	//prototype = new (core()->GetGC(), cvtable->getExtraSize())NetStreamObject(cvtable, toplevel()->objectClass->prototype, 0);
	//Add your construct code here...
};
ScriptObject* NetStreamClass::createInstance(VTable *ivtable, ScriptObject* prototype)
{
	return new (core()->GetGC(), ivtable->getExtraSize()) NetStreamObject(ivtable, prototype, 0);
	//AS3 'new' opt...Add your init code here...
}

//new adds
void NetStreamClass::AS3_resetDRMVouchers()
{

}
//new adds end

NetStreamObject::NetStreamObject(VTable *vtable, ScriptObject* proto, int capacity): EventDispatcherObject(vtable, proto, 0)
{
	//Add your construct code here...
	m_bUseHardwareDecoder = true;
	m_bUseJitterBuffer = false;
	m_pSettings = NULL;
}
//////////////////////////////////////////////////////////
//Native Method start...
AvmBox NetStreamObject::AS3_bufferLength_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_bufferTime_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_bufferTime_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_bytesLoaded_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_bytesTotal_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_checkPolicyFile_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_checkPolicyFile_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_client_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_client_set(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_currentFPS_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_liveDelay_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_objectEncoding_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

SoundTransformObject* NetStreamObject::AS3_soundTransform_get()
{
	//Add your act code here...
	//return kAvmThunkUndefined;//Modify this please!
	if(m_pTrans==NULL)
	{
		SoundTransformClass*pClass=((ShellToplevel*)toplevel())->getSoundTransformClass();
		m_pTrans=pClass->CreateTransform();
		//if(m_pChannel)
		//	m_pChannel->m_pTransform=&m_pTransform->m_trans;
	}
	return m_pTrans;
}

void NetStreamObject::AS3_soundTransform_set(SoundTransformObject*trans)
{
	//Add your act code here...
	m_pTrans=trans;
}

AvmBox NetStreamObject::AS3_time_get(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

//AS3 contructor function..
AvmBox NetStreamObject::AS3_constructor(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_attachAudio(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_attachCamera(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_close(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_pause(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_play(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_publish(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_receiveAudio(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_receiveVideo(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_resume(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_seek(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_send(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

AvmBox NetStreamObject::AS3_togglePause(AvmMethodEnv, uint32_t, AvmBox* argv)
{
	//Add your act code here...
	return kAvmThunkUndefined;//Modify this please!
}

//new adds
bool NetStreamObject::AS3_audioReliable_get()
{
	return false;
}
void NetStreamObject::AS3_audioReliable_set(bool audioReliable)
{

}
bool NetStreamObject::AS3_audioSampleAccess_get()
{
	return false;
}
void NetStreamObject::AS3_audioSampleAccess_set(bool audioSampleAccess)
{

}
double NetStreamObject::AS3_backBufferLength_get()
{
	return 0;
}
double NetStreamObject::AS3_backBufferTime_get()
{
	return 0;
}
void NetStreamObject::AS3_backBufferTime_set(double backBufferTime)
{

}
double NetStreamObject::AS3_bufferTimeMax_get()
{
	return 0;
}
void NetStreamObject::AS3_bufferTimeMax_set(double bufferTimeMax)
{

}
bool NetStreamObject::AS3_dataReliable_get()
{
	return false;
}
void NetStreamObject::AS3_dataReliable_set(bool dataReliable)
{

}
Stringp NetStreamObject::AS3_farID_get()
{
	return NULL;
}
Stringp NetStreamObject::AS3_farNonce_get()
{
	return NULL;
}
bool NetStreamObject::AS3_inBufferSeek_get()
{
	return false;
}
void NetStreamObject::AS3_inBufferSeek_set(bool inBufferSeek)
{

}
NetStreamInfoObject* NetStreamObject::AS3_info_get()
{
	return NULL;
}
double NetStreamObject::AS3_maxPauseBufferTime_get()
{
	return 0;
}
void NetStreamObject::AS3_maxPauseBufferTime_set(double maxPauseBufferTime)
{

}
bool NetStreamObject::AS3_multicastAvailabilitySendToAll_get()
{
	return false;
}
void NetStreamObject::AS3_multicastAvailabilitySendToAll_set(bool multicastAvailabilitySendToAll)
{

}
double NetStreamObject::AS3_multicastAvailabilityUpdatePeriod_get()
{
	return 0;
}
void NetStreamObject::AS3_multicastAvailabilityUpdatePeriod_set(double multicastAvailabilityUpdatePeriod)
{

}
double NetStreamObject::AS3_multicastFetchPeriod_get()
{
	return 0;
}
void NetStreamObject::AS3_multicastFetchPeriod_set(double multicastFetchPeriod)
{

}
NetStreamMulticastInfoObject* NetStreamObject::AS3_multicastInfo_get()
{
	return NULL;
}
double NetStreamObject::AS3_multicastPushNeighborLimit_get()
{
	return 0;
}
void NetStreamObject::AS3_multicastPushNeighborLimit_set(double multicastPushNeighborLimit)
{

}
double NetStreamObject::AS3_multicastRelayMarginDuration_get()
{
	return 0;
}
void NetStreamObject::AS3_multicastRelayMarginDuration_set(double multicastRelayMarginDuration)
{

}
double NetStreamObject::AS3_multicastWindowDuration_get()
{
	return 0;
}
void NetStreamObject::AS3_multicastWindowDuration_set(double multicastWindowDuration)
{

}
Stringp NetStreamObject::AS3_nearNonce_get()
{
	return NULL;
}
ArrayObject* NetStreamObject::AS3_peerStreams_get()
{
	return NULL;
}
bool NetStreamObject::AS3_videoReliable_get()
{
	return false;
}
void NetStreamObject::AS3_videoReliable_set(bool videoReliable)
{

}
bool NetStreamObject::AS3_videoSampleAccess_get()
{
	return false;
}
void NetStreamObject::AS3_videoSampleAccess_set(bool videoSampleAccess)
{

}

void NetStreamObject::AS3_appendBytes(ByteArrayObject *pBytes)
{

}
void NetStreamObject::AS3_appendBytesAction(Stringp pNetStreamAppendBytesAction)
{

}
void NetStreamObject::AS3_attach(NetConnectionObject *pConnection)
{

}
bool NetStreamObject::AS3_onPeerConnect(NetStreamObject *pSubscriber)
{
	return false;
}
void NetStreamObject::AS3_play2(NetStreamPlayOptionsObject *pParam)
{

}
void NetStreamObject::AS3_preloadEmbeddedData(NetStreamPlayOptionsObject *pParam)
{

}
void NetStreamObject::AS3_receiveVideoFPS(double FPS)
{

}
void NetStreamObject::AS3_setDRMAuthenticationCredentials(Stringp pUserName, Stringp pPassword, Stringp pType)
{

}
void NetStreamObject::AS3_step(int frames)
{

}
//new adds end

//new adds 11
bool NetStreamObject::AS3_useHardwareDecoder_get()
{
	//LOGWHERE();
	return m_bUseHardwareDecoder;//false;
}
void NetStreamObject::AS3_useHardwareDecoder_set(bool b)
{
	//LOGWHERE();
	m_bUseHardwareDecoder = b;
}

bool NetStreamObject::AS3_useJitterBuffer_get()
{
	//LOGWHERE();
	return m_bUseJitterBuffer;//false;
}
void NetStreamObject::AS3_useJitterBuffer_set(bool b)
{

	//LOGWHERE();
	m_bUseJitterBuffer = b;
}

VideoStreamSettingsObject* NetStreamObject::AS3_videoStreamSettings_get()
{
	//LOGWHERE();
	return m_pSettings;
}
void NetStreamObject::AS3_videoStreamSettings_set(VideoStreamSettingsObject* pSettings)
{
	//LOGWHERE();
	m_pSettings = pSettings;
}

void NetStreamObject::AS3_dispose()
{
	LOGWHERE();

}
 
 //new adds 11 end
void NetStreamObject::WaveIn(void*pData,int l)
{//Capture Audio
}

}