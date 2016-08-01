/****************************************************************************
CONFIDENTIAL AND PROPRIETARY INFORMATION.  The entire contents of this file
is Copyright ?Macromedia, Inc. 1993-1998, All Rights Reserved.  This
document is an unpublished trade secret of Macromedia, Inc. and may not be
viewed, copied or distributed by anyone, without the specific, written
permission of Macromedia, Inc. 
****************************************************************************/

// The object is responsible for generating edges and colors in a format 
//	that the scan converter can use.

#ifndef EDITTEXT_INCLUDED
#define EDITTEXT_INCLUDED

#include "shape.h"	// Added by ClassView
#include "stags.h"
#include "sdisplay.h"
//#include "XFontDraw.h"

#define editTextNewline '\r'
#define editTextSoftNewline '\n'
#define editTextNewHTMLLine '\r'-1
#define editTextSpace ' '
#ifdef EDITTEXT
struct _XSObject;
struct _XSCharacter;
struct _XSTransform;
class XSWFPlayer;
class XSWFCONTEXT;
class CharsData
{
public:
	CharsData();
	~CharsData();
	GLTranglesOrLines* GetCharData(short nChar,int nSize,const char*fontName);
private:
	static CharsData* m_pInstance;
public:
	static CharsData* GetInst();
	static void Release();
};
struct _XTEXTFORMAT
{
	 U8 align;//Stringp align;
	 short blockIndent;
	 bool  Bold;
	 bool  bullet;
	 U32   color;	 
	 short indent;
	 bool  italic;
	 short kerning;
	 short leading;
	 short leftMargin;
	 short letterSpacing;
	 short rightMargin;
	 _XFontCharInfo*dotFont;
	 int   fontSize;
	 U32   size;
	 U32   spaceHeight;
	 //ArrayObject* tabStops;
	 XXVar font;
	 XXVar target;
	 XXVar strID,strSrc;
	 XXVar txtEvent;
	 bool  underline;
	 XXVar url;
	 _XTEXTFORMAT*parent;
	 _XTEXTFORMAT*child;
	 _XTEXTFORMAT*next;
	 U32 iStart,iEnd;
	 void Release()
	 {
		 font.Release();
		 target.Release();
		 url.Release();
	 }
	 void Copy(_XTEXTFORMAT&o)
	 {
		 align=o.align;
		 blockIndent=o.blockIndent;
		 Bold=o.Bold;
		 bullet=o.bullet;
		 spaceHeight=o.spaceHeight;
		 color=o.color;
		 indent=o.indent;
		 italic=o.italic;
		 dotFont=o.dotFont;
		 kerning=o.kerning;
		 leading=o.leading;
		 letterSpacing=o.letterSpacing;
		 leftMargin=o.leftMargin;
		 rightMargin=o.rightMargin;
		 size=o.size;
		 font=o.font;
		 target=o.target;
		 underline=o.underline;
		 url=o.url;
		 txtEvent=o.txtEvent;
		 iStart=o.iStart;
		 iEnd=o.iEnd;
	 }
	 void Init()
	 {
		 align=stextAlignLeft;
		 blockIndent=0;
		 Bold=0;
		 bullet=0;
		 color=0;
		 indent=0;
		 italic=0;
		 spaceHeight=0;
		 kerning=0;
		 leading=0;
		 leftMargin=0;
		 letterSpacing=0;
		 rightMargin=0;
		 size=0;
		 dotFont=NULL;
		 parent=NULL;
		 child=NULL;
		 next=NULL;
		 iStart=0;
		 font=_strEmpty;
		 url=_strEmpty;
		 strID=_strEmpty;
		 strSrc=_strEmpty;
		 target=_strEmpty;
		 txtEvent=_strEmpty;
		 iEnd=0xffffffff;
	 }
	 _XTEXTFORMAT* _Parent(){return parent;}

