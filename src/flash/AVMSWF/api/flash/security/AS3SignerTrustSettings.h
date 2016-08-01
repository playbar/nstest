#ifndef _AS3SignerTrustSettings_
#define _AS3SignerTrustSettings_
namespace avmplus{namespace NativeID{
class SignerTrustSettingsClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//CODE_SIGNING : String = "codeSigning"
//PLAYLIST_SIGNING : String = "playlistSigning"
//SIGNING : String = "signing"

	Stringp CODE_SIGNING;
	Stringp PLAYLIST_SIGNING;
	Stringp SIGNING;

private:};
class SignerTrustSettingsObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class SignerTrustSettingsClass : public ClassClosure
	{
	public:
		SignerTrustSettingsClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SignerTrustSettingsClassSlots m_slots_SignerTrustSettingsClass;
};
class SignerTrustSettingsObject : public ScriptObject
{
	public:
		SignerTrustSettingsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::SignerTrustSettingsObjectSlots m_slots_SignerTrustSettingsObject;
};}
#endif