#ifndef _AS3XMLSignatureValidator_
#define _AS3XMLSignatureValidator_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class XMLSignatureValidatorClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class XMLSignatureValidatorObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class XMLSignatureValidatorClass : public ClassClosure
	{
	public:
		XMLSignatureValidatorClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLSignatureValidatorClassSlots m_slots_XMLSignatureValidatorClass;
};
class XMLSignatureValidatorObject : public EventDispatcherObject
{
	public:
		XMLSignatureValidatorObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_isSupported_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_digestStatus_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_identityStatus_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_referencesStatus_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_referencesValidationSetting_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_referencesValidationSetting_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_revocationCheckSetting_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_revocationCheckSetting_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_signerCN_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_signerDN_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_signerExtendedKeyUsages_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_signerTrustSettings_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_uriDereferencer_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_uriDereferencer_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_useSystemTrustStore_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_useSystemTrustStore_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_validityStatus_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_addCertificate(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_verify(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::XMLSignatureValidatorObjectSlots m_slots_XMLSignatureValidatorObject;
};}
#endif