	 _XTEXTFORMAT* _NextFormat(U32 id,_XTEXTFORMAT*_root)
	 {
		 if(id>=iEnd)
		 {
			if(next)
			{
				if(id>=next->iStart)
				{
					if(!next->child)
						return next;
					else
					{
						_XTEXTFORMAT*fmt0=next;
						_XTEXTFORMAT*fmt=next->child;
						while(fmt->iStart<=id)
						{
							if(!fmt->child) return fmt;
							fmt0=fmt;
							fmt=fmt->child;
							//if(fmt->iStart>id) break;
						}
						return fmt0;
					}
				}
				else
					return _root->_GetFormat(id);
			}
			else	 return _root->_GetFormat(id);
		 }
		 else if(child)
		 {
			 _XTEXTFORMAT*c=child;
			 while(c)
			 {
				 _XTEXTFORMAT*cc=c->_GetFormat(id);
				 if(cc) return cc;
				 //if(id>=c->iStart&&id<c->iEnd) return c;
				 c=c->next;
			 }
			 //if(id>=child->iStart) return child;
		 }
		 return this;
	 }

	 void Free(DisplayList*disp)
	 {
		 while(child)
		 {
			 child->Free(disp);
			 child=child->next;
		 }
		 Release();
		 disp->FreeFormat(this);
	 }

	 _XTEXTFORMAT* _NewChild(int iStart,DisplayList*disp)
	 {
		 _XTEXTFORMAT*c=disp->CreateFormat();
		 if(c)
		 {
			//c->Init();
			c->Copy(*this);			
			c->child=XNULL;
			c->next=XNULL;
			c->iStart=iStart;
			c->parent=this;
			if(!child)
				child=c;
			else
			{
				_XTEXTFORMAT* p=child;
				for(;;)
				{
					if(!p->next) {p->next=c;break;}
					p=p->next;
				}
			}
		 }
		 return c;
	 }
	 void ExpandAt(U32 i,U32 l)
	 {
		 _XTEXTFORMAT*p=_GetFormat(i);
		 while(p)
		 {
			 p->iEnd+=l;
			 _XTEXTFORMAT*pc=p->next;
			 while(pc)
			 {
				 pc->iStart+=l;
				 pc->iEnd+=l;
				 pc=pc->next;
			 }
			 p=p->_Parent();
		 }

	 }
	 _XTEXTFORMAT* _GetFormat(U32 id)
	 {
		 if(id<iStart||id>=iEnd) return NULL;
		 _XTEXTFORMAT*p=child;
		 while(p)
		 {
			 _XTEXTFORMAT*pf=p->_GetFormat(id);
			 if(pf) return pf;
			 p=p->next;
		 }
		 if(child)
		 {
			 _XTEXTFORMAT*fmt0=this;
			 _XTEXTFORMAT*fmt=child;
			 while(fmt->iStart<=id&&fmt->iEnd>id)
			 {
				 if(!fmt->child) return fmt;
				fmt0=fmt;
				fmt=fmt->child;
			 }
			 return fmt0;
		 }
		 return this;
	 }
};


enum {
	editTextGetScroll,
	editTextSetScroll,
	editTextGetMaxScroll
};

int WideStrLen(const U16 *buf);
void WideStrCopy(U16 *dst, const U16 *src);

struct _XCHARINFO
{
	float charWidth;
	int charSize;
	//int pixelWidth;
	//int charHeight;
	int charCode;
	//int charType;
	//int charPixel;
	void* pTL;
	int letterSpace;
	int lineSpace;
	//int adjust;
	_XTEXTFORMAT* pFormat;
};
struct _XLINEINFO
{
public:
	int nStart;
	int nMaxScroll;
	int nHeight;
	int lMargin,rMargin,indent;
	int x;
//#if (__CORE_VERSION__>=0x02070100)
//	int xPixelStart;
//#endif
	//int nPixelWidth,nPixelHeight;
	int nAscent,nDescent,nLeading,nWidth,nAdjust;
};

//extern _XTEXTFORMAT* FindFormat(_XTEXTFORMAT*fmt,XUINT iStart,XUINT iEnd);

class EditText
{
public:
	int	 m_nDeltaY;
	void SetDeltay(int n){m_nDeltaY=n;}
	void ClearFormat();
	bool m_bNoSize,m_bModifyHTML;
	int  stageW,stageH,nTextWidth,nTextHeight;
	void CalcLineInfo(STransform*xt);
	XBOOL UpdateHTMLText();
	void InitHTMLText(const char* strTxt,int l);
	void Release();
	void SetURLFlag();
	_XTEXTFORMAT* m_pFormat;
	void SetFilterSize(_XTEXTFORMAT*fmt);
public:
	void TranslatePoint(int&lx,int&ly);
	_XTEXTFORMAT* GetFormat(int x,int y);
	enum {ANTITYPE_NORMAL,ANTITYPE_ADVANCED};

