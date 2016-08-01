#ifndef _AS3SoundCodec_
#define _AS3SoundCodec_
namespace avmplus{namespace NativeID{
class SoundCodecClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//NELLYMOSER : String = "NellyMoser"
	//SPEEX : String = "Speex"

	Stringp NELLYMOSER;
	Stringp SPEEX;
	Stringp	PCMA;
	Stringp	PCMU;

private:};
class SoundCodecObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SoundCodecClass : public ClassClosure
	{
	public:
		SoundCodecClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundCodecClassSlots m_slots_SoundCodecClass;
};
class SoundCodecObject : public ScriptObject
{
	public:
		SoundCodecObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SoundCodecObjectSlots m_slots_SoundCodecObject;
};}
#endif