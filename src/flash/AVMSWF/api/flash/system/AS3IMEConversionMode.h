#ifndef _AS3IMEConversionMode_
#define _AS3IMEConversionMode_
namespace avmplus{namespace NativeID{
class IMEConversionModeClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!
	Stringp ALPHANUMERIC_FULL;
	Stringp ALPHANUMERIC_HALF;
	Stringp CHINESE;
	Stringp JAPANESE_HIRAGANA; 
	Stringp JAPANESE_KATAKANA_FULL; 

	Stringp JAPANESE_KATAKANA_HALF;
	Stringp KOREAN;
	Stringp Unknown; 
private:};
class IMEConversionModeObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!

private:};
}}
namespace avmshell{
	class IMEConversionModeClass : public ClassClosure
	{
	public:
		enum
		{
			IME_ALPHANUMERIC_FULL=0,
			IME_ALPHANUMERIC_HALF=1,
			IME_CHINESE=2,
			IME_JAPANESE_HIRAGANA=3, 
			IME_JAPANESE_KATAKANA_FULL=4, 
			IME_JAPANESE_KATAKANA_HALF=5,
			IME_KOREAN=6,
			IME_Unknown=7,
		};
	public:
		IMEConversionModeClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	public:
		Stringp NameOf(int id)
		{
			switch(id)
			{
			case IME_ALPHANUMERIC_FULL:
				  return m_slots_IMEConversionModeClass.ALPHANUMERIC_FULL;
			case IME_ALPHANUMERIC_HALF:
				  return m_slots_IMEConversionModeClass.ALPHANUMERIC_HALF;
			default:
			case IME_CHINESE:
				  return m_slots_IMEConversionModeClass.CHINESE;
			case IME_JAPANESE_HIRAGANA:
				  return m_slots_IMEConversionModeClass.JAPANESE_HIRAGANA; 
			case IME_JAPANESE_KATAKANA_FULL:
				  return m_slots_IMEConversionModeClass.JAPANESE_KATAKANA_FULL; 
			case IME_JAPANESE_KATAKANA_HALF:
				 return m_slots_IMEConversionModeClass.JAPANESE_KATAKANA_HALF;
			case IME_KOREAN:
				 return m_slots_IMEConversionModeClass.KOREAN;
			case IME_Unknown:
				 return m_slots_IMEConversionModeClass.Unknown; 
			}
			return NULL;
		}
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IMEConversionModeClassSlots m_slots_IMEConversionModeClass;
};
class IMEConversionModeObject : public ScriptObject
{
	public:
		IMEConversionModeObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::IMEConversionModeObjectSlots m_slots_IMEConversionModeObject;
};}
#endif