	inline bool IsCondenseWhite()
	{
		return (m_flags&seditTextCondenseWhite)!=0;
	}
	inline void SetCondenseWhite(bool b)
	{
		if(b)
			m_flags|=seditTextCondenseWhite;
		else
			m_flags&=~seditTextCondenseWhite;
		m_obj->Modify();
	}
	inline bool IsPassword()
	{
		return (m_flags&seditTextFlagsPassword)!=0;
	}
	inline void SetPassword(bool b)
	{
		if(b)
			m_flags|=seditTextFlagsPassword;
		else
			m_flags&=~seditTextFlagsPassword;
		m_obj->Modify();
	}
	inline _XTEXTFORMAT* GetDefaultFormat(){return m_pFormat;}
	
	inline bool IsAlwaysShowSelection()
	{
		return (m_flags&seditTextAlwaysShowSelection)!=0;
	}
	inline void SetAlwaysShowSelection(bool b)
	{
		if(b) m_flags|=seditTextAlwaysShowSelection;
		else m_flags&=~seditTextAlwaysShowSelection;
		m_obj->Modify();
	}

	XBOOL IsEditAble(bool bDrawn=true);

	void SelectAll();
	XU32 GetEditStyle();
	void SetVariable(XXVar&var);//XPCTSTR str);
	XBOOL IsMultiLine()
	{
		return m_flags&seditTextFlagsMultiline;
	}
	void SetMultiLine(XBOOL b)
	{
		if(b)
			m_flags|=seditTextFlagsMultiline;
		else
			m_flags&=~seditTextFlagsMultiline;
		m_obj->Modify();
	}
	inline bool IsSelectable()
	{
		return (m_flags&seditTextFlagsNoSelect)==0;
	}
	inline void SetSelectable(bool b)
	{
		if(b)
			m_flags&=~seditTextFlagsNoSelect;
		else
			m_flags|=seditTextFlagsNoSelect;
	}
	void SetHTMLText(XSWFCONTEXT*cnt,XPCTSTR strTxt);
	void UpdateToVariable(XSWFCONTEXT&cnt,XXVar&var);//XPCTSTR strTxt);
	int CalcMaxHScroll();
	XBOOL IsHtml()
	{
		return m_flags&seditTextFlagsHTML;
	}
	void SetHtml(XBOOL b)
	{
		if(b)
			m_flags|=seditTextFlagsHTML;
		else
			m_flags&=~seditTextFlagsHTML;
	}
	XBOOL IsUseFont()
	{
		return m_flags&seditTextFlagsUseOutlines;
	}
	void SetUseFont(XBOOL b)
	{
		if(b)
			m_flags|=seditTextFlagsUseOutlines;
		else
			m_flags&=~seditTextFlagsUseOutlines;
	}
	XU32 GetBackColor()
	{

		XU32 color = (m_backColor.rgb.red << 16) | 
					 (m_backColor.rgb.green << 8 ) | 
					 (m_backColor.rgb.blue ) | 
					 (m_backColor.rgb.transparency << 24 );
		return color;

	}
	void SetBackColor(XU32 nColor) 
	{

		m_backColor.rgb.transparency = 0xFF;
		m_backColor.rgb.red = (0x00FF0000 & nColor ) >> 16;
		m_backColor.rgb.green = ( 0x0000FF00 & nColor )>> 8;
		m_backColor.rgb.blue = ( 0x000000FF &nColor );

		m_obj->Modify();
	}
	XBOOL IsBackground()
	{
		return (m_flags&seditTextFlagsFillBackground);
	}
	XBOOL IsBorder()
	{
		return m_flags&seditTextFlagsBorder;
	}
	void SetBorder(XBOOL b)
	{
		if(b)
			m_flags|=seditTextFlagsBorder;
		else
			m_flags&=~seditTextFlagsBorder;
		m_obj->Modify();
	}
	XU32 GetTextColor()
	{
		return m_pFormat->color;//m_textColor.all;
	}
	void SetTextColor(XU32 c)
	{
		//m_textColor.all=c;
		m_pFormat->color=c;
		_XTEXTFORMAT*p=m_pFormat->child;
		while(p)
		{
			p->Free(m_obj->display);
			p=p->next;
		}
		m_pFormat->child=XNULL;
		m_obj->Modify();
		m_bModifyHTML=true;
	}
	void SetBorderColor(XU32 c)
	{

		m_borderColor.rgb.transparency = 0xFF;
		m_borderColor.rgb.red = (0x00FF0000 & c ) >> 16;
		m_borderColor.rgb.green = ( 0x0000FF00 & c )>> 8;
		m_borderColor.rgb.blue = ( 0x000000FF &c );

	}
	XU32 GetBorderColor()
	{

		XU32 color = (m_borderColor.rgb.red << 16) | 
			(m_borderColor.rgb.green << 8 ) | 
			(m_borderColor.rgb.blue ) | 
			(m_borderColor.rgb.transparency << 24 );
		return color;

	}
	void SetBackground(XBOOL b)
	{
		if(b)
			m_flags|=seditTextFlagsFillBackground;
		else
			m_flags&=~seditTextFlagsFillBackground;
		m_obj->Modify();
	}
	void AutoSize(XBOOL b);
	XBOOL IsAutoSize(){return (m_flags&seditTextAutoSize);}
	void ReplaceText(int ns,int ne,XPCTSTR strTxt);
	void ReplaceSel(XPCTSTR strTxt);
	XBOOL UpdateVariable(XXVar&var);
	// next is pointer to next EditText in update list
	EditText *pNext;
	//EditText *next;

