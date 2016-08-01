#ifndef _AS3MicrophoneEnhancedMode_
#define _AS3MicrophoneEnhancedMode_
namespace avmplus{namespace NativeID{
class MicrophoneEnhancedModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//FULL_DUPLEX : Stringp = "fullDuplex"
//HALF_DUPLEX : Stringp = "halfDuplex"
//HEADSET : Stringp = "headset"
//OFF : Stringp = "off"
//SPEAKER_MUTE : Stringp = "speakerMute"

	Stringp FULL_DUPLEX;
	Stringp HALF_DUPLEX;
	Stringp HEADSET;
	Stringp OFF;
	Stringp SPEAKER_MUTE;

private:};
class MicrophoneEnhancedModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class MicrophoneEnhancedModeClass : public ClassClosure
	{
	public:
		MicrophoneEnhancedModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MicrophoneEnhancedModeClassSlots m_slots_MicrophoneEnhancedModeClass;
};
class MicrophoneEnhancedModeObject : public ScriptObject
{
	public:
		MicrophoneEnhancedModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MicrophoneEnhancedModeObjectSlots m_slots_MicrophoneEnhancedModeObject;
};}
#endif