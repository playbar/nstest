#ifndef _AS3X509Certificate_
#define _AS3X509Certificate_

#include "AS3ByteArray.h"
#include "AS3X500DistinguishedName.h"

namespace avmplus{namespace NativeID{
class X509CertificateClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class X509CertificateObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class X509CertificateClass : public ClassClosure
	{
	public:
		X509CertificateClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::X509CertificateClassSlots m_slots_X509CertificateClass;
};
class X509CertificateObject : public ScriptObject
{
	public:
		X509CertificateObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		uint32_t AS3_version_get();
		Stringp AS3_serialNumber_get();
		Stringp AS3_signatureAlgorithmOID_get();
		ByteArrayObject* AS3_signatureAlgorithmParams_get();
		X500DistinguishedNameObject* AS3_issuer_get();
		DateObject* AS3_validNotBefore_get();
		DateObject* AS3_validNotAfter_get();
		X500DistinguishedNameObject* AS3_subject_get();
		Stringp AS3_subjectPublicKeyAlgorithmOID_get();
		Stringp AS3_subjectPublicKey_get();
		Stringp AS3_issuerUniqueID_get();
		Stringp AS3_subjectUniqueID_get();
		ByteArrayObject* AS3_encoded_get();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::X509CertificateObjectSlots m_slots_X509CertificateObject;
};}
#endif