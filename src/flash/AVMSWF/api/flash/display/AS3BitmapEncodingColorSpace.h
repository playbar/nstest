#ifndef _AS3BitmapEncodingColorSpace_
#define _AS3BitmapEncodingColorSpace_
namespace avmplus
{
	namespace NativeID
	{
		class BitmapEncodingColorSpaceClassSlots
		{
			friend class SlotOffsetsAndAsserts;
		public:
			Stringp COLORSPACE_4_2_0;
			Stringp COLORSPACE_4_2_2;
			Stringp COLORSPACE_4_4_4;
			Stringp COLORSPACE_AUTO;
		private:
		};
		class BitmapEncodingColorSpaceObjectSlots
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
	class BitmapEncodingColorSpaceClass: public ClassClosure
	{
	public:
		BitmapEncodingColorSpaceClass(VTable* vtable);
		ScriptObject* createInstance(VTable* ivtable, ScriptObject* delegate);

		inline Stringp getSlotCOLORSPACE_4_2_0() {return m_slots_BitmapEncodingColorSpaceClass.COLORSPACE_4_2_0;}
		inline Stringp getSlotCOLORSPACE_4_2_2() {return m_slots_BitmapEncodingColorSpaceClass.COLORSPACE_4_2_2;}
		inline Stringp getSlotCOLORSPACE_4_4_4() {return m_slots_BitmapEncodingColorSpaceClass.COLORSPACE_4_4_4;}
		inline Stringp getSlotCOLORSPACE_AUTO() {return m_slots_BitmapEncodingColorSpaceClass.COLORSPACE_AUTO;}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapEncodingColorSpaceClassSlots m_slots_BitmapEncodingColorSpaceClass;
	};

	class BitmapEncodingColorSpaceObject: public ScriptObject
	{
	public:
		BitmapEncodingColorSpaceObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData; //Set your Data!!!

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::BitmapEncodingColorSpaceObjectSlots m_slots_BitmapEncodingColorSpaceObject;
	};
}


#endif //_AS3BitmapEncodingColorSpace_