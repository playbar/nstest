#ifndef _AS3DRMManagerSession_
#define _AS3DRMManagerSession_

#include "AS3EventDispatcher.h"

namespace avmplus{namespace NativeID{
class DRMContentDataObject;
class TimerObject;

class DRMManagerSessionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	uint32_t STATUS_READY;
	uint32_t STATUS_NOTREADY;
	uint32_t STATUS_FAILED;
	uint32_t STATUS_UNKNOWN;
private:};
class DRMManagerSessionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	DRMContentDataObject	*m_metadata;
	TimerObject	*m_checkStatusTimer;
	XBOOL	m_isInSession;
private:};
}}
namespace avmshell{
	class DRMManagerSessionClass : public ClassClosure//EventDispatcherClass
	{
	public:
		DRMManagerSessionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMManagerSessionClassSlots m_slots_DRMManagerSessionClass;
};
class DRMManagerSessionObject : public EventDispatcherObject
{
	public:
		DRMManagerSessionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		uint32_t AS3_getLastError();
		uint32_t AS3_getLastSubErrorID();
		void	 AS3_issueDRMErrorEvent(DRMContentDataObject *pMetadata, int errorID, int subErrorID, Stringp eventType);
		void	 AS3_errorCodeToThrow(uint32_t errorCode);

		void	AS3_issueDRMStatusEventInner(Stringp specificEventType, DRMContentDataObject *pMetadata, DRMVoucherObject *pVoucher, bool isLocal){}
		uint32_t	AS3_checkStatusInner(){return 0;}
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMManagerSessionObjectSlots m_slots_DRMManagerSessionObject;
};}
#endif