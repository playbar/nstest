#ifndef _AS3PNGEncoderOptions_
#define _AS3PNGEncoderOptions_

namespace avmplus
{
	namespace NativeID
	{
		class PNGEncoderOptionsClassSlots
		{
			friend class SlotOffsetsAndAsserts;

		public:

		private:

		};
		class PNGEncoderOptionsObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			
		private:

		};
	}
}

namespace avmshell
{
	class PNGEncoderOptionsClass : public ClassClosure
	{
	public:
		PNGEncoderOptionsClass (VTable* vtable);
		ScriptObject* createInstance( VTable* ivtable, ScriptObject* delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PNGEncoderOptionsClassSlots m_slots_PNGEncoderOptionsClass;
	};
	
	class PNGEncoderOptionsObject : public ScriptObject
	{
	public:
		PNGEncoderOptionsObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	
		bool m_fastCompression;
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::PNGEncoderOptionsObjectSlots m_slots_PNGEncoderOptionsObject;
	};
}

#endif // _AS3PNGEncoderOptions_