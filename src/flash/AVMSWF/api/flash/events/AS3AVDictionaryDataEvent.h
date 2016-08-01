#ifndef _AS3AVDictionaryDataEvent_
#define _AS3AVDictionaryDataEvent_

#include "AS3Event.h"
#include "AS3Dictionary.h"

namespace avmplus
{
	namespace NativeID
	{
		class AVDictionaryDataEventClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!

			//AV_DICTIONARY_DATA:String = "avDictionaryData"

			Stringp AV_DICTIONARY_DATA;

		private:
		};
		class AVDictionaryDataEventObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
			avmshell::DictionaryObject* m_dictionary;
			double            m_time;
		private:
		};
	}
}
namespace avmshell{
	class AVDictionaryDataEventClass : public ClassClosure//EventClass
	{
	public:
		AVDictionaryDataEventClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AVDictionaryDataEventClassSlots m_slots_AVDictionaryDataEventClass;
		inline Stringp getSlotAV_DICTIONARY_DATA(){return m_slots_AVDictionaryDataEventClass.AV_DICTIONARY_DATA;}
};
	class AVDictionaryDataEventObject : public EventObject
	{
	public:
		AVDictionaryDataEventObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		//DictionaryObject* AS3_dictionary_get()
		//{
		//	return m_slots_AVDictionaryDataEventObject.m_dictionary;
		//}
		//double AS3_time_get()
		//{
		//	return m_slots_AVDictionaryDataEventObject.m_time;
		//}

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::AVDictionaryDataEventObjectSlots m_slots_AVDictionaryDataEventObject;
	};
}
#endif