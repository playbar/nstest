#ifndef _AS3H264VideoStreamSettings_
#define _AS3H264VideoStreamSettings_

#include "AS3H264Level.h"
#include "AS3H264Profile.h"
#include "AS3VideoCodec.h"
#include "AS3VideoStreamSettings.h"

namespace avmplus
{
	namespace NativeID
	{
		class H264VideoStreamSettingsClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!


		private:
		};
		class H264VideoStreamSettingsObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class H264VideoStreamSettingsClass : public ClassClosure//EventClass
	{
	public:
		H264VideoStreamSettingsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::H264VideoStreamSettingsClassSlots m_slots_H264VideoStreamSettingsClass;
	};
	class H264VideoStreamSettingsObject : public VideoStreamSettingsObject
	{
	public:
		H264VideoStreamSettingsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

		DRCWB(Stringp) m_strCodec;
		DRCWB(Stringp) m_strLevel;
		DRCWB(Stringp) m_strProfile;
		Stringp AS3_codec_get();
		
		Stringp AS3_level_get();
		
		Stringp AS3_profile_get();

		void AS3_setProfileLevel(Stringp profile, Stringp level);


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::H264VideoStreamSettingsObjectSlots m_slots_H264VideoStreamSettingsObject;
	};
}
#endif