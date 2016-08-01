#ifndef _AS3CameraPosition_
#define _AS3CameraPosition_

namespace avmplus
{
	namespace NativeID
	{
		class CameraPositionClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			// BACK:String = "back"
			// FRONT:String = "front"
			// UNKNOWN:String = "unknown"

			Stringp BACK;
			Stringp FRONT;
			Stringp UNKNOWN;


		private:
		};
		class CameraPositionObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class CameraPositionClass : public ClassClosure//EventClass
	{
	public:
		CameraPositionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:

		inline Stringp getSlotBACK(){return m_slots_CameraPositionClass.BACK;}
		inline Stringp getSlotFRONT(){return m_slots_CameraPositionClass.FRONT;}
		inline Stringp getSlotUNKNOWN(){return m_slots_CameraPositionClass.UNKNOWN;}
		

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraPositionClassSlots m_slots_CameraPositionClass;
	};
	class CameraPositionObject : public ScriptObject
	{
	public:
		CameraPositionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::CameraPositionObjectSlots m_slots_CameraPositionObject;
	};
}
#endif