#ifndef _AS3DRMMetadataEvent_
#define _AS3DRMMetadataEvent_

#include "AS3Event.h"
#include "AS3DRMContentData.h"
#include "AS3ByteArray.h"

namespace avmplus
{
	namespace NativeID
	{
		class DRMMetadataEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//DRM_METADATA:String = "drmMetadata"

			Stringp DRM_METADATA;
		private:
		};
		class DRMMetadataEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!

		private:
		};
	}
}
namespace avmshell{
	class DRMMetadataEventClass : public ClassClosure//EventClass
	{
	public:
		DRMMetadataEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMMetadataEventClassSlots m_slots_DRMMetadataEventClass;
	};
	class DRMMetadataEventObject : public EventObject
	{
	public:
		DRMMetadataEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRMContentDataObject* AS3_drmMetadata_get();
		double AS3_timestamp_get();
	private:
		DRCWB(DRMContentDataObject*) m_drmMetadata;
		double m_timestamp;
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DRMMetadataEventObjectSlots m_slots_DRMMetadataEventObject;
	};
}
#endif