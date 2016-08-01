#ifndef _AS3NationalDigitsType_
#define _AS3NationalDigitsType_
namespace avmplus{namespace NativeID{
class NationalDigitsTypeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

//ARABIC_INDIC : uint32_t = 0x0660
//BALINESE : uint32_t = 0x1B50
//BENGALI : uint32_t = 0x09E6
//CHAM : uint32_t = 0xAA50
//DEVANAGARI : uint32_t = 0x0966
//EUROPEAN : uint32_t = 0x0030
//EXTENDED_ARABIC_INDIC : uint32_t = 0x06F0
//FULL_WIDTH : uint32_t = 0xFF10
//GUJARATI : uint32_t = 0x0AE6
//GURMUKHI : uint32_t = 0x0A66
//KANNADA : uint32_t = 0x0CE6
//KAYAH_LI : uint32_t = 0xA900
//KHMER : uint32_t = 0x17E0
//LAO : uint32_t = 0x0ED0
//LEPCHA : uint32_t = 0x1C40
//LIMBU : uint32_t = 0x1946
//MALAYALAM : uint32_t = 0x0D66
//MONGOLIAN : uint32_t = 0x1810
//MYANMAR : uint32_t = 0x1040
//MYANMAR_SHAN : uint32_t = 0x1090
//NEW_TAI_LUE : uint32_t = 0x19D0
//NKO : uint32_t = 0x07C0
//OL_CHIKI : uint32_t = 0x1C50
//ORIYA : uint32_t = 0x0B66
//OSMANYA : uint32_t = 0x104A0
//SAURASHTRA : uint32_t = 0xA8D0
//SUNDANESE : uint32_t = 0x1BB0
//TAMIL : uint32_t = 0x0BE6
//TELUGU : uint32_t = 0x0C66
//THAI : uint32_t = 0x0E50
//TIBETAN : uint32_t = 0x0F20
//VAI : uint32_t = 0xA620

	uint32_t ARABIC_INDIC;
	uint32_t BALINESE;
	uint32_t BENGALI;
	uint32_t CHAM;
	uint32_t DEVANAGARI;
	uint32_t EUROPEAN;
	uint32_t EXTENDED_ARABIC_INDIC;
	uint32_t FULL_WIDTH;
	uint32_t GUJARATI;
	uint32_t GURMUKHI;
	uint32_t KANNADA;
	uint32_t KAYAH_LI;
	uint32_t KHMER;
	uint32_t LAO;
	uint32_t LEPCHA;
	uint32_t LIMBU;
	uint32_t MALAYALAM;
	uint32_t MONGOLIAN;
	uint32_t MYANMAR;
	uint32_t MYANMAR_SHAN;
	uint32_t NEW_TAI_LUE;
	uint32_t NKO;
	uint32_t OL_CHIKI;
	uint32_t ORIYA;
	uint32_t OSMANYA;
	uint32_t SAURASHTRA;
	uint32_t SUNDANESE;
	uint32_t TAMIL;
	uint32_t TELUGU;
	uint32_t THAI;
	uint32_t TIBETAN;
	uint32_t VAI;

private:};
class NationalDigitsTypeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class NationalDigitsTypeClass : public ClassClosure
	{
	public:
		NationalDigitsTypeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NationalDigitsTypeClassSlots m_slots_NationalDigitsTypeClass;
};
class NationalDigitsTypeObject : public ScriptObject
{
	public:
		NationalDigitsTypeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::NationalDigitsTypeObjectSlots m_slots_NationalDigitsTypeObject;
};}
#endif