	enum {
		PASSWORD_CHAR   = '*'
	};

	EditText(_XSObject* obj);
	void Init(_XSObject*obj);
	~EditText();

	/*! HandleKeyDown processes keys for the edit text fields. The NativePlayerWnd 
	    gets passed in so the edit text can request pastes and copies to the 
		clipboard.
	 */
	bool     HandleKeyDown( int key, int modifiers,bool bChar);//, NativePlayerWnd* native );
	void	 GetBuffer(XXVar&var);
	void     SetBuffer(XPCTSTR text,bool bGB2312);
	void	 SetBuffer(XPCWSTR text,int l,bool bEvent=false);
	void     DoMouse(SPOINT *pt, BOOL mouseIsDown);
	void	 DoClick(int x,int y);
	XBOOL    HasClickEvent();
	void	 DblClick(SPOINT*);
	void     ClearSelection();
	BOOL     IsSelected() { return m_selectionStart != m_selectionEnd; }

	enum {
		kCutEnabled			= 1,
		kCopyEnabled		= 2,
		kPasteEnabled		= 4,
		kClearEnabled		= 8,
		kSelectAllEnabled	= 16
	};
 	void	GetEnabledCommands(int& flags);

	BOOL	Draw(STransform* x, BOOL buildEdges,_XSObject*obj,_XSObject*charObj,bool justUpdate=false);//bool bFilter);

	BOOL IsFontIncluded() { return (m_flags & seditTextFlagsUseOutlines) != 0; }
	bool		m_bURL;
	U16		   *m_buffer;
	XXVar		m_variable;
	XXVar		m_initialText;
	//XString8	m_htmlText;
	U8			m_nAntiType;
	int			m_sharpness;
	int         m_length;
	int         m_selectionStart, m_selectionEnd,m_caretIndex;
	int         m_hscroll, m_vscroll;
	MATRIX		m_mat;
	//int		   *m_lineX;
	BOOL        m_mouseIsDown;
	S32			m_clickTime;
	SPOINT      m_mousePosition;
	BOOL        m_selecting;
	U32			m_flags;//,m_flags1;
	int		    m_maxLength;
	//int         m_height;
	//int        *m_lineStarts,*m_lineHeights;
	int         m_numLines;
	BOOL	    m_atLineEnd;
	_XSObject*	m_obj;
	SCharacter *m_character;
	BOOL		m_drawn;
	BOOL		m_doFindCursor;
	int			m_heldLeadByte;
	int 		m_autoMode;
	static BOOL m_insertMode;

	SRECT editRect;
	SRECT editBounds;
	//int devAscent;
	//int devLineSpacing;
	_XCHARINFO* charsInfo;
	_XLINEINFO* lineInfo;
	//int *devCharWidths,*devCharHeights;

