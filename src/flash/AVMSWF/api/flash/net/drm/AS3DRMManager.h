#ifndef _AS3DRMManager_
#define _AS3DRMManager_
namespace avmplus{namespace NativeID{
class DRMManagerClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DRMManagerObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DRMManagerObject;
	class DRMDeviceGroupObject;
	class DRMManagerClass : public ClassClosure
	{
	public:
		DRMManagerClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		DRMManagerObject*	AS3_createDRMManager(){return NULL;}
		bool	AS3_isCalledFromRemoteSWF(){return false;}

		//new adds 11
		double m_nNetWorkIdleTimeout;
        double AS3_networkIdleTimeout_get();
		void AS3_networkIdleTimeout_set(double d);
		
        //new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMManagerClassSlots m_slots_DRMManagerClass;
};
class DRMManagerObject : public ScriptObject
{
	public:
		DRMManagerObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		uint32_t	AS3_setSAMLTokenInner(Stringp serverUrl, Stringp domain, ByteArrayObject *pToken){return 0;}
		void		AS3_errorCodeToThrow(uint32_t errorCode){}
		void		AS3_issueDRMStatusEvent(Stringp specificEventType, DRMContentDataObject *pMetadata, DRMVoucherObject *pVoucher, bool isLocal){}
		void		AS3_issueDRMErrorEvent(DRMContentDataObject *pMetadata, int errorID, int subErrorID, Stringp eventType){}
		uint32_t	AS3_storeVoucherInner(ByteArrayObject *pVoucher){return 0;}

		//new adds 11		
		void AS3_addToDeviceGroup(DRMDeviceGroupObject* pDeviceGroup, bool forceRefresh = false);
		void AS3_removeFromDeviceGroup(DRMDeviceGroupObject* pDeviceGroup);
		void AS3_resetDRMVouchers();
		void AS3_returnVoucher(Stringp inServerURL, bool immediateCommit, Stringp licenseID, Stringp policyID);
        void AS3_storeVoucher(ByteArrayObject* pVoucher);
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMManagerObjectSlots m_slots_DRMManagerObject;
};}
#endif