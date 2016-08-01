#ifndef _AS3NetStream_
#define _AS3NetStream_

#include "AS3EventDispatcher.h"
#include "AS3NetStreamInfo.h"
#include "AS3NetStreamMulticastInfo.h"
#include "AS3NetStreamPlayOptions.h"
#include "AS3ByteArray.h"
#include "AS3NetConnection.h"

namespace avmplus{namespace NativeID{
class NetStreamClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class NetStreamObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SoundTransformObject;
	class VideoStreamSettingsObject;
	class NetStreamClass : public ClassClosure
	{
	public:
		NetStreamClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		//new adds
		void AS3_resetDRMVouchers();
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamClassSlots m_slots_NetStreamClass;
};
class NetStreamObject : public EventDispatcherObject
{
	public:
		NetStreamObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		void WaveIn(void*pData,int l);
	public:
		DRCWB(SoundTransformObject*) m_pTrans;
		AvmBox AS3_bufferLength_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bufferTime_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bufferTime_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bytesLoaded_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_bytesTotal_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_checkPolicyFile_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_checkPolicyFile_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_client_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_client_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_currentFPS_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_liveDelay_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_objectEncoding_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		SoundTransformObject* AS3_soundTransform_get();
		void AS3_soundTransform_set(SoundTransformObject*);
		AvmBox AS3_time_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_attachAudio(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_attachCamera(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_close(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_pause(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_play(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_publish(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_receiveAudio(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_receiveVideo(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_resume(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_seek(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_send(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_togglePause(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds
		bool AS3_audioReliable_get();
		void AS3_audioReliable_set(bool audioReliable);
		bool AS3_audioSampleAccess_get();
		void AS3_audioSampleAccess_set(bool audioSampleAccess);
		double AS3_backBufferLength_get();
		double AS3_backBufferTime_get();
		void AS3_backBufferTime_set(double backBufferTime);
		double AS3_bufferTimeMax_get();
		void AS3_bufferTimeMax_set(double bufferTimeMax);
		bool AS3_dataReliable_get();
		void AS3_dataReliable_set(bool dataReliable);
		Stringp AS3_farID_get();
		Stringp AS3_farNonce_get();
		bool AS3_inBufferSeek_get();
		void AS3_inBufferSeek_set(bool inBufferSeek);
		NetStreamInfoObject* AS3_info_get();
		double AS3_maxPauseBufferTime_get();
		void AS3_maxPauseBufferTime_set(double maxPauseBufferTime);
		bool AS3_multicastAvailabilitySendToAll_get();
		void AS3_multicastAvailabilitySendToAll_set(bool multicastAvailabilitySendToAll);
		double AS3_multicastAvailabilityUpdatePeriod_get();
		void AS3_multicastAvailabilityUpdatePeriod_set(double multicastAvailabilityUpdatePeriod);
		double AS3_multicastFetchPeriod_get();
		void AS3_multicastFetchPeriod_set(double multicastFetchPeriod);
		NetStreamMulticastInfoObject* AS3_multicastInfo_get();
		double AS3_multicastPushNeighborLimit_get();
		void AS3_multicastPushNeighborLimit_set(double multicastPushNeighborLimit);
		double AS3_multicastRelayMarginDuration_get();
		void AS3_multicastRelayMarginDuration_set(double multicastRelayMarginDuration);
		double AS3_multicastWindowDuration_get();
		void AS3_multicastWindowDuration_set(double multicastWindowDuration);
		Stringp AS3_nearNonce_get();
		ArrayObject* AS3_peerStreams_get();
		bool AS3_videoReliable_get();
		void AS3_videoReliable_set(bool videoReliable);
		bool AS3_videoSampleAccess_get();
		void AS3_videoSampleAccess_set(bool videoSampleAccess);

		void AS3_appendBytes(ByteArrayObject *pBytes);
		void AS3_appendBytesAction(Stringp pNetStreamAppendBytesAction);
		void AS3_attach(NetConnectionObject *pConnection);
		bool AS3_onPeerConnect(NetStreamObject *pSubscriber);
		void AS3_play2(NetStreamPlayOptionsObject *pParam);
		void AS3_preloadEmbeddedData(NetStreamPlayOptionsObject *pParam);
		void AS3_receiveVideoFPS(double FPS);
		void AS3_setDRMAuthenticationCredentials(Stringp pUserName, Stringp pPassword, Stringp pType);
		void AS3_step(int frames);
		//new adds end

		//new adds 11
		bool m_bUseHardwareDecoder;
		bool AS3_useHardwareDecoder_get();
		void AS3_useHardwareDecoder_set(bool b);
		
		bool m_bUseJitterBuffer;
	    bool AS3_useJitterBuffer_get();
		void AS3_useJitterBuffer_set(bool b);
		
		DRCWB(VideoStreamSettingsObject*) m_pSettings;
	    VideoStreamSettingsObject* AS3_videoStreamSettings_get();
		void AS3_videoStreamSettings_set(VideoStreamSettingsObject* pSettings);

		void AS3_dispose();
		 
		 //new adds 11 end
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NetStreamObjectSlots m_slots_NetStreamObject;
};}
#endif