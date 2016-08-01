#ifndef _AS3Capabilities_
#define _AS3Capabilities_
namespace avmplus{namespace NativeID{
class CapabilitiesClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class CapabilitiesObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CapabilitiesClass : public ClassClosure
	{
	public:
		static bool avHardwareDisable;//[static] [read-only] ָ�����û�������ͷ����˷�ķ������Ѿ�ͨ����?ʽ��ֹ (true) �������� (false)��
		static bool hasAccessibility;//[read-only] ָ������������֧�� (true) �븨���ܽ���ͨ�ŵĻ��������У������ڲ�֧�� (false) ���ֹ��ܵĻ��������С�
 	 	static bool hasAudio;//[read-only] ָ���������Ƿ��ھ�����Ƶ���ܵ�ϵͳ�����С�
 	 	static bool hasAudioEncoder;//[read-only] ָ���������� (true) ���ǲ��� (false) ����Ƶ������������˷����Ƶ�������б��롣
 	 	static bool hasEmbeddedVideo;//[read-only] ָ������������֧�� (true) Ƕ����Ƶ��ϵͳ�����У������ڲ�֧�� (false) Ƕ����Ƶ��ϵͳ�����С�
		static bool hasIME;//[read-only] ָ�����������ڰ�װ�� (true) ���뷨�༭�� (IME) ��ϵͳ�����У�������δ��װ (false) IME ��ϵͳ�����С�
		static bool hasMP3;//[read-only] ָ�����������ھ��� (true) MP3 ��������ϵͳ�����У�������û�� (false) MP3 ��������ϵͳ�����С�
		static bool hasPrinting;//[static] [read-only] ָ������������֧�� (true) ��ӡ��ϵͳ�����У������ڲ�֧�� (false) ��ӡ��ϵͳ�����С�
 	 	static bool hasScreenBroadcast;//[static] [read-only] ָ����������֧�� (true) ����ͨ�� Flash Media Server ���е���Ļ�㲥Ӧ�ó��򣬻��ǲ�֧�� (false) ��������Ӧ�ó���
		static bool hasScreenPlayback;// : Boolean[static] [read-only] ָ����������֧�� (true) ͨ�� Flash Media Server ���е���Ļ�㲥Ӧ�ó���Ļطţ����ǲ�֧�� (false) ����Ӧ�ó���Ļطš�
		static bool hasStreamingAudio;// [read-only] ָ���������� (true) ���ǲ��� (false) ������Ƶ����
		static bool hasStreamingVideo;//[read-only] ָ���������� (true) ���ǲ��� (false) ������Ƶ����
		static bool hasTLS;//[static] [read-only] ָ���������� (true) �� (false) λ��ͨ�� NetConnection ֧�ֱ��� SSL �׽��ֵ�ϵͳ�ϡ�
		static bool hasVideoEncoder;//[read-only] ָ���������� (true) ���ǲ��� (false) ����Ƶ���������� Web ����ͷ����Ƶ�������б��롣
		static bool isDebugger;// : Boolean[static] [read-only] ָ��������������ĵ��԰汾 (true)��������ʽ�����İ汾 (false)��
	 	static const char*  language;//[static] [read-only] ָ�����в�������ϵͳ�����Դ��롣
 	 	static bool localFileReadDisable;//[static] [read-only] ָ�����û�Ӳ�̵Ķ�ȡȨ�����Ѿ�ͨ����?ʽ��ֹ (true) �������� (false)��
		static const char* manufacturer;//[static] [read-only] ָ�� Flash Player �������̣����ʽΪ��Adobe OSName����
		static const char* os;//[static] [read-only] ָ����ǰ�Ĳ���ϵͳ��
		static double pixelAspectRatio;//[static] [read-only] ָ����Ļ�����ظ߿�ȡ�
		static const char* playerType;
		static const char* screenColor;//[read-only] ָ����Ļ����ɫ��
		static double screenDPI;
		static double screenResolutionX;
		static double screenResolutionY;// : Number
		//static const char* serverString;
		static const char* version;

		//new adds
		static const char *cpuArchitecture;
		static const char *maxLevelIDC;
		//new adds end

		//new adds
		static bool m_isEmbeddedInAcrobat;
		static bool m_supports32BitProcesses;
		static bool m_supports64BitProcesses;
		//new adds end

		//new adds
		DRCWB(Stringp) m_pCpuArchitecture;
		DRCWB(ArrayObject*) m_pLanguages;
		DRCWB(Stringp) m_pMaxLevelIDC;
		DRCWB(Stringp) m_pTouchscreenType;
		//new adds end

	public:
		DRCWB(Stringp) kLanguage;
		DRCWB(Stringp) kManufacturer;
		DRCWB(Stringp) kOs;
		DRCWB(Stringp) kPlayerType;
		DRCWB(Stringp) kServerString;
		DRCWB(Stringp) kVersion;
		DRCWB(Stringp) kScreenColor;
	public:
		CapabilitiesClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		void InitDatas();
		void MakeString();
	public:
		bool AS3_avHardwareDisable_get(){return avHardwareDisable;}
		bool AS3_hasAccessibility_get(){return hasAccessibility;}
		bool AS3_hasAudio_get(){return hasAudio;}
		bool AS3_hasAudioEncoder_get(){return hasAudioEncoder;}
		bool AS3_hasEmbeddedVideo_get(){return hasEmbeddedVideo;}
		bool AS3_hasIME_get(){return hasIME;}
		bool AS3_hasMP3_get(){return hasMP3;}
		bool AS3_hasPrinting_get(){return hasPrinting;}
		bool AS3_hasScreenBroadcast_get(){return hasScreenBroadcast;}
		bool AS3_hasScreenPlayback_get(){return hasScreenPlayback;}
		bool AS3_hasStreamingAudio_get(){return hasStreamingAudio;}
		bool AS3_hasStreamingVideo_get(){return hasStreamingVideo;}
		bool AS3_hasTLS_get(){return hasTLS;}
		bool AS3_hasVideoEncoder_get(){return hasVideoEncoder;}
		bool AS3_isDebugger_get(){return isDebugger;}
		Stringp AS3_language_get(){return kLanguage;}
		bool AS3_localFileReadDisable_get(){return localFileReadDisable;}
		Stringp AS3_manufacturer_get(){return kManufacturer;}
		Stringp AS3_os_get(){return kOs;}
		double AS3_pixelAspectRatio_get(){return pixelAspectRatio;}
		Stringp AS3_playerType_get(){return kPlayerType;}
		Stringp AS3_screenColor_get(){return kScreenColor;}
		double AS3_screenDPI_get(){return screenDPI;}
		double AS3_screenResolutionX_get(){return screenResolutionX;}
		double AS3_screenResolutionY_get(){return screenResolutionY;}
		Stringp AS3_serverString_get(){return kServerString;}
		Stringp AS3_version_get(){return kVersion;}

		//new adds
		Stringp AS3_cpuArchitecture_get();
		bool AS3_isEmbeddedInAcrobat_get();
		ArrayObject* AS3_languages_get();
		Stringp AS3_maxLevelIDC_get();
		bool AS3_supports32BitProcesses_get();
		bool AS3_supports64BitProcesses_get();
		Stringp AS3_touchscreenType_get();
		//new adds end

		//new adds 11
		bool AS3_hasMultiChannelAudio(Stringp type);
		//new adds 11 end

	public:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CapabilitiesClassSlots m_slots_CapabilitiesClass;
};
class CapabilitiesObject : public ScriptObject
{
	public:
		CapabilitiesObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
				
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CapabilitiesObjectSlots m_slots_CapabilitiesObject;
};}
#endif
