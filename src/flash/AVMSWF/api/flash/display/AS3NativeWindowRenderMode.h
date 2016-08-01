#ifndef _AS3NativeWindowRenderMode_
#define _AS3NativeWindowRenderMode_
namespace avmplus
{
	namespace NativeID
	{
		class NativeWindowRenderModeClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			Stringp AUTO;
			Stringp CPU;
			Stringp DIRECT;
			Stringp GPU;
		private:
		};
		class NativeWindowRenderModeObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your MEMBER AS3 slots here!!!	
		private:
		};
	}
}

namespace avmshell
{
	class NativeWindowRenderModeClass: public ClassClosure
	{
	public:
		NativeWindowRenderModeClass(VTable* vtable);
		ScriptObject* createInstance(VTable* ivtable, ScriptObject* delegate);

		inline Stringp getSlotAUTO() {return m_slots_NativeWindowRenderModeClass.AUTO;}
		inline Stringp getSlotCPU() {return m_slots_NativeWindowRenderModeClass.CPU;}
		inline Stringp getSlotDIRECT() {return m_slots_NativeWindowRenderModeClass.DIRECT;}
		inline Stringp getSlotGPU() {return m_slots_NativeWindowRenderModeClass.GPU;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowRenderModeClassSlots m_slots_NativeWindowRenderModeClass;

	};

	class NativeWindowRenderModeObject: public ScriptObject
	{
	public:
		NativeWindowRenderModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData; //Set your Data!!!

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NativeWindowRenderModeObjectSlots m_slots_NativeWindowRenderModeObject;
	};
}


#endif //_AS3NativeWindowRenderMode_