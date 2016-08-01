#ifndef _AS3DRMErrorEvent_
#define _AS3DRMErrorEvent_

#include "AS3Event.h"
#include "AS3ErrorEvent.h"
#include "AS3DRMContentData.h"

namespace avmplus{namespace NativeID{

class DRMContentDataObject;

class DRMErrorEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//DRM_ERROR : String = "drmError"

	Stringp DRM_ERROR;
	Stringp DRM_LOAD_DEVICEID_ERROR;
private:};
class DRMErrorEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	DRMContentDataObject *m_metadata;
	int32_t m_subErrorID;
	XBOOL m_systemUpdateNeeded;
	XBOOL m_drmUpdateNeeded;
private:};
}}
namespace avmshell{
	class DRMErrorEventClass : public ClassClosure//ErrorEventClass
	{
	public:
		DRMErrorEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMErrorEventClassSlots m_slots_DRMErrorEventClass;
};
class DRMErrorEventObject : public ErrorEventObject
{
	public:
		DRMErrorEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//void* pData;//Set your data!!
		//DRMContentDataObject* AS3_contentData_get();
		//void AS3_contentData_set(DRMContentDataObject *pContentData);
		//bool AS3_drmUpdateNeeded_get();
		//int AS3_subErrorID_get();
		//bool AS3_systemUpdateNeeded_get();

		//void AS3_constructor(Stringp pType, bool bubbles, bool cancelable, Stringp pInErrorDetail, 
		//	int inErrorCode, int insubErrorID, DRMContentDataObject *pInMetadata, 
		//	bool inSystemUpdateNeeded, bool inDrmUpdateNeeded);
		//EventObject* AS3_clone();
		//Stringp AS3_toString();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMErrorEventObjectSlots m_slots_DRMErrorEventObject;
};}
#endif