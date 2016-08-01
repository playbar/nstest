#ifndef _AS3H264Level_
#define _AS3H264Level_

namespace avmplus
{
	namespace NativeID
	{
		class H264LevelClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			//LEVEL_1:String = "1"
			//LEVEL_1_1:String = "1.1"
			//LEVEL_1_2:String = "1.2"
			//LEVEL_1_3:String = "1.3"
			//LEVEL_1B:String = "1b"
			//LEVEL_2:String = "2"
			//LEVEL_2_1:String = "2.1"
			//LEVEL_2_2:String = "2.2"
			//LEVEL_3:String = "3"
			//LEVEL_3_1:String = "3.1"
			//LEVEL_3_2:String = "3.2"
			//LEVEL_4:String = "4"
			//LEVEL_4_1:String = "4.1"
			//LEVEL_4_2:String = "4.2"
			//LEVEL_5:String = "5"
			//LEVEL_5_1:String = "5.1"

			Stringp LEVEL_1;
			Stringp LEVEL_1_1;
			Stringp LEVEL_1_2;
			Stringp LEVEL_1_3;
			Stringp LEVEL_1B;
			Stringp LEVEL_2;
			Stringp LEVEL_2_1;
			Stringp LEVEL_2_2;
			Stringp LEVEL_3;
			Stringp LEVEL_3_1;
			Stringp LEVEL_3_2;
			Stringp LEVEL_4;
			Stringp LEVEL_4_1;
			Stringp LEVEL_4_2;
			Stringp LEVEL_5;
			Stringp LEVEL_5_1;

		private:
		};
		class H264LevelObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class H264LevelClass : public ClassClosure//EventClass
	{
	public:
		H264LevelClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:

		inline Stringp getSlotLEVEL_1() {return m_slots_H264LevelClass.LEVEL_1;}
		inline Stringp getSlotLEVEL_1_1() {return m_slots_H264LevelClass.LEVEL_1_1;}
		inline Stringp getSlotLEVEL_1_2(){return m_slots_H264LevelClass.LEVEL_1_2;}
		inline Stringp getSlotLEVEL_1_3(){return m_slots_H264LevelClass.LEVEL_1_3;}
		inline Stringp getSlotLEVEL_1B(){return m_slots_H264LevelClass.LEVEL_1B;}
		inline Stringp getSlotLEVEL_2(){return m_slots_H264LevelClass.LEVEL_2;}
		inline Stringp getSlotLEVEL_2_1(){return m_slots_H264LevelClass.LEVEL_2_1;}
		inline Stringp getSlotLEVEL_2_2(){return m_slots_H264LevelClass.LEVEL_2_2;}
		inline Stringp getSlotLEVEL_3(){return m_slots_H264LevelClass.LEVEL_3;}
		inline Stringp getSlotLEVEL_3_1(){return m_slots_H264LevelClass.LEVEL_3_1;}
		inline Stringp getSlotLEVEL_3_2(){return m_slots_H264LevelClass.LEVEL_3_2;}
		inline Stringp getSlotLEVEL_4(){return m_slots_H264LevelClass.LEVEL_4;}
		inline Stringp getSlotLEVEL_4_1(){return m_slots_H264LevelClass.LEVEL_4_1;}
		inline Stringp getSlotLEVEL_4_2(){return m_slots_H264LevelClass.LEVEL_4_2;}
		inline Stringp getSlotLEVEL_5(){return m_slots_H264LevelClass.LEVEL_5;}
		inline Stringp getSlotLEVEL_5_1(){return m_slots_H264LevelClass.LEVEL_5_1;}


	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::H264LevelClassSlots m_slots_H264LevelClass;
	};
	class H264LevelObject : public ScriptObject
	{
	public:
		H264LevelObject(VTable* _vtable, ScriptObject* _delegate, int capacity);




	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::H264LevelObjectSlots m_slots_H264LevelObject;
	};
}
#endif