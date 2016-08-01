#ifndef _AS3VideoStreamSettings_
#define _AS3VideoStreamSettings_
#include "AS3VideoCodec.h"
#include "AS3Camera.h"
namespace avmplus
{
	namespace NativeID
	{
		class VideoStreamSettingsClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!


		private:
		};
		class VideoStreamSettingsObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class VideoStreamSettingsClass : public ClassClosure//EventClass
	{
	public:
		VideoStreamSettingsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoStreamSettingsClassSlots m_slots_VideoStreamSettingsClass;
	};
	class VideoStreamSettingsObject : public ScriptObject
	{
	public:
		VideoStreamSettingsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

		int m_iBandwidth;
		Stringp m_strCodec;
		double m_nFps;
		int m_iHeight,m_iKeyFrameInterval,m_iQuality,m_iWidth;
		DRCWB(CameraObject*) m_pCamera;
		inline CameraObject* GetCamera();
		int AS3_bandwidth_get();
		
		Stringp AS3_codec_get();
		
		double AS3_fps_get();
		
		int AS3_height_get();
		
		int AS3_keyFrameInterval_get();
		
		int AS3_quality_get();
		
		int AS3_width_get();
		
		
		void AS3_setKeyFrameInterval(int keyFrameInterval);
		void AS3_setMode(int width, int height, double fps);
		void AS3_setQuality(int bandwidth, int quality);



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VideoStreamSettingsObjectSlots m_slots_VideoStreamSettingsObject;
	};
}
#endif