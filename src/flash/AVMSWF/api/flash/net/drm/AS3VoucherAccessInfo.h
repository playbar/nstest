#ifndef _AS3VoucherAccessInfo_
#define _AS3VoucherAccessInfo_
#include "AS3DRMDeviceGroup.h"
namespace avmplus{namespace NativeID{
class VoucherAccessInfoClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class VoucherAccessInfoObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp	m_displayName;
	Stringp	m_authMethod;
	Stringp	m_domain;
	DRCWB(avmshell::DRMDeviceGroupObject*) m_deviceGroup;
	Stringp m_policyID;
private:};
}}
namespace avmshell{
	class DRMDeviceGroupObject;
	class VoucherAccessInfoClass : public ClassClosure
	{
	public:
		VoucherAccessInfoClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VoucherAccessInfoClassSlots m_slots_VoucherAccessInfoClass;
};
class VoucherAccessInfoObject : public ScriptObject
{
	public:
		VoucherAccessInfoObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//AvmBox AS3_authenticationMethod_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_displayName_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_domain_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds 11
		//DRCWB(DRMDeviceGroupObject*) m_pDeviceGroup;
		//DRMDeviceGroupObject* AS3_deviceGroup_get(){
		//	//LOGWHERE();
		//	return m_pDeviceGroup;
		//}
		//Stringp m_strPolicyID;
		//Stringp AS3_policyID_get(){
		//	//LOGWHERE();
		//	return m_strPolicyID;
		//}
        //new adds 11 end 
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::VoucherAccessInfoObjectSlots m_slots_VoucherAccessInfoObject;
};}
#endif