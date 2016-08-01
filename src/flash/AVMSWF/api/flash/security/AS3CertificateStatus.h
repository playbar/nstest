#ifndef _AS3CertificateStatus_
#define _AS3CertificateStatus_
namespace avmplus{namespace NativeID{
class CertificateStatusClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//EXPIRED : String = "expired"
//INVALID : String = "invalid"
//INVALID_CHAIN : String = "invalidChain"
//NOT_YET_VALID : String = "notYetValid"
//PRINCIPAL_MISMATCH : String = "principalMismatch"
//REVOKED : String = "revoked"
//TRUSTED : String = "trusted"
//UNKNOWN : String = "unknown"
//UNTRUSTED_SIGNERS : String = "untrustedSigners"

	Stringp EXPIRED;
	Stringp INVALID;
	Stringp INVALID_CHAIN;
	Stringp NOT_YET_VALID;
	Stringp PRINCIPAL_MISMATCH;
	Stringp REVOKED;
	Stringp TRUSTED;
	Stringp UNKNOWN;
	Stringp UNTRUSTED_SIGNERS;

private:};
class CertificateStatusObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class CertificateStatusClass : public ClassClosure
	{
	public:
		CertificateStatusClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CertificateStatusClassSlots m_slots_CertificateStatusClass;
};
class CertificateStatusObject : public ScriptObject
{
	public:
		CertificateStatusObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CertificateStatusObjectSlots m_slots_CertificateStatusObject;
};}
#endif