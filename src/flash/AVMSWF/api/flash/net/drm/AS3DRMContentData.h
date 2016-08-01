#ifndef _AS3DRMContentData_
#define _AS3DRMContentData_
namespace avmplus{namespace NativeID{
class ObjectVectorObject;
class ByteArrayObject;

class DRMContentDataClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	ByteArrayObject	*m_internalOnly;
private:};
class DRMContentDataObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Stringp	m_licenseID;
	Stringp	m_domain;
	ObjectVectorObject	*m_voucherAccessInfo;
private:};
}}
namespace avmshell{
class ByteArrayObject;

	class DRMContentDataClass : public ClassClosure
	{
	public:
		DRMContentDataClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMContentDataClassSlots m_slots_DRMContentDataClass;
};
class DRMContentDataObject : public ScriptObject
{
	public:
		DRMContentDataObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		Stringp		AS3_FMRMSURL_get(){return NULL;}
		Stringp		AS3_authenticationMethod_get(){return NULL;}
		Stringp		AS3_getLicenseIDInner(){return NULL;}
		Stringp		AS3_getDomainInner(){return NULL;}
		uint32_t	AS3_setRawMetadataInner(ByteArrayObject *pInRawData){return 0;}
		void		AS3_errorCodeToThrow(uint32_t errorCode){}
		void		AS3_populateVoucherAccessInfo(){}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMContentDataObjectSlots m_slots_DRMContentDataObject;
};}
#endif