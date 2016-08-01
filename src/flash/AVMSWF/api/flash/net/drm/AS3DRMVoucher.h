#ifndef _AS3DRMVoucher_
#define _AS3DRMVoucher_

#include "AS3ByteArray.h"
#include "AS3DRMPlaybackTimeWindow.h"

namespace avmplus{namespace NativeID{
class DRMPlaybackTimeWindowObject;

class DRMVoucherClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class DRMVoucherObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	DateObject	*m_endDate;
	DateObject	*m_startDate;
	DateObject	*m_offlineLeaseStartDate;
	DateObject	*m_offlineLeaseExpirationDate;
	ScriptObject	*m_customPolicies;
	DRMPlaybackTimeWindowObject	*m_playbackTimeWindow;
private:};
}}
namespace avmshell{
	class DRMVoucherClass : public ClassClosure
	{
	public:
		DRMVoucherClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMVoucherClassSlots m_slots_DRMVoucherClass;
};
class DRMVoucherObject : public ScriptObject
{
	public:
		DRMVoucherObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		double	AS3_getEndDateInner(){return 0.0;}
		double	AS3_getStartDateInner(){return 0.0;}
		double	AS3_getOfflineLeaseStartDateInner(){return 0.0;}
		double	AS3_getOfflineLeaseExpirationDateInner(){return 0.0;}
		void	AS3_createCustomPolicyObject(){}
		double	AS3_playStartTime_get(){return 0.0;}
		double	AS3_playbackEndTime_get(){return 0.0;}
		double	AS3_playbackWindow_get(){return 0.0;}
		DRMPlaybackTimeWindowObject*	AS3_createDRMPlaybackTimeWindow(double length, double start, double end);
		ByteArrayObject*	AS3_toByteArrayInner(){return NULL;}

		//new adds 11
		Stringp m_strLicenseID;
		Stringp AS3_licenseID_get(){ 
			//LOGWHERE();
			return m_strLicenseID;
		}

		Stringp m_strPolicyID;
		Stringp AS3_policyID_get(){ 
			//LOGWHERE();
			return m_strPolicyID; 
		}
        //new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMVoucherObjectSlots m_slots_DRMVoucherObject;
};}
#endif