	void Expand(int size);
	void Insert(U16 ch) { InsertWideChars(&ch, 1); }
	void Insert(char *s, int len,bool bEvent=false);
	void InsertWideChars(U16 *s, int len,bool bEvent=false);
	void Backward(int wholeWord, int extend);
	void Forward(int wholeWord, int extend);
	void Backspace();
	void Delete();
	void DeleteSelection();
	void FindCursor(SRECT* devBounds, _XCHARINFO*info);//int *devCharWidths);
	void CopyToClipboard( );//NativePlayerWnd* );
	void PasteFromClipboard( );//NativePlayerWnd* );
	int  FindLineStart(int pos) { return lineInfo?lineInfo[FindLineNumber(pos)].nStart:0; }
	int  FindLineEnd(int pos);
	int  FindLineNumber(int pos);
	void AutoScroll();
	void CalculateLineStarts(MATRIX&mat,_XCHARINFO*info, int width,SFIXED a,int iAscent,int iDescent);//, int indent);

	struct FontDesc {
		char fontName[256];
		int height;
		BOOL boldFlag;
		BOOL italicFlag;
		BOOL japaneseFlag;
		SCharacter* font;
		S32 codeOffset;
	};

	void GetFontDesc(FontDesc& fontDesc,XU16* text);

	BOOL IsLeadByte(U8 ch);
	BOOL IsMultiByte();
	U16 *MBCSToWide(char *buffer, int index, int count,bool bGB);
	void WideToMBCS(U16 *buffer, int index, int count,XXVar&var);
	static U16 FindGlyph(U16 targetCode, FontDesc& fontDesc);
	U16 *FindGlyphs(U16 *buffer, int length, FontDesc& fontDesc);

	int CalcVisibleLines(int iStart);
	int CalcMaxVScroll();

	void UpdateFromVariable(XBOOL bInit);//XSWFCONTEXT&cnt);

	BOOL BreakEnglish(U16 ch1, U16 ch2);
	BOOL LineBreakOK(U16 ch1, U16 ch2);
	BOOL LineBreakOK_Index(int index);
	void GetTextSize(double&w,double&h,bool bTrans=true);
	void GetCharBoundaries(int id,SRECT&rect);
	int  GetCharIndextAtPoint(int x,int y);
	int  GetFirstCharInParagraph(int id);
	const char* GetImageRefrence(char* url);
	int  GetLineIndexAtPoint(int x,int y);
	int  GetLineIndexOfChar(int);
	int  GetLineLength(int);
	_XLINEINFO*  GetLineMetrics(int i){return lineInfo?&lineInfo[i]:XNULL;}
	int	 GetLineOffset(int);
	U16* GetLineText(int&);
	int  GetPararaphLength(int);
	_XTEXTFORMAT* GetTextFormat(int b,int e);
	void replaceSelectText(U16* pText,int l);

	void replaceWideText(U16*pText,int l,int b,int e,bool bFormat=false);

	void setSelection(int b,int e);
	void AppendText(U16*pText,int l);
	void AppendText(U16*pText,int l,_XTEXTFORMAT*pFmt);
	_XTEXTFORMAT* AppendSpace(int nWidth,int Height);
	void UpdateIfNeed();
public:
	void Select(int s,int e)
	{
		if(m_length<=0) return;
		if(e<s) e=s;
		if(s<0) s=0;
		if(e>=m_length) e=m_length-1;
		m_selectionStart=s;
		m_selectionEnd=s;
		m_obj->Modify();
	}
	void SetFormat(void*fmt,XU32 iStart,XU32 iEnd);
protected:
	
	void ClearEditVariable();
	XBOOL TransHTMLText(XPCTSTR strTxt,XXVar&dst);
	SRGB  m_backColor,m_borderColor; /*m_textColor,*/
//#if (__CORE_VERSION__>=0x02073000)
//	int m_nRevScale;
//	XBOOL m_bDotShow;
//#endif
};

// The SaveFocus object is used to save and restore the current focus.

typedef struct _XSaveFocus
{
	XXVar	variable;
	int		depth;
	int		selectionStart;
	int		selectionEnd;
	BOOL	selecting;

	void SaveFocus();
	void Save(XSWFPlayer* splayer);
	void Clear();
}SaveFocus;

#endif // EDITTEXT

#endif // EDITTEXT_INCLUDED


