#ifndef _AS3JPEGEncoderOptions_
#define _AS3JPEGEncoderOptions_

namespace avmplus
{
	namespace NativeID
	{
		class JPEGEncoderOptionsClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			//Declare your STATIC AS3 slots here!!!
		private:

		};

		class JPEGEncoderOptionsObjectSlots
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
	class JPEGEncoderOptionsClass: public ClassClosure
	{
	public:
		JPEGEncoderOptionsClass(VTable* vtable);
		ScriptObject* createInstance(VTable* ivtable, ScriptObject* delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JPEGEncoderOptionsClassSlots m_slots_JPEGEncoderOptionsClass;
	};

	class JPEGEncoderOptionsObject: public ScriptObject
	{
	public:
		JPEGEncoderOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		uint32_t m_quality;  //1-100 Õº∆¨—πÀı∆∑÷ 
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JPEGEncoderOptionsObjectSlots m_slots_JPEGEncoderOptionsObject;
	};
}


#endif // _AS3JPEGEncoderOptions_
