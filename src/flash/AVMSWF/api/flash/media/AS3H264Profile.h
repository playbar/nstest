#ifndef _AS3H264Profile_
#define _AS3H264Profile_

namespace avmplus
{
	namespace NativeID
	{
		class H264ProfileClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			// BASELINE:String = "baseline"
			// MAIN:String = "main"

			Stringp BASELINE;
			Stringp MAIN;


		private:
		};
		class H264ProfileObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class H264ProfileClass : public ClassClosure//EventClass
	{
	public:
		H264ProfileClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:
		inline Stringp getSlotBASELINE() {return m_slots_H264ProfileClass.BASELINE;}
		inline Stringp getSlotMAIN() {return m_slots_H264ProfileClass.MAIN;}

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::H264ProfileClassSlots m_slots_H264ProfileClass;
	};
	class H264ProfileObject : public ScriptObject
	{
	public:
		H264ProfileObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::H264ProfileObjectSlots m_slots_H264ProfileObject;
	};
}
#endif