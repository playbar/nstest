#ifndef _AS3ElementFormat_
#define _AS3ElementFormat_

#include "AS3FontDescription.h"
#include "AS3FontMetrics.h"

namespace avmplus{namespace NativeID{
class ElementFormatClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class ElementFormatObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FontMetricsObject;
	class ElementFormatObject;
	class ElementFormatClass : public ClassClosure
	{
	public:
		ElementFormatClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		ElementFormatObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ElementFormatClassSlots m_slots_ElementFormatClass;
};
class ElementFormatObject : public ScriptObject
{
	public:
		ElementFormatObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DRCWB(FontDescriptionObject*) m_pFontDesc;
		DRCWB(FontMetricsObject*) m_pMetrics;
		DRC(Stringp) m_strDigitCase;
		DRC(Stringp) m_strDigitWidth;
		DRC(Stringp) m_strKerning;
		DRC(Stringp) m_strLigatureLevel;
		DRC(Stringp) m_strLocal;
		DRC(Stringp) m_strRotation;
		DRC(Stringp) m_strTypographicCase;
		int	m_nBaseLine,m_nBreakMode,m_nDomainBaseLine;
		double m_alpha;
		int    m_baselineShift,m_nFontSize,m_nTrackingLeft,m_nTrackingRight;
		bool   m_bLocked;
		XU32   m_nColor;
		
	public:
		Stringp AS3_alignmentBaseline_get();
		void AS3_alignmentBaseline_set(Stringp alignmentBaseline);
		double AS3_alpha_get();
		void AS3_alpha_set(double alpha);
		double AS3_baselineShift_get();
		void AS3_baselineShift_set(double baselineShift);
		Stringp AS3_breakOpportunity_get();
		void AS3_breakOpportunity_set(Stringp breakOpportunity);
		uint32_t AS3_color_get();
		void AS3_color_set(uint32_t color);
		Stringp AS3_digitCase_get();
		void AS3_digitCase_set(Stringp digitCase);
		Stringp AS3_digitWidth_get();
		void AS3_digitWidth_set(Stringp digitWidth);
		Stringp AS3_dominantBaseline_get();
		void AS3_dominantBaseline_set(Stringp dominantBaseline);
		FontDescriptionObject* AS3_fontDescription_get();
		void AS3_fontDescription_set(FontDescriptionObject *pFontDescription);
		double AS3_fontSize_get();
		void AS3_fontSize_set(double fontSize);
		Stringp AS3_kerning_get();
		void AS3_kerning_set(Stringp kerning);
		Stringp AS3_ligatureLevel_get();
		void AS3_ligatureLevel_set(Stringp ligatureLevel);
		Stringp AS3_locale_get();
		void AS3_locale_set(Stringp locale);
		bool AS3_locked_get();
		void AS3_locked_set(bool locked);
		Stringp AS3_textRotation_get();
		void AS3_textRotation_set(Stringp pTextRotation);
		double AS3_trackingLeft_get();
		void AS3_trackingLeft_set(double trackingLeft);
		double AS3_trackingRight_get();
		void AS3_trackingRight_set(double trackingRight);
		Stringp AS3_typographicCase_get();
		void AS3_typographicCase_set(Stringp typographicCase);
		//AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		//AvmBox AS3_clone(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		FontMetricsObject* AS3_getFontMetrics();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::ElementFormatObjectSlots m_slots_ElementFormatObject;
};}
#endif