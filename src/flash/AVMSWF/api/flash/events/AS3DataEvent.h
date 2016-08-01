#ifndef _AS3DataEvent_
#define _AS3DataEvent_

#include "AS3TextEvent.h"

namespace avmplus{namespace NativeID{
class DataEventClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
//	Stringp DATA = "data";
//	Stringp UPLOAD_COMPLETE_DATA = "uploadCompleteData";

	Stringp DATA;
	Stringp UPLOAD_COMPLETE_DATA;
private:};
class DataEventObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class DataEventObject;

	class DataEventClass : public ClassClosure//TextEventClass
	{
	public:
		DataEventObject* CreateEventObject(int id, Stringp pData);
		Stringp		 IDToStringType(int id);
	public:
		DataEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DataEventClassSlots m_slots_DataEventClass;
	public:
		Stringp getSlotDATA(){return m_slots_DataEventClass.DATA;}
		Stringp getSlotUPLOAD_COMPLETE_DATA(){return m_slots_DataEventClass.UPLOAD_COMPLETE_DATA;}
};
class DataEventObject : public TextEventObject
{
	public:
//		DRCWB(Stringp) m_pData;
		Stringp m_pData;
	public:
		DataEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		virtual EventObject* Clone(EventObject *pObj);

		Stringp AS3_data_get();
		void AS3_data_set(Stringp pData);
		void AS3_constructor(Stringp type, bool bBubbles, bool bCancelable, Stringp pData);
		DataEventObject* AS3_clone();
		Stringp AS3_toString();
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::DataEventObjectSlots m_slots_DataEventObject;
};}
#endif