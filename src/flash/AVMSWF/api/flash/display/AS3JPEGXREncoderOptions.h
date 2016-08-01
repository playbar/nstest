#ifndef _AS3JPEGXREncoderOptions_
#define _AS3JPEGXREncoderOptions_

#include "AS3BitmapEncodingColorSpace.h"
namespace avmplus
{
	namespace NativeID
	{
		class JPEGXREncoderOptionsClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			////Declare your STATIC AS3 slots here!!!

		private:

		};
		class JPEGXREncoderOptionsObjectSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			////Declare your MEMBER AS3 slots here!!!

		private:

		};
	}
}

namespace avmshell
{
	class BitmapEncodingColorSpaceObject;
	class JPEGXREncoderOptionsClass : public ClassClosure
	{
	public:
		JPEGXREncoderOptionsClass(VTable* vtable);
		ScriptObject* createInstance(VTable* ivtable, ScriptObject* delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JPEGXREncoderOptionsClassSlots m_slots_JPEGXREncoderOptionsClass;
	};
	class JPEGXREncoderOptionsObject : public ScriptObject
	{
	public:
		JPEGXREncoderOptionsObject(VTable* _vtable,ScriptObject* _delegate,int capacity);
	
		Stringp  m_ColorSpace;
		uint32_t m_quantization;
		uint32_t m_trimFlexBits;
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::JPEGXREncoderOptionsObjectSlots m_slots_JPEGXREncoderOptionsObject;
	};
}

#endif //_AS3JPEGXREncoderOptions_