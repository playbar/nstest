#ifndef _AS3GameInputDevice_
#define _AS3GameInputDevice_

namespace avmplus
{
	namespace NativeID
	{
		class GameInputDeviceClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public://Declare your STATIC AS3 slots here!!!
			//MAX_BUFFER_SIZE:int = 32000

			Stringp MAX_BUFFER_SIZE;


		private:
		};
		class GameInputDeviceObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!
		private:
		};
	}
}
namespace avmshell{
	class GameInputControlObject;
	class ByteArrayObject;
	class StringpVectorObject;
	class GameInputDeviceClass : public ClassClosure//EventClass
	{
	public:
		GameInputDeviceClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

	public:



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GameInputDeviceClassSlots m_slots_GameInputDeviceClass;
	};
	class GameInputDeviceObject : public ScriptObject
	{
	public:
		GameInputDeviceObject(VTable* _vtable, ScriptObject* _delegate, int capacity);

	public:
		bool m_bEnabled;
		Stringp m_strID;
		Stringp m_strName;
		int m_numControls;
		int m_sampleInterval;

		bool AS3_enabled_get();
		void AS3_enabled_set(bool bEnabled);
	    
		Stringp AS3_id_get();
	    
		Stringp AS3_name_get();
	    
		int AS3_numControls_get();
	    
		int AS3_sampleInterval_get();
		void AS3_sampleInterval_set(int nSam);
		
		
		int AS3_getCachedSamples(ByteArrayObject* data, bool append = false);
		
		GameInputControlObject* AS3_getControlAt(int i);
		
		void AS3_startCachingSamples(int numSamples, StringpVectorObject* controls);
		
		void AS3_stopCachingSamples();



	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::GameInputDeviceObjectSlots m_slots_GameInputDeviceObject;
	};
}
#endif