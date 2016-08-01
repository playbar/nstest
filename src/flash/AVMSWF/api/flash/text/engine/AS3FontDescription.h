#ifndef _AS3FontDescription_
#define _AS3FontDescription_
namespace avmplus{namespace NativeID{
class FontDescriptionClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FontDescriptionObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FontDescriptionClass : public ClassClosure
	{
	public:
		FontDescriptionClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		bool AS3_isDeviceFontCompatible(Stringp fontName, Stringp fontWeight, Stringp fontPosture);
		bool AS3_isFontCompatible(Stringp fontName, Stringp fontWeight, Stringp fontPosture);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontDescriptionClassSlots m_slots_FontDescriptionClass;
};
class FontDescriptionObject : public ScriptObject
{
	public:
		FontDescriptionObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRC(Stringp) m_strCffHinting;
		DRC(Stringp) m_strFontLookup;
		DRC(Stringp) m_strFontName;
		DRC(Stringp) m_strFontPosture;
		DRC(Stringp) m_strFontWeight;
		DRC(Stringp) m_strRenderingMode;

		bool		 m_bLocked;
	public:
		Stringp AS3_cffHinting_get();
		void AS3_cffHinting_set(Stringp cffHinting);
		Stringp AS3_fontLookup_get();
		void AS3_fontLookup_set(Stringp fontLookup);
		Stringp AS3_fontName_get();
		void AS3_fontName_set(Stringp fontName);
		Stringp AS3_fontPosture_get();
		void AS3_fontPosture_set(Stringp fontPosture);
		Stringp AS3_fontWeight_get();
		void AS3_fontWeight_set(Stringp fontWeight);
		bool AS3_locked_get();
		void AS3_locked_set(bool locked);
		Stringp AS3_renderingMode_get();
		void AS3_renderingMode_set(Stringp renderingMode);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_clone(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FontDescriptionObjectSlots m_slots_FontDescriptionObject;
};}
#endif