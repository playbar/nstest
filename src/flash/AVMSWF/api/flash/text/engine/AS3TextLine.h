#ifndef _AS3TextLine_
#define _AS3TextLine_

#include "AS3TextBlock.h"
#include "AS3DisplayObject.h"
#include "AS3DisplayObjectContainer.h"

namespace avmplus{namespace NativeID{
class TextLineClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

	//MAX_LINE_WIDTH : int = 1000000

	int MAX_LINE_WIDTH;

private:};
class TextLineObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
	Atom userData;
private:};
}}
namespace avmshell{
	class TextBlockObject;
	class TextLineObject;
	class GraphicElementObject;
	class TextLineClass : public ClassClosure
	{
	public:
		TextLineClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
		TextLineObject* CreateObject();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineClassSlots m_slots_TextLineClass;
};
class TextLineObject : public DisplayObjectContainerObject
{
	public:
		TextLineObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
		virtual void InitObject();
	public:
		DRCWB(TextBlockObject*) m_pBlock;
		DRCWB(TextLineObject*)  m_pNext;
		DRCWB(TextLineObject*)  m_pPrev;
		DRCWB(ContentElementObject*) m_pElement;
		int							 m_nElementID;
		DRC(Stringp)		    m_strInvalid;
		int	 m_nStartChar,m_nEndChar;
		int  m_nAscent,m_nDescent,m_nTextWidth;
		int	 m_nTotalAscent,m_nTotalDescent;
		int  m_nMaxWidth;
		XBOOL m_bHasGraphic;
	public:
		void	AppendText(XU16*text,int length,void*pFmt,int nWidth);
		void	AppendObject(GraphicElementObject*gr);
		void	AppendDisplay(GraphicElementObject*gr);
		void	SetBounds();
		TextBlockObject*	AS3_textBlock_get();
		bool	AS3_hasGraphicElement_get();
		bool	AS3_hasTabs_get();
		TextLineObject*	AS3_nextLine_get();
		TextLineObject*	AS3_previousLine_get();
		double	AS3_ascent_get();
		double	AS3_descent_get();
		double	AS3_textHeight_get();
		double	AS3_textWidth_get();
		double	AS3_totalAscent_get();
		double	AS3_totalDescent_get();
		double	AS3_totalHeight_get();
		int		AS3_textBlockBeginIndex_get();
		int		AS3_rawTextLength_get();
		double	AS3_specifiedWidth_get();
		double	AS3_unjustifiedTextWidth_get();
		Stringp	AS3_validity_get();
		void	AS3_validity_set(Stringp value);
		int		AS3_atomCount_get();
		ObjectVectorObject*	AS3_mirrorRegions_get();
		int		AS3_getAtomIndexAtPoint(double stageX, double stageY);
		int		AS3_getAtomIndexAtCharIndex(int charIndex);
		RectangleObject*	AS3_getAtomBounds(int atomIndex);
		int		AS3_getAtomBidiLevel(int atomIndex);
		Stringp	AS3_getAtomTextRotation(int atomIndex);
		int		AS3_getAtomTextBlockBeginIndex(int atomIndex);
		int		AS3_getAtomTextBlockEndIndex(int atomIndex);
		double	AS3_getAtomCenter(int atomIndex);
		bool	AS3_getAtomWordBoundaryOnLeft(int atomIndex);
		DisplayObjectObject*	AS3_getAtomGraphic(int atomIndex);
		double	AS3_getBaselinePosition(Stringp baseline);
		Stringp	AS3_dump();
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::TextLineObjectSlots m_slots_TextLineObject;
};}
#endif