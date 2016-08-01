#ifndef _AS3TextField_
#define _AS3TextField_

class EditText;

#include "AS3InteractiveObject.h"

namespace avmplus{namespace NativeID{
class TextFieldClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextFieldObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextFormatObject;
	class StyleSheetObject;
	class TextLineMetricsObject;
	class TextFieldClass : public ClassClosure
	{
	public:
		
		TextFieldClass(VTable *vtable);

		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		//new adds
		bool AS3_isFontCompatible(Stringp pFontName, Stringp pFontStyle);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFieldClassSlots m_slots_TextFieldClass;
};
class TextFieldObject : public InteractiveObjectObject
{
	private:
		EditText* GetEditObject();
	public:
//#if (__CORE_VERSION__>=0x02079000)
//		bool		   m_bHtmlNewLine;
//#endif
		bool		   m_bPosX;
		int			   m_nPosX;
		double		   m_nWidth0,m_nHeight0,m_nOldPos;
		DRCWB(Stringp) m_strFitType;
		DRCWB(Stringp) m_strRestrict;
		DRCWB(StyleSheetObject*) m_pSheet;
		void CheckID(int id);
		void CheckLine(int iline);
		TextFieldObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual ~TextFieldObject();
		virtual void InitObject();
		void CalcAutoSize(double w,bool bAuto);
		void SetWidth(double w,bool b);
	public:
		bool AS3_alwaysShowSelection_get();
		void AS3_alwaysShowSelection_set(bool b);
		Stringp AS3_antiAliasType_get();
		void AS3_antiAliasType_set(Stringp strType);
		Stringp AS3_autoSize_get();
		void AS3_autoSize_set(Stringp autoSize);
		bool AS3_background_get();
		void AS3_background_set(bool b);
		uint32_t AS3_backgroundColor_get();
		void AS3_backgroundColor_set(uint32_t c);
		bool AS3_border_get();
		void AS3_border_set(bool b);
		uint32_t AS3_borderColor_get();
		void AS3_borderColor_set(uint32_t c);
		int AS3_bottomScrollV_get();
		int AS3_caretIndex_get();
		bool AS3_condenseWhite_get();
		void AS3_condenseWhite_set(bool b);
		TextFormatObject* AS3_defaultTextFormat_get();
		void AS3_defaultTextFormat_set(TextFormatObject*);
		bool AS3_displayAsPassword_get();
		void AS3_displayAsPassword_set(bool b);
		bool AS3_embedFonts_get();
		void AS3_embedFonts_set(bool b);
		Stringp AS3_gridFitType_get();
		void AS3_gridFitType_set(Stringp strType);
		Stringp AS3_htmlText_get();
		void AS3_htmlText_set(Stringp);
		int AS3_length_get();
		int AS3_maxChars_get();
		void AS3_maxChars_set(int l);
		int AS3_maxScrollH_get();
		int AS3_maxScrollV_get();
		bool AS3_mouseWheelEnabled_get();
		void AS3_mouseWheelEnabled_set(bool b);
		bool AS3_multiline_get();
		void AS3_multiline_set(bool b);
		int AS3_numLines_get();
		Stringp AS3_restrict_get();
		void AS3_restrict_set(Stringp);
		int AS3_scrollH_get();
		void AS3_scrollH_set(int v);
		int AS3_scrollV_get();
		void AS3_scrollV_set(int);
		bool AS3_selectable_get();
		void AS3_selectable_set(bool b);
		int AS3_selectionBeginIndex_get();
		int AS3_selectionEndIndex_get();
		double AS3_sharpness_get();
		void AS3_sharpness_set(double);
		StyleSheetObject* AS3_styleSheet_get();
		void AS3_styleSheet_set(StyleSheetObject*);
		Stringp AS3_text_get();
		void AS3_text_set(Stringp);
		uint32_t AS3_textColor_get();
		void AS3_textColor_set(uint32_t);
		double AS3_textHeight_get();
		double AS3_textWidth_get();
		double AS3_thickness_get();
		void AS3_thickness_set(double);
		Stringp AS3_type_get();
		void AS3_type_set(Stringp);
		bool AS3_useRichTextClipboard_get();
		void AS3_useRichTextClipboard_set(bool b);
		bool AS3_wordWrap_get();
		void AS3_wordWrap_set(bool b);
		void AS3_constructor();
		void AS3_appendText(Stringp);
		RectangleObject* AS3_getCharBoundaries(int);
		int AS3_getCharIndexAtPoint(double x,double y);
		int AS3_getFirstCharInParagraph(int);
		DisplayObjectObject* AS3_getImageReference(Stringp);
		int AS3_getLineIndexAtPoint(double x,double y);
		int AS3_getLineIndexOfChar(int);
		int AS3_getLineLength(int);
		TextLineMetricsObject* AS3_getLineMetrics(int);
		int AS3_getLineOffset(int);
		Stringp AS3_getLineText(int);
		int AS3_getParagraphLength(int);
		TextFormatObject* AS3_getTextFormat(int,int);
		void AS3_replaceSelectedText(Stringp);
		void AS3_replaceText(int,int,Stringp);
		void AS3_setSelection(int,int);
		void AS3_setTextFormat(TextFormatObject*,int,int);
		double AS3_width_get();
		void AS3_width_set(double);
		double AS3_height_get();
		void AS3_height_set(double);
		double AS3_x_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_x_set(double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		double AS3_y_get();//AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		void AS3_y_set(double);//AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds
		Stringp AS3_textInteractionMode_get();
		//new adds end
		//void PrintfAddr(char* c);

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextFieldObjectSlots m_slots_TextFieldObject;
};}
#endif