#ifndef _AS3TextBlock_
#define _AS3TextBlock_

#include "AS3FontDescription.h"
#include "AS3ContentElement.h"
#include "AS3TextJustifier.h"

namespace avmplus{namespace NativeID{
class TextBlockClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class TextBlockObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class TextLineObject;

	class TextBlockClass : public ClassClosure
	{
	public:
		TextBlockClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextBlockClassSlots m_slots_TextBlockClass;
};
class TextBlockObject : public ScriptObject
{
	public:
		enum{CHAR_NUM,CHAR_ALPHA,CHAR_SIGN};
		static int TypeOfChar(const wchar c);
		TextBlockObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	private:
		XBOOL m_bApplyNonLinearFontScaling;
		DRCWB(FontDescriptionObject*) m_pFontDesc;
		DRCWB(ContentElementObject*) m_pContent;
		DRCWB(TextLineObject*)		 m_pFirstInvalidLine;
		DRCWB(TextLineObject*)		 m_pFirstLine;
		DRCWB(TextLineObject*)		 m_pLastLine;
		DRCWB(ObjectVectorObject*)   m_pTabStops;
		DRCWB(TextJustifierObject*)  m_pTextJustifier;
		DRC(Stringp) m_strBaselineZero;
		DRC(Stringp) m_strLineRotation;
		DRC(Stringp) m_strResult;
		double m_fBaseLineFontSize;
		int	   m_nBidiLevel;
	public:
		bool	AS3_applyNonLinearFontScaling_get();

		void	AS3_applyNonLinearFontScaling_set(bool value);

		FontDescriptionObject*	AS3_baselineFontDescription_get();

		void	AS3_baselineFontDescription_set(FontDescriptionObject *pValue);

		double	AS3_baselineFontSize_get();

		void	AS3_baselineFontSize_set(double value);

		Stringp	AS3_baselineZero_get();

		void	AS3_baselineZero_set(Stringp value);

		ContentElementObject*	AS3_content_get();

		void	AS3_content_set(ContentElementObject *pValue);

		int		AS3_bidiLevel_get();

		void	AS3_bidiLevel_set(int value);

		TextLineObject*	AS3_firstInvalidLine_get();

		TextLineObject*	AS3_firstLine_get();

		TextLineObject*	AS3_lastLine_get();

		TextJustifierObject*	AS3_getTextJustifier();

		void	AS3_setTextJustifier(TextJustifierObject *pValue);

		Stringp	AS3_textLineCreationResult_get();

		Stringp	AS3_lineRotation_get();

		void	AS3_lineRotation_set(Stringp value);

		ObjectVectorObject*	AS3_getTabStops();

		void	AS3_setTabStops(ObjectVectorObject *pValue);

		int		AS3_findNextAtomBoundary(int afterCharIndex);

		int		AS3_findPreviousAtomBoundary(int beforeCharIndex);

		int		AS3_findNextWordBoundary(int afterCharIndex);

		int		AS3_findPreviousWordBoundary(int beforeCharIndex);

		TextLineObject*	AS3_getTextLineAtCharIndex(int charIndex);

		TextLineObject*	AS3_DoCreateTextLine(TextLineObject *pPreviousLine, double width, double lineOffset, bool fitSomething, TextLineObject *pReuseLine);

		void	AS3_releaseLineCreationData();

		void	AS3_releaseLines(TextLineObject *pFirstLine, TextLineObject *pLastLine);

		Stringp	AS3_dump();

	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextBlockObjectSlots m_slots_TextBlockObject;
};}
#endif