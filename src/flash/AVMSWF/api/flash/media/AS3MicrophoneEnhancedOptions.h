#ifndef _AS3MicrophoneEnhancedOptions_
#define _AS3MicrophoneEnhancedOptions_
namespace avmplus{namespace NativeID{
class MicrophoneEnhancedOptionsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class MicrophoneEnhancedOptionsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

	Stringp	m_mode;
	int32_t	m_echoPath;
	XBOOL	m_nlp;
	int32_t	m_isVoiceDetected;
private:};
}}
namespace avmshell{
	class MicrophoneEnhancedOptionsClass : public ClassClosure
	{
	public:
		MicrophoneEnhancedOptionsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MicrophoneEnhancedOptionsClassSlots m_slots_MicrophoneEnhancedOptionsClass;
};
class MicrophoneEnhancedOptionsObject : public ScriptObject
{
	public:
		MicrophoneEnhancedOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//int AS3_echoPath_get();
		//void AS3_echoPath_set(int echoPath);
		//int AS3_isVoiceDetected_get();
		//Stringp AS3_mode_get();
		//void AS3_mode_set(Stringp pMode);
		//bool AS3_nonLinearProcessing_get();
		//void AS3_nonLinearProcessing_set(bool pNonLinearProcessing);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::MicrophoneEnhancedOptionsObjectSlots m_slots_MicrophoneEnhancedOptionsObject;
};}